#include "ais/transponder.hpp"
#include "ais/abitfields.hpp"

#include "gps/gparser.hpp"

/* https://gpsd.gitlab.io/gpsd/AIVDM.html */

using namespace WarGrey::DTPM;
using namespace WarGrey::GYDM;

/*************************************************************************************************/
namespace {
	private struct AIMSG {
		std::map<int, AINMEA> body;
	};
}

struct WarGrey::DTPM::Transponder::Sentences {
	AIMSG vdm;
	AIMSG vdo;
};

/*************************************************************************************************/
void Transponder::on_message(int id, long long timepoint, const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1, Syslog* logger) {
	unsigned int type = message_type(pool, head_start + 2);
	size_t cursor = body_start;
	AIMSG* ai_msg = nullptr;
	bool self = true;
	AINMEA ai_nmea;

	if (this->sentences.find(id) == this->sentences.end()) {
		this->sentences[id] = new Transponder::Sentences();
	}

	switch (type) {
	case MESSAGE_TYPE('V', 'D', 'M'): {
		self = false;
		ai_msg = &(this->sentences[id]->vdm);
		scan_nmea(&ai_nmea, pool, &cursor, endp1);
	}; break;
	case MESSAGE_TYPE('V', 'D', 'O'): {
		ai_msg = &(this->sentences[id]->vdo);
		scan_nmea(&ai_nmea, pool, &cursor, endp1);
	}; break;
	
	default: {
		logger->log_message(Log::Debug, L"unrecognized message[%S:%S], ignored", pool + head_start, pool + body_start);

		cursor = endp1;
	}
	}

	if (cursor < endp1) {
		logger->log_message(Log::Error, L"invalid %S message: %S, ignored", pool + head_start, pool + body_start);
	} else if (ai_msg != nullptr) {
		if (ai_nmea.s_size == 1) {
			this->on_message(id, timepoint, self, &ai_nmea, logger);
		} else if (ai_nmea.s_idx == 1) {
			ai_msg->body[ai_nmea.msg_id] = ai_nmea;
		} else {
			AINMEA* prev_fragment = &(ai_msg->body[ai_nmea.msg_id]);

			if ((prev_fragment->s_size == ai_nmea.s_size) && ((prev_fragment->s_idx + 1) == ai_nmea.s_idx)) {
				ai_nmea.payload = prev_fragment->payload.append(ai_nmea.payload);

				if (ai_nmea.s_idx < ai_nmea.s_size) {
					ai_msg->body[ai_nmea.msg_id] = ai_nmea;
				} else {
					this->on_message(id, timepoint, self, &ai_nmea, logger);
					ai_msg->body.erase(ai_nmea.msg_id);
				}
			} else if (ai_nmea.s_idx == ai_nmea.s_size) {
				logger->log_message(Log::Error, L"fragmented message %d: %S; ignored", prev_fragment->msg_id, prev_fragment->payload.c_str());
			}
		}
	}
}

void Transponder::on_message(int id, long long timepoint_ms, bool self, AINMEA* ai_nmea, Syslog* logger) {
	std::basic_string<unsigned char> bitfields = ais_unarmor(ai_nmea->payload);

	logger->log_message(Log::Info, L"%d: %S ==> %S", ai_nmea->msg_id, ai_nmea->payload.c_str(), bitfields.c_str());
}
