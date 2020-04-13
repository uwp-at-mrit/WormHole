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

	return payload.is_bit_set(isize - idx - 1);
}

t WarGrey::DTPM::ais_t_ref(WarGrey::GYDM::Natural& payload, size_t idx, size_t length) {
	size_t t_size = length / 6;
	std::string t;
	size_t space = 0;

	for (size_t t_idx = 0; t_idx < t_size; t_idx++) {
		unsigned long long ais_ch = ais_bit_field(payload, idx + t_idx * 6, 6) & 0b111111;
		
		if (ais_ch > 32) { // (' ', '?']
			if (space > 0) {
				t.append(space, ' ');
				space = 0;
			}

			t.push_back(char(ais_ch));
		} else if (ais_ch == 32) {
			space += 1;
		} else if (ais_ch > 0) { // ('@', '_']
			if (space > 0) {
				t.append(space, ' ');
				space = 0;
			}
			
			t.push_back(char(ais_ch) + '@');
		} else { // the '@' that followed by garbage
			break;
		}
	}
	
	return t;
}

u WarGrey::DTPM::ais_u_ref(Natural& payload, size_t idx, size_t length) {
	return static_cast<unsigned int>(ais_bit_field(payload, idx, length));
}

i WarGrey::DTPM::ais_i_ref(Natural& payload, size_t idx, size_t length) {
	size_t isize = payload.integer_length(6);
	size_t nend = isize - idx;
	size_t nstart = nend - length;
	
	return static_cast<int>(payload.signed_bitfield(nstart, nend));
}
