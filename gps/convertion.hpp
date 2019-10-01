#pragma once

#include "datum/flonum.hpp"

namespace WarGrey::SCADA {
	private struct GPSInfo {
		long double a = 0.0L; // Ellipsoid Radius
		long double f = 0.0L; // Ellipsoid Flatness
		long double cm = 0.0L; // Center Meridian

		// cartography transformation and convertion
		long double cs_s = 1.0L;
		long double cs_tx = 0.0L;
		long double cs_ty = 0.0L;
		long double cs_tz = 0.0L;
		long double cs_rx = 0.0L;
		long double cs_ry = 0.0L;
		long double cs_rz = 0.0L;

		// gauss projection
		long double gp_dx = 0.0L;
		long double gp_dy = 0.0L;
		long double gp_dz = 0.0L;
		long double utm_s = 1.0L;
	};

	double gps_degmm_to_degrees(double DDmm_mm);
	double gps_degmm_to_radians(double DDmm_mm);

	WarGrey::SCADA::double2 gps_to_XY(double latitude, double longitude, double altitude, WarGrey::SCADA::GPSInfo& info);

	// ECEF: Earth Centered Earth Fixed
	WarGrey::SCADA::double3 WGS84BLH_to_ECEFXYZ(double latitude, double longitude, double altitude, WarGrey::SCADA::GPSInfo& info);
	WarGrey::SCADA::double3 ECEFXYZ_to_BEJ54XYZ(double x, double y, double z, WarGrey::SCADA::GPSInfo& info);
	WarGrey::SCADA::double3 BEJ54XYZ_to_BEJ54BLH(double x, double y, double z, WarGrey::SCADA::GPSInfo& info);
	WarGrey::SCADA::double3 BEJ54BLH_to_GAUSSXYH(double b, double l, double h, WarGrey::SCADA::GPSInfo& info);
}
