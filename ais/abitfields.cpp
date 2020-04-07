#include "ais/abitfields.hpp"

#include "datum/string.hpp"
#include "datum/natural.hpp"

using namespace WarGrey::SCADA;
using namespace WarGrey::DTPM;
using namespace WarGrey::GYDM;

/*************************************************************************************************/
/* https://gpsd.gitlab.io/gpsd/AIVDM.html#_aivdmaivdo_payload_armoring */

WarGrey::GYDM::Natural WarGrey::DTPM::ais_unarmor(std::string& payload, int pad_bits) {
	const char* src = payload.c_str();
	size_t length = payload.size();
	Natural bitfields(0);

	bitfields.expand(length);

	for (size_t idx = 0; idx < length; idx++) {
		unsigned char ch = static_cast<unsigned char>(src[idx]) - 48;

		if (ch > 40) {
			ch -= 8;
		}

		bitfields <<= 6;
		bitfields |= ch;
	}

	{ // padding
		int remainder = (length * 6 + pad_bits) % 6;

		if (remainder != 0) {
			pad_bits += (6 - remainder);
		}

		if (pad_bits > 0) {
			bitfields <<= pad_bits;
		}
	}

	return bitfields;
}

/*************************************************************************************************/
