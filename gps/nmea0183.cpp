#include <ppltasks.h>

#include "datum/bytes.hpp"
#include "datum/time.hpp"

#include "gps/nmea0183.hpp"
#include "gps/gparser.hpp"
#include "network/netexn.hpp"

#include "system.hpp"
#include "syslog.hpp"
#include "taskexn.hpp"

using namespace WarGrey::SCADA;

using namespace Concurrency;

using namespace Windows::Foundation;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;

/*************************************************************************************************/
IGPS::IGPS(Syslog* sl, Platform::String^ h, uint16 p, IGPSReceiver* cf) {
	this->logger = ((sl == nullptr) ? make_silent_logger("Silent GPS Receiver") : sl);
	this->logger->reference();

	this->push_confirmation_receiver(cf);
	this->service = p.ToString();
	this->device = ref new HostName(h);

    this->shake_hands();
};

IGPS::~IGPS() {
	if (this->socket != nullptr) {
		delete this->socket; // stop the confirmation loop before release transactions.
	}

	this->logger->destroy();
}

Platform::String^ IGPS::device_hostname() {
	return this->device->RawName;
}

Platform::String^ IGPS::device_description() {
	return this->device->RawName + ":" + this->service;
}

Syslog* IGPS::get_logger() {
	return this->logger;
}

void IGPS::push_confirmation_receiver(IGPSReceiver* confirmation) {
	if (confirmation != nullptr) {
		this->confirmations.push_back(confirmation);
	}
}

void IGPS::shake_hands() {
	this->clear();
	this->socket = ref new StreamSocket();
	this->socket->Control->KeepAlive = false;

	this->logger->log_message(Log::Debug, L">> connecting to device[%s]", this->device_description()->Data());

	create_task(this->socket->ConnectAsync(this->device, this->service)).then([this](task<void> handshaking) {
		try {
			handshaking.get();

			this->gpsin  = make_socket_available_reader(this->socket);
			this->refresh_data_size = 0;

			this->logger->log_message(Log::Debug, L">> connected to device[%s]", this->device_description()->Data());

			this->notify_connectivity_changed();
			this->wait_process_confirm_loop();
		} catch (Platform::Exception^ e) {
			this->logger->log_message(Log::Warning, socket_strerror(e));
			this->shake_hands();
		}
	});
}

void IGPS::wait_process_confirm_loop() {
	size_t pool_capacity = sizeof(this->data_pool) / sizeof(uint8);
	unsigned int request_max_size = (unsigned int)(pool_capacity - this->refresh_data_size);
	double receiving_ts = current_inexact_milliseconds();
	
	create_task(this->gpsin->LoadAsync(request_max_size)).then([=](unsigned int size) {
		if (size == 0) {
			task_fatal(this->logger, L"GPS[%s] has lost", this->device_description()->Data());
		}

		READ_BYTES(this->gpsin, this->data_pool + this->refresh_data_size, size);
		this->data_size = this->refresh_data_size + size;
	}).then([=](task<void> verify) {
		verify.get();

		this->notify_data_received(this->refresh_data_size, current_inexact_milliseconds() - receiving_ts);
		this->refresh_data_size = this->data_size;
		this->message_start = 0;
		
		do {
			size_t message_size = this->check_message();

			this->refresh_data_size -= message_size;
			
			if (this->CR_LF_idx > 0) {
				double confirming_ms = current_inexact_milliseconds();
				size_t endp1 = ((this->checksum_idx > 0) ? this->checksum_idx : this->CR_LF_idx);

				this->data_pool[this->field0_idx] = '\0';

				if (this->refresh_data_size <= 0) {
					this->logger->log_message(Log::Debug,
						L"<received %d-byte-size %S message coming from GPS[%s]>",
						message_size, this->data_pool + this->message_start, this->device_description()->Data());
				} else {
					this->logger->log_message(Log::Debug,
						L"<received %d-byte-size %S message coming from GPS[%s] along with extra %d bytes>",
						message_size, this->data_pool + this->message_start, this->device_description()->Data(),
						this->refresh_data_size);

					this->message_start += message_size;
				}

				this->apply_confirmation(this->data_pool, this->field0_idx, endp1);
				this->notify_data_confirmed(message_size, current_inexact_milliseconds() - confirming_ms);
			} else if (this->message_start == 0) {
				task_fatal(this->logger,
					L"message coming from device[%s] is overlength",
					this->device_description()->Data());
			}
		} while ((this->CR_LF_idx > 0) && (this->refresh_data_size > 0));
	}).then([=](task<void> confirm) {
		try {
			confirm.get();

			this->realign_message();
			this->wait_process_confirm_loop();
		} catch (task_discarded&) {
			this->realign_message();
			this->wait_process_confirm_loop();
		} catch (task_terminated&) {
			this->shake_hands();
		} catch (task_canceled&) {
			this->shake_hands();
		} catch (Platform::Exception^ e) {
			this->logger->log_message(Log::Warning, e->Message);
			this->shake_hands();
		}
	});
}

size_t IGPS::check_message() {
	unsigned short checksum = 0;
	size_t message_size = 0;
	size_t start_idx = this->message_start + 1;
	
	this->field0_idx = 0;
	this->checksum_idx = 0;
	this->CR_LF_idx = 0;
	
	for (size_t i = this->message_start; (i < this->data_size) && (this->CR_LF_idx == 0); i++) {
		unsigned char ch = this->data_pool[i];

		switch (ch) {
		case '$': case '!': start_idx = i; break;
		case ',': if (this->field0_idx == 0) this->field0_idx = i; break;
		case '*': this->checksum_idx = i; break;
		case 0x0D: this->CR_LF_idx = i; break;
		default: {
			if ((ch <= ' ') || (ch > '~')) {
				this->get_logger()->log_message(Log::Warning,
					L"message@%d coming from device[%s] constains non-printable ASCII character: 0x%02x@%d",
					this->message_start, this->device_description()->Data(), ch, i);
			}
		}
		}

		if ((i > this->message_start) && (this->checksum_idx == 0)) {
			checksum ^= ch;
		}
	}

	if (this->CR_LF_idx > 0) {
		if ((start_idx != this->message_start) || (this->field0_idx != this->message_start + 6)
			|| (this->data_pool[this->CR_LF_idx + 1] != 0x0A)
			|| ((this->checksum_idx > 0) && (this->checksum_idx != CR_LF_idx - 3))) {
			task_fatal(this->logger,
				L"message@%d coming from device[%s] is unrecognized('%c', '%c', %u, %u)",
				this->message_start, this->device_description()->Data(),
				this->data_pool[this->message_start], this->data_pool[this->message_start + 6],
				this->checksum_idx, this->CR_LF_idx);
		}

		if (this->checksum_idx > 0) {
			unsigned short signature
				= hexadecimal_ref(this->data_pool, this->checksum_idx + 1) * 16
				+ hexadecimal_ref(this->data_pool, this->checksum_idx + 2);

			if (checksum != signature) {
				task_fatal(this->logger,
					L"message@%d coming from GPS[%s] has been corrupted(signature: 0X%02X; checksum: 0X%02X)",
					this->message_start, this->device_description()->Data(), signature, checksum);
			}
		}

		message_size = this->CR_LF_idx + 2 - this->message_start;
	}

	return message_size;
}

void IGPS::realign_message() {
	if (this->refresh_data_size > 0) {
		memcpy(this->data_pool, this->data_pool + this->message_start, this->refresh_data_size);

		this->logger->log_message(Log::Debug,
			L"<realigned %d-byte-size partial message coming from GPS[%s]>",
			this->refresh_data_size, this->device_description()->Data());
	}
}

void IGPS::apply_confirmation(const unsigned char* pool, size_t start, size_t endp1) {
	long long timepoint = current_milliseconds();
	unsigned int type = message_type(pool, start - 3);
	size_t cursor = start + 1;

	switch (type) {
	case MESSAGE_TYPE('G', 'G', 'A'): {
		double utc = scan_scalar(pool, &cursor, endp1);
		double latitude = scan_vector(pool, &cursor, endp1, 'N', 'S');
		double longitude = scan_vector(pool, &cursor, endp1, 'E', 'W');
		NMEA_GQI quality = scan_gps_quality_indicator(pool, &cursor, endp1);
		unsigned long long satellites = scan_natural(pool, &cursor, endp1);
		double hdop = scan_scalar(pool, &cursor, endp1);
		double altitude = scan_vector(pool, &cursor, endp1, 'M');
		double undulation = scan_vector(pool, &cursor, endp1, 'M');
		double age = scan_scalar(pool, &cursor, endp1);
		unsigned long long stn_id = scan_natural(pool, &cursor, endp1);

		if (cursor >= endp1) {
			for (auto confirmation : this->confirmations) {
				if (confirmation->available()) {
					confirmation->on_GGA(timepoint, utc, latitude, longitude, quality,
						satellites, hdop, altitude, undulation, age, stn_id,
						logger);
				}
			}
		}
	}; break;
	case MESSAGE_TYPE('V', 'T', 'G'): {
		double tmg_true_deg = scan_vector(pool, &cursor, endp1, 'T');
		double tmg_magnetic_deg = scan_vector(pool, &cursor, endp1, 'M');
		double s_kn = scan_vector(pool, &cursor, endp1, 'N');
		double s_kmph = scan_vector(pool, &cursor, endp1, 'K');
		NMEA_PSMI mode = scan_positioning_system_mode_indicator(pool, &cursor, endp1);

		if (cursor >= endp1) {
			for (auto confirmation : this->confirmations) {
				if (confirmation->available()) {
					confirmation->on_VTG(timepoint, tmg_true_deg, tmg_magnetic_deg, s_kn, s_kmph, mode, logger);
				}
			}
		}
	}; break;
	case MESSAGE_TYPE('H', 'D', 'T'): {
		double heading_deg = scan_vector(pool, &cursor, endp1, 'T');

		if (cursor >= endp1) {
			for (auto confirmation : this->confirmations) {
				if (confirmation->available()) {
					confirmation->on_HDT(timepoint, heading_deg, this->logger);
				}
			}
		}
	}; break;
	case MESSAGE_TYPE('G', 'L', 'L'): {
		double latitude = scan_vector(pool, &cursor, endp1, 'N', 'S');
		double longitude = scan_vector(pool, &cursor, endp1, 'E', 'W');
		double utc = scan_scalar(pool, &cursor, endp1);
		bool validity = scan_boolean(pool, &cursor, endp1, 'A', 'V');
		NMEA_PSMI mode = scan_positioning_system_mode_indicator(pool, &cursor, endp1);
		
		if (cursor >= endp1) {
			for (auto confirmation : this->confirmations) {
				if (confirmation->available()) {
					confirmation->on_GLL(timepoint, utc, latitude, longitude, validity, mode, logger);
				}
			}
		}
	}; break;
	default: {
		task_discard(this->logger, L"unrecognized message[%c%c%c%c%c] coming from GPS[%s], ignored",
			pool[start - 5], pool[start - 4], pool[start - 3], pool[start - 2], pool[start - 1],
			this->device_description()->Data());

		cursor = endp1;
	}
	}

	if (cursor < endp1) {
		task_discard(this->logger, L"invalid %c%c%c%c%c message coming from GPS[%s], ignored",
			pool[start - 5], pool[start - 4], pool[start - 3], pool[start - 2], pool[start - 1],
			this->device_description()->Data());
	}
}

bool IGPS::connected() {
	return (this->gpsin != nullptr);
}

void IGPS::suicide() {
	if (this->socket != nullptr) {
		delete this->socket;

		if (this->gpsin != nullptr) {
			delete this->gpsin;

			this->gpsin = nullptr;
		}

		this->notify_connectivity_changed();
	}
}

void IGPS::clear() {
	if (this->gpsin != nullptr) {
		this->suicide();
	}
}
