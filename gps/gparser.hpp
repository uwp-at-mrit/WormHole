#pragma once

#include "gps/gps_enums.hpp"

namespace WarGrey::SCADA {
#define MESSAGE_TYPE(ch1, ch2, ch3) ((ch1 << 16) | (ch2 << 8) | ch3)

	unsigned int message_type(const unsigned char* pool, size_t index);

	double scan_dimension(const unsigned char* pool, size_t* idx, size_t endp1, unsigned char unit);
	NMEA_PSMI scan_positioning_system_mode_indicator(const unsigned char* pool, size_t* idx);
}
