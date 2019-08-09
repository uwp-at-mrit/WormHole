#pragma once

#include "gps/genums.hpp"

namespace WarGrey::SCADA {
#define GPS_GSA_PRN_COUNT 12
#define	GPS_GSV_CAPACITY 4

	private struct GPGGA {
		double utc;
		double latitude;
		double longitude;
		WarGrey::SCADA::NMEA_GQI quality;
		unsigned long long satellites;
		double hdop;
		double altitude;
		double undulation;
		double age;
		unsigned long long ref_station_id;
	};

	private struct GPVTG {
		double true_tmg_deg;
		double magnetic_tmg_deg;
		double s_kn;
		double s_kmph;
		WarGrey::SCADA::NMEA_PSMI mode;
	};

	private struct GPGLL {
		double utc;
		double latitude;
		double longitude;
		bool validity;
		WarGrey::SCADA::NMEA_PSMI mode;
	};

	private struct GPGSA {
		bool auto_selection;
		WarGrey::SCADA::NMEA_FIX_TYPE type;
		unsigned long long PRNs[GPS_GSA_PRN_COUNT];
		double pdop;
		double hdop;
		double vdop;
	};

	private struct GPGSV {
		unsigned long long total;
		unsigned long long sequence0;
		unsigned long long PRNs[GPS_GSV_CAPACITY];
		unsigned long long elevations[GPS_GSV_CAPACITY];
		unsigned long long azimuthes[GPS_GSV_CAPACITY];
		unsigned long long SNRs[GPS_GSV_CAPACITY];
	};

	private struct GPZDA {
		double utc;
		unsigned char day;
		unsigned char month;
		unsigned int year;
		unsigned char local_hour_offset;
		unsigned char local_minute_offset;
	};

	private struct HEHDT {
		double heading_deg;
	};

	/*************************************************************************************************/
	void scan_gpgga(GPGGA* gga, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_gpvtg(GPVTG* vtg, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_gpgll(GPGLL* gll, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_gpgsa(GPGSA* gsa, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_gpgsv(GPGSV* gsv, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_gpzda(GPZDA* gsv, const unsigned char* pool, size_t* cursor, size_t endp1);

	void scan_hehdt(HEHDT* hdt, const unsigned char* pool, size_t* cursor, size_t endp1);
}
