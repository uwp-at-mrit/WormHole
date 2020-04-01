#pragma once

#include "gps/genums.hpp"

// https://en.wikipedia.org/wiki/Automatic_identification_system#Messages_sent_to_other_equipment_in_the_ship

namespace WarGrey::DTPM {
	private struct VDM {
		double utc;
	};

	private struct VDO {
		double utc;
	};

	/*************************************************************************************************/
	void scan_vdm(WarGrey::DTPM::VDM* vdm, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_vdo(WarGrey::DTPM::VDO* vdo, const unsigned char* pool, size_t* cursor, size_t endp1);
}
