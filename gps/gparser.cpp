#include "datum/bytes.hpp"
#include "datum/string.hpp"
#include "datum/time.hpp"

#include "gps/gparser.hpp"

using namespace WarGrey::SCADA;

/*************************************************************************************************/
unsigned int WarGrey::SCADA::message_type(const unsigned char* pool, size_t index) {
	return MESSAGE_TYPE(pool[index + 0], pool[index + 1], pool[index + 2]);
}

double WarGrey::SCADA::scan_dimension(const unsigned char* pool, size_t* idx, size_t endp1, unsigned char unit) {
	double flnum = scan_flonum(pool, idx, endp1, false);
	size_t end_idx = (*idx) + 2;

	if ((pool[(*idx)] == ',') && (pool[(*idx) + 1] == unit)) {
		if (pool[end_idx] == ',') {
			(*idx) += 3;
		} else if (end_idx == endp1) {
			(*idx) += 2;
		}
	}

	return flnum;
}

NMEA_PSMI WarGrey::SCADA::scan_positioning_system_mode_indicator(const unsigned char* pool, size_t* idx) {
	NMEA_PSMI indicator = NMEA_PSMI::_;

	switch (pool[(*idx)]) {
	case 'A': indicator = NMEA_PSMI::Autonomouse; break;
	case 'D': indicator = NMEA_PSMI::Differential; break;
	case 'E': indicator = NMEA_PSMI::Estimated; break;
	case 'M': indicator = NMEA_PSMI::Manual; break;
	case 'N': indicator = NMEA_PSMI::NotValid; break;
	}

	if (indicator != NMEA_PSMI::_) {
		(*idx) += 1;
	}

	return indicator;
}
