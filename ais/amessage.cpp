#include "ais/amessage.hpp"
#include "ais/abitfields.hpp"

#include "gps/gparser.hpp"

#include "datum/enum.hpp"

using namespace WarGrey::DTPM;
using namespace WarGrey::GYDM;

/*************************************************************************************************/
void WarGrey::DTPM::scan_ainmea(AINMEA* nmea, const unsigned char* pool, size_t* cursor, size_t endp1) {
	nmea->s_size = int(scan_natural(pool, cursor, endp1));
	nmea->s_idx = int(scan_natural(pool, cursor, endp1));
	nmea->msg_id = int(scan_natural(pool, cursor, endp1));
	nmea->channel = scan_char(pool, cursor, endp1, '\0');
	nmea->payload = scan_text(pool, cursor, endp1);
	nmea->pad_bits = int(scan_natural(pool, cursor, endp1));
}

/*************************************************************************************************/
AISMessage WarGrey::DTPM::ais_message_type(Natural& payload) {
	Natural bitfields = ais_bit_field(payload, 0, 6);

	return _E(AISMessage, bitfields[0] - 1);
}

void WarGrey::DTPM::ais_extract_prca(WarGrey::DTPM::PRCA* prca, WarGrey::GYDM::Natural& payload) {

}
