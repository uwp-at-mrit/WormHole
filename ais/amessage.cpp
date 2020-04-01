#include "ais/amessage.hpp"
#include "gps/gparser.hpp"

using namespace WarGrey::DTPM;

void WarGrey::DTPM::scan_vdm(VDM* gga, const unsigned char* pool, size_t* cursor, size_t endp1) {
	gga->utc = scan_scalar(pool, cursor, endp1);
	//gga->latitude = scan_vector(pool, cursor, endp1, 'N', 'S');
	//gga->longitude = scan_vector(pool, cursor, endp1, 'E', 'W');
	//gga->quality = scan_gps_quality_indicator(pool, cursor, endp1);
	//gga->satellites = scan_natural(pool, cursor, endp1);
	//gga->hdop = scan_scalar(pool, cursor, endp1);
	//gga->altitude = scan_vector(pool, cursor, endp1, 'M');
	//gga->undulation = scan_vector(pool, cursor, endp1, 'M');
	//gga->age = scan_scalar(pool, cursor, endp1);
	//gga->ref_station_id = scan_natural(pool, cursor, endp1);
}
