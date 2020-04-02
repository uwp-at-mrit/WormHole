#include "ais/transponder.hpp"
#include "ais/amessage.hpp"

#include "gps/gparser.hpp"

#include "datum/bytes.hpp"
#include "datum/time.hpp"
#include "datum/string.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::DTPM;
using namespace WarGrey::GYDM;

/*************************************************************************************************/
#define AIS_SENTENCE_POOL_SIZE 9

struct WarGrey::DTPM::Transponder::Sentences {
	std::string vdm[AIS_SENTENCE_POOL_SIZE];
	std::string vdo[AIS_SENTENCE_POOL_SIZE];
};

/*************************************************************************************************/
void Transponder::on_message(int id, long long timepoint, const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1, Syslog* logger) {
	unsigned int type = message_type(pool, head_start + 2);
	size_t cursor = body_start;
	Sentences* sentences = &(this->sentences[id]);
	std::string* sentence = nullptr;
	AINMEA ai_nmea;

	switch (type) {
	case MESSAGE_TYPE('V', 'D', 'M'): {
		sentence = sentences->vdm;
		scan_nmea(&ai_nmea, pool, &cursor, endp1);
	}; break;
	case MESSAGE_TYPE('V', 'D', 'O'): {
		sentence = sentences->vdo;
		scan_nmea(&ai_nmea, pool, &cursor, endp1);
	}; break;
	
	default: {
		logger->log_message(Log::Debug, L"unrecognized message[%S:%S], ignored", pool + head_start, pool + body_start);

		cursor = endp1;
	}
	}

	if (cursor < endp1) {
		logger->log_message(Log::Error, L"invalid %S message: %S, ignored", pool + head_start, pool + body_start);
	}
}
