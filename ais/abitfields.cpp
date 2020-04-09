#include "ais/abitfields.hpp"

#include "datum/string.hpp"

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

unsigned long long WarGrey::DTPM::ais_bit_field(Natural& payload, size_t idx, size_t length) {
	size_t isize = payload.integer_length(6);
	size_t nend = isize - idx;
	size_t nstart = nend - length;

	return payload.bitfield(nstart, nend);
}

b WarGrey::DTPM::ais_b_ref(Natural& payload, size_t idx) {
	size_t isize = payload.integer_length(6);

	return payload.is_bit_set(isize - idx);
}

t WarGrey::DTPM::ais_t_ref(WarGrey::GYDM::Natural& payload, size_t idx, size_t length) {
	size_t isize = payload.integer_length(6);
	size_t nend = isize - idx;
	size_t nstart = nend - length;

	return payload.bit_field(nstart, nend);
}

u WarGrey::DTPM::ais_u_ref(Natural& payload, size_t idx, size_t length) {
	return static_cast<unsigned int>(ais_bit_field(payload, idx, length) & 0xFFFFFFFF);
}

i WarGrey::DTPM::ais_i_ref(Natural& payload, size_t idx, size_t length) {
	return static_cast<signed int>(ais_u_ref(payload, idx, length));
}
