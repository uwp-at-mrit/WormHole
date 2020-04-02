#pragma once

#include <string>

#include "gps/genums.hpp"

// https://en.wikipedia.org/wiki/Automatic_identification_system#Messages_sent_to_other_equipment_in_the_ship

namespace WarGrey::DTPM {
	private struct AINMEA {
		int s_size;
		int s_idx;
		int msg_id;
		unsigned char channel;
		std::string fragment;
		int fill_bits_number;
	};

	/*************************************************************************************************/
	void scan_nmea(WarGrey::DTPM::AINMEA* nmea, const unsigned char* pool, size_t* cursor, size_t endp1);
}
