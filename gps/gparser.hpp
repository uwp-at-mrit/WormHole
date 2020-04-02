#pragma once

#include <string>

#include "gps/genums.hpp"

namespace WarGrey::DTPM {
#define MESSAGE_TYPE(ch1, ch2, ch3) ((ch1 << 16) | (ch2 << 8) | ch3)

	unsigned int message_type(const unsigned char* pool, size_t index);

	bool scan_boolean(const unsigned char* pool, size_t* idx, size_t endp1, unsigned char T, unsigned char F);
	unsigned char scan_char(const unsigned char* pool, size_t* idx, size_t endp1);
	std::string scan_text(const unsigned char* pool, size_t* idx, size_t endp1);
	unsigned long long scan_natural(const unsigned char* pool, size_t* idx, size_t endp1);
	double scan_scalar(const unsigned char* pool, size_t* idx, size_t endp1);
	double scan_vector(const unsigned char* pool, size_t* idx, size_t endp1, unsigned char unit);
	double scan_vector(const unsigned char* pool, size_t* idx, size_t endp1, unsigned char positive_dir, unsigned char negative_dir);

	WarGrey::DTPM::NMEA_PSMI scan_positioning_system_mode_indicator(const unsigned char* pool, size_t* idx, size_t endp1);
	WarGrey::DTPM::NMEA_GQI scan_gps_quality_indicator(const unsigned char* pool, size_t* idx, size_t endp1);
	WarGrey::DTPM::NMEA_FIX_TYPE scan_gps_fix_type(const unsigned char* pool, size_t* idx, size_t endp1);
}
