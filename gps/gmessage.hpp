#pragma once

#include "gps/genums.hpp"

// https://docs.novatel.com/oem7/Content/Logs/Core_Logs.htm?tocpath=Logs%7CAll%20Logs%7CGNSS%20Logs%7C_____0

namespace WarGrey::SCADA {
#define GPS_GSA_PRN_COUNT 12
#define	GPS_GSV_CAPACITY 4

	private struct GGA {
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

	private struct VTG {
		double track_deg;
		double track_magnetic_deg;
		double s_kn;
		double s_kmph;
		WarGrey::SCADA::NMEA_PSMI mode;
	};

	private struct GLL {
		double utc;
		double latitude;
		double longitude;
		bool validity;
		WarGrey::SCADA::NMEA_PSMI mode;
	};

	private struct GSA {
		bool auto_selection;
		WarGrey::SCADA::NMEA_FIX_TYPE type;
		unsigned long long PRNs[GPS_GSA_PRN_COUNT];
		double pdop;
		double hdop;
		double vdop;
	};

	private struct GSV {
		unsigned long long total;
		unsigned long long sequence0;
		unsigned long long PRNs[GPS_GSV_CAPACITY];
		unsigned long long elevations[GPS_GSV_CAPACITY];
		unsigned long long azimuthes[GPS_GSV_CAPACITY];
		unsigned long long SNRs[GPS_GSV_CAPACITY];
	};

	private struct ZDA {
		double utc;
		unsigned long long day;
		unsigned long long month;
		unsigned long long year;
		unsigned long long local_hour_offset;
		unsigned long long local_minute_offset;
	};

	private struct HDT {
		double heading_deg;
	};

	private struct ROT {
		double degpmin;
		bool validity;
	};

	/*************************************************************************************************/
	void scan_gga(GGA* gga, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_vtg(VTG* vtg, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_gll(GLL* gll, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_gsa(GSA* gsa, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_gsv(GSV* gsv, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_zda(ZDA* zda, const unsigned char* pool, size_t* cursor, size_t endp1);

	void scan_hdt(HDT* hdt, const unsigned char* pool, size_t* cursor, size_t endp1);
	void scan_rot(ROT* rot, const unsigned char* pool, size_t* cursor, size_t endp1);
}
