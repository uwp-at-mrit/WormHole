#pragma once

#include "datum/flonum.hpp"

namespace WarGrey::SCADA {
	private struct GPSInfo {
		int cs_mode = 0;

		long double cs_a = 0.0L;
		long double cs_f = 0.0L;
		long double cs_a84 = 0.0L;
		long double cs_f84 = 0.0L;
		long double cs_a54 = 0.0L;
		long double cs_f54 = 0.0L;
		long double cs_cm = 0.0L; // Center Meridian

		long double cs_s = 1.0L;
		long double cs_tx = 0.0L;
		long double cs_ty = 0.0L;
		long double cs_tz = 0.0L;
		long double cs_rx = 0.0L;
		long double cs_ry = 0.0L;
		long double cs_rz = 0.0L;
		long double cs_dx = 0.0L;
		long double cs_dy = 0.0L;
		long double cs_dz = 0.0L;

		long double utm_s = 1.0L;
	};

	WarGrey::SCADA::double2 WGS84BLH_to_XY(double x0, double y0, double z0, WarGrey::SCADA::GPSInfo& info);

	WarGrey::SCADA::double3 WGS84BLH_to_XYZ(double b, double l, double h, WarGrey::SCADA::GPSInfo& info);
	WarGrey::SCADA::double3 XYZ_to_BEJ54XYZ(double x, double y, double z, WarGrey::SCADA::GPSInfo& info);
	WarGrey::SCADA::double3 BEJ54XYZ_to_BEJ54BLH(double x, double y, double z, WarGrey::SCADA::GPSInfo& info);
	WarGrey::SCADA::double3 BEJ54BLH_to_GAUSSXYH(double b, double l, double h, WarGrey::SCADA::GPSInfo& info);
}
