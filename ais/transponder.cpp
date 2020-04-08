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
		scan_ainmea(&ai_nmea, pool, &cursor, endp1);
	}; break;
	case MESSAGE_TYPE('V', 'D', 'O'): {
		ai_msg = &(this->sentences[id]->vdo);
		scan_ainmea(&ai_nmea, pool, &cursor, endp1);
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
			this->on_payload(id, timepoint, self, ai_nmea.payload, ai_nmea.pad_bits, logger);
		} else if (ai_nmea.s_idx == 1) {
			ai_msg->body[ai_nmea.msg_id] = ai_nmea;
		} else {
			AINMEA* prev_fragment = &(ai_msg->body[ai_nmea.msg_id]);

			if ((prev_fragment->s_size == ai_nmea.s_size) && ((prev_fragment->s_idx + 1) == ai_nmea.s_idx)) {
				ai_nmea.payload = prev_fragment->payload.append(ai_nmea.payload);

				if (ai_nmea.s_idx < ai_nmea.s_size) {
					ai_msg->body[ai_nmea.msg_id] = ai_nmea;
				} else {
					this->on_payload(id, timepoint, self, ai_nmea.payload, ai_nmea.pad_bits, logger);
					ai_msg->body.erase(ai_nmea.msg_id);
				}
			} else if (ai_nmea.s_idx == ai_nmea.s_size) {
				logger->log_message(Log::Error, L"fragmented message %d: %S; ignored", prev_fragment->msg_id, prev_fragment->payload.c_str());
			}
		}
	}
}

void Transponder::on_payload(int id, long long timepoint_ms, bool self, std::string& payload, int pad_bits, Syslog* logger) {
	Natural bitfields = ais_unarmor(payload, pad_bits);
	AISMessage type = ais_message_type(bitfields);

#define ON_MESSAGE(MSG, extract, payload, id, self, logger, timepoint) \
{ \
	MSG msg; \
	extract(&msg, payload); \
	this->pre_interpret_payload(id, logger); \
    this->on_##MSG(id, timepoint, self, &msg, logger); \
    this->pre_interpret_payload(id, logger); \
}

	switch (type) {
	//case AISMessage::PositionReportClassA:
	//case AISMessage::PositionReportClassA_AssignedSchedule:
	//case AISMessage::PositionReportClassA_Response2Interrogation: ON_MESSAGE(PRCA, ais_extract_prca, bitfields, id, self, logger, timepoint_ms); break;

	default: {
		logger->log_message(Log::Debug, L"unrecognized message %s, ignored", type.ToString()->Data());
	}
	}
}
