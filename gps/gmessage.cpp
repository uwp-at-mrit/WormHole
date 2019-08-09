#include "datum/bytes.hpp"
#include "datum/time.hpp"

#include "gps/gmessage.hpp"
#include "gps/gparser.hpp"

using namespace WarGrey::SCADA;

// DOP: dilution of precision
// PRN: pseudo random number
// SNR: signal noise ratio

void WarGrey::SCADA::scan_gpgga(GPGGA* gga, const unsigned char* pool, size_t* cursor, size_t endp1) {
	gga->utc = scan_scalar(pool, cursor, endp1);
	gga->latitude = scan_vector(pool, cursor, endp1, 'N', 'S');
	gga->longitude = scan_vector(pool, cursor, endp1, 'E', 'W');
	gga->quality = scan_gps_quality_indicator(pool, cursor, endp1);
	gga->satellites = scan_natural(pool, cursor, endp1);
	gga->hdop = scan_scalar(pool, cursor, endp1);
	gga->altitude = scan_vector(pool, cursor, endp1, 'M');
	gga->undulation = scan_vector(pool, cursor, endp1, 'M');
	gga->age = scan_scalar(pool, cursor, endp1);
	gga->ref_station_id = scan_natural(pool, cursor, endp1);
}

void WarGrey::SCADA::scan_gpvtg(GPVTG* vtg, const unsigned char* pool, size_t* cursor, size_t endp1) {
	vtg->true_tmg_deg = scan_vector(pool, cursor, endp1, 'T');
	vtg->magnetic_tmg_deg = scan_vector(pool, cursor, endp1, 'M');
	vtg->s_kn = scan_vector(pool, cursor, endp1, 'N');
	vtg->s_kmph = scan_vector(pool, cursor, endp1, 'K');
	vtg->mode = scan_positioning_system_mode_indicator(pool, cursor, endp1);
}

void WarGrey::SCADA::scan_gpgll(GPGLL* gll, const unsigned char* pool, size_t* cursor, size_t endp1) {
	gll->latitude = scan_vector(pool, cursor, endp1, 'N', 'S');
	gll->longitude = scan_vector(pool, cursor, endp1, 'E', 'W');
	gll->utc = scan_scalar(pool, cursor, endp1);
	gll->validity = scan_boolean(pool, cursor, endp1, 'A', 'V');
	gll->mode = scan_positioning_system_mode_indicator(pool, cursor, endp1);
}

void WarGrey::SCADA::scan_gpgsa(GPGSA* gsa, const unsigned char* pool, size_t* cursor, size_t endp1) {
    gsa->auto_selection = scan_boolean(pool, cursor, endp1, 'A', 'M');
	gsa->type = scan_gps_fix_type(pool, cursor, endp1);

	for (unsigned long long idx = 0; idx < GPS_GSA_PRN_COUNT; idx++) {
		gsa->PRNs[idx] = scan_natural(pool, cursor, endp1);
	}

	gsa->pdop = scan_scalar(pool, cursor, endp1);
	gsa->hdop = scan_scalar(pool, cursor, endp1);
	gsa->vdop = scan_scalar(pool, cursor, endp1);
}

void WarGrey::SCADA::scan_gpgsv(GPGSV* gsv, const unsigned char* pool, size_t* cursor, size_t endp1) {
	// NOTE: other messages may be inserted between multiple GSVs 
	unsigned long long total_message = scan_natural(pool, cursor, endp1);
	unsigned long long this_message = scan_natural(pool, cursor, endp1);
	
	gsv->total = scan_natural(pool, cursor, endp1);
	gsv->sequence0 = ((this_message - 1) * GPS_GSV_CAPACITY);

	for (unsigned char idx = 0; idx < GPS_GSV_CAPACITY; idx++) { // always four groups, so 'PRNs[idx] = 0' ==> absent
		gsv->PRNs[idx] = scan_natural(pool, cursor, endp1);
		gsv->elevations[idx] = scan_natural(pool, cursor, endp1);
		gsv->azimuthes[idx] = scan_natural(pool, cursor, endp1);
		gsv->SNRs[idx] = scan_natural(pool, cursor, endp1);
	}
}

void WarGrey::SCADA::scan_gpzda(GPZDA* zda, const unsigned char* pool, size_t* cursor, size_t endp1) {
	zda->utc = scan_scalar(pool, cursor, endp1);
	zda->day = scan_natural(pool, cursor, endp1);
	zda->month = scan_natural(pool, cursor, endp1);
	zda->year = scan_natural(pool, cursor, endp1);
	zda->local_hour_offset = scan_natural(pool, cursor, endp1);
	zda->local_minute_offset = scan_natural(pool, cursor, endp1);
}

/*************************************************************************************************/
void WarGrey::SCADA::scan_hehdt(HEHDT* hdt, const unsigned char* pool, size_t* cursor, size_t endp1) {
	hdt->heading_deg = scan_vector(pool, cursor, endp1, 'T');
}
