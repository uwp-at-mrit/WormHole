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
	return _E(AISMessage, ais_uint32_field(payload, 0, 6) - 1);
}

void WarGrey::DTPM::ais_extract_prca(WarGrey::DTPM::PRCA* prca, WarGrey::GYDM::Natural& payload) {
	prca->repeat = ais_uint32_field(payload, 6, 2);
	prca->mmsi = ais_uint32_field(payload, 8, 30);
	prca->status = _E(AISNavigation, ais_uint32_field(payload, 38, 4));
	prca->turn.box(ais_uint32_field(payload, 42, 8));
	prca->speed.box(ais_uint32_field(payload, 50, 10));
	prca->accuracy = ais_is_bit_set(payload, 60);
	prca->longitude.box(ais_uint32_field(payload, 61, 28));
	prca->latitude.box(ais_uint32_field(payload, 89, 27));
	prca->course.box(ais_uint32_field(payload, 116, 12));
	prca->heading = ais_uint32_field(payload, 128, 9);
	prca->timetamp = ais_uint32_field(payload, 137, 6);
	prca->maneuver = _E(AISManeuver, ais_uint32_field(payload, 143, 2));
	prca->raim = ais_is_bit_set(payload, 144);
	prca->radio = ais_uint32_field(payload, 149, 19);
}
