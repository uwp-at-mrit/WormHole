#include "ais/abitfields.hpp"

#include "datum/string.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::DTPM;

/*************************************************************************************************/
/* https://gpsd.gitlab.io/gpsd/AIVDM.html#_aivdmaivdo_payload_armoring */

std::basic_string<unsigned char> WarGrey::DTPM::ais_unarmor(std::string& payload) {
	const char* src = payload.c_str();
	size_t length = payload.size();
	std::basic_string<unsigned char> bitfields;

	for (size_t idx = 0; idx < length; idx++) {
		unsigned char ch = static_cast<unsigned char>(src[idx]) - 48;

		if (ch > 40) {
			ch -= 8;
		}

		bitfields.append(binumber(ch, 6));
	}

	return bitfields;
}

/*************************************************************************************************/
