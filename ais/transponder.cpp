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
void Transponder::on_message(int id, long long timepoint, const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1, Syslog* logger) {
	unsigned int type = message_type(pool, head_start + 2);
	size_t cursor = body_start;

#define ON_MESSAGE(MSG, scan, pool, cursor, endp1, id, logger, timepoint) \
{ \
	MSG msg; \
	scan(&msg, pool, &cursor, endp1); \
	if (cursor >= endp1) { \
		this->pre_scan_data(id, logger); \
        this->on_##MSG(id, timepoint, &msg, logger); \
        this->post_scan_data(id, logger); \
	} \
}

	switch (type) {
	case MESSAGE_TYPE('V', 'D', 'M'): ON_MESSAGE(VDM, scan_vdm, pool, cursor, endp1, id, logger, timepoint); break;
	case MESSAGE_TYPE('V', 'D', 'O'): ON_MESSAGE(VDO, scan_vdo, pool, cursor, endp1, id, logger, timepoint); break;
	
	default: {
		logger->log_message(Log::Debug, L"unrecognized message[%S:%S], ignored", pool + head_start, pool + body_start);

		cursor = endp1;
	}
	}

	if (cursor < endp1) {
		logger->log_message(Log::Error, L"invalid %S message: %S, ignored", pool + head_start, pool + body_start);
	}
}
