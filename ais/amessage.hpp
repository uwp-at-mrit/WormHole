#pragma once

#include <string>

#include "gps/genums.hpp"

// https://en.wikipedia.org/wiki/Automatic_identification_system#Messages_sent_to_other_equipment_in_the_ship

namespace WarGrey::DTPM {
	private struct AINMEA {
		int s_size = 0;
		int s_idx = 0;
		int msg_id = 0;
		unsigned char channel;
		std::string payload;
		int pad_bits;
	};

	/*************************************************************************************************/
	void scan_nmea(WarGrey::DTPM::AINMEA* nmea, const unsigned char* pool, size_t* cursor, size_t endp1);
}
