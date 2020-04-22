#include "ais/transceiver.hpp"
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

struct WarGrey::DTPM::Transceiver::Sentences {
	AIMSG vdm;
	AIMSG vdo;
};

/*************************************************************************************************/
void Transceiver::on_message(int id, long long timepoint, const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1, Syslog* logger) {
	unsigned int type = message_type(pool, head_start + 2);
	size_t cursor = body_start;
	AIMSG* ai_msg = nullptr;
	bool self = true;
	AINMEA ai_nmea;

	if (this->sentences.find(id) == this->sentences.end()) {
		this->sentences[id] = new Transceiver::Sentences();
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
		} else { // TODO: Message 24 may interleaved
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

void Transceiver::on_payload(int id, long long timepoint_ms, bool self, std::string& payload, int pad_bits, Syslog* logger) {
	Natural bitfields = ais_unarmor(payload, pad_bits);
	AISMessage type = ais_message_type(bitfields);
	uint16 mmsi = ais_mobile_marine_service_identifier(bitfields);

#define ON_MESSAGE(MSG, payload, id, self, mmsi, priority, logger, timepoint) \
{ \
	MSG msg(payload); \
	this->pre_interpret_payload(id, logger); \
    this->on_##MSG(id, timepoint, self, mmsi, &msg, priority, logger); \
    this->post_interpret_payload(id, logger); \
}

	switch (type) {
	case AISMessage::PositionReportASO:
	case AISMessage::PositionReportASO_AssignedSchedule:
	case AISMessage::PositionReportASO_Response2Interrogation: ON_MESSAGE(ASO, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;

	case AISMessage::BaseStationReport: ON_MESSAGE(BSR, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	case AISMessage::StaticVoyageData: ON_MESSAGE(SVD, bitfields, id, self, mmsi, 4U, logger, timepoint_ms); break;
	
	case AISMessage::BinaryUnicast: ON_MESSAGE(BAM, bitfields, id, self, mmsi, 4U, logger, timepoint_ms); break;
	case AISMessage::BinaryAcknowledge: ON_MESSAGE(BA, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	case AISMessage::BinaryBroadcast: ON_MESSAGE(BBM, bitfields, id, self, mmsi, 4U, logger, timepoint_ms); break;

	case AISMessage::UTCInquiry: ON_MESSAGE(UTCI, bitfields, id, self, mmsi, 3U, logger, timepoint_ms); break;
	case AISMessage::UTCResponse: ON_MESSAGE(BSR, bitfields, id, self, mmsi, 3U, logger, timepoint_ms); break;
	case AISMessage::StdSARAircraftPositionReport: ON_MESSAGE(SRAPR, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	case AISMessage::ATONReport: ON_MESSAGE(ATON, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;

	case AISMessage::SaftyUnicast: ON_MESSAGE(ASRM, bitfields, id, self, mmsi, 2U, logger, timepoint_ms); break;
	case AISMessage::SaftyAcknowledge: ON_MESSAGE(BA, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	case AISMessage::SaftyBroadcast: ON_MESSAGE(SRBM, bitfields, id, self, mmsi, 2U, logger, timepoint_ms); break;

	case AISMessage::Interrogation: ON_MESSAGE(BSIQ, bitfields, id, self, mmsi, 3U, logger, timepoint_ms); break;
	case AISMessage::AssignmentModeCommand: ON_MESSAGE(AMC, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	case AISMessage::DGNSSBinaryBroadcast: ON_MESSAGE(GBBM, bitfields, id, self, mmsi, 2U, logger, timepoint_ms); break;

	case AISMessage::PositionReportBCS: ON_MESSAGE(BCS, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	case AISMessage::PositionReportBCS_Extended: ON_MESSAGE(BCSE, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	
	case AISMessage::DataLinkManagement: ON_MESSAGE(DLM, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	case AISMessage::ChannelManagement: ON_MESSAGE(CM, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	case AISMessage::GroupAssignmentCommand: ON_MESSAGE(GAC, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;
	
	case AISMessage::StaticDataReport: ON_MESSAGE(SDR, bitfields, id, self, mmsi, 4U, logger, timepoint_ms); break;
	case AISMessage::SingleSlotBinary: ON_MESSAGE(SSBM, bitfields, id, self, mmsi, 4U, logger, timepoint_ms); break;
	case AISMessage::MultipleSlotBinary: ON_MESSAGE(MSBM, bitfields, id, self, mmsi, 4U, logger, timepoint_ms); break;
	case AISMessage::PositionReportLongRangeBroadcast: ON_MESSAGE(LRB, bitfields, id, self, mmsi, 1U, logger, timepoint_ms); break;

	default: {
		// Message 28-63 are reserved for future use
		logger->log_message(Log::Debug, L"unrecognized message %s[%d], ignored",
			type.ToString()->Data(), ais_u_ref(bitfields, 0, 6));
	}
	}
}
