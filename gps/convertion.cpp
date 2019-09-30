#include "gps/convertion.hpp"

#include "datum/time.hpp"

using namespace WarGrey::SCADA;

double2 WarGrey::SCADA::WGS84BLH_to_XY(double b, double l, double h, GPSInfo& info) {
	double3 space = WGS84BLH_to_XYZ(b, l, h, info);
	double3 bj54x = XYZ_to_BEJ54XYZ(space.x, space.y, space.y, info);
	double3 bj54b = BEJ54XYZ_to_BEJ54BLH(bj54x.x, bj54x.y, bj54x.y, info);
	double3 gauss = BEJ54BLH_to_GAUSSXYH(bj54b.x, bj54b.y, bj54b.z, info);

	// TODO: local rotation

	return double2(gauss.x, gauss.y);
}

/*************************************************************************************************/
double3 WarGrey::SCADA::WGS84BLH_to_XYZ(double B,double L,double H, GPSInfo& info) {
	double e = 0.00669437999013;
	double N = 6378137.0 / flsqrt(1.0 - e * flsin(B) * flsin(B));
	double X = (N + H) * flcos(B) * flcos(L);
	double Y = (N + H) * flcos(B) * flsin(L);
	double Z = (N * (1.0 - e) + H) * flsin(B);

	return double3(X, Y, Z);
}

double3 WarGrey::SCADA::XYZ_to_BEJ54XYZ(double X, double Y, double Z, GPSInfo& info) {
	double e = 0.0000048481368;
	double bj54_rx = e * info.cs_rx;
	double bj54_ry = e * info.cs_ry;
	double bj54_rz = e * info.cs_rz;
	double x = double(info.cs_tx + info.cs_s * X - bj54_rz * Y + bj54_ry * Z);
	double y = double(info.cs_ty + info.cs_s * Y + bj54_rz * X - bj54_rx * Z);
	double z = double(info.cs_tz + info.cs_s * Z - bj54_ry * X + bj54_rx * Y);

	return double3(x, y, z);
}

double3 WarGrey::SCADA::BEJ54XYZ_to_BEJ54BLH(double x, double y, double z, GPSInfo& info) {
	double b = info.cs_a - 1.0 / info.cs_f * info.cs_a;
	double a = info.cs_a;
	double e = (a * a - b * b) / (a * a);
	double a54 = info.cs_a;
	double R = flsqrt(x * x + y * y);
	double B0 = flatan(z, R);
	double L = flatan(y, x);
	double B, N;
	
	long long future = current_milliseconds() + 1000;
	do {
		N = a54 / flsqrt(1.0 - e * flsin(B0) * flsin(B0));
		B = flatan(z + N * e * flsin(B0), R);
		
		if (flabs(B - B0) < 1.0e-10) {
			break;
		} else {
			B0 = B;
		}
	} while(current_milliseconds() < future);

	return double3(B, L, R / flcos(B)-N);
}

double3 WarGrey::SCADA::BEJ54BLH_to_GAUSSXYH(double B, double L, double H, WarGrey::SCADA::GPSInfo& info) {
	double cs_a54 = info.cs_a;
	double a = info.cs_a;
	double b = info.cs_a - 1.0 / info.cs_f * info.cs_a;
	double e = (a * a - b * b) / (a * a);
	
	double x2 = e;
	double x4 = x2 * x2;
	double x6 = x4 * x2;
	double x8 = x4 * x4;
	double x10 = x2 * x8;

	double a0 = 1.0 + 3.0 * x2 / 4.0 + 45.0 * x4 / 64.0 + 175.0 * x6 / 256.0 + 11025.0 * x8 / 16384.0 + 43659.0 * x10 / 65536.0;
	double b0 = 3.0 * x2 / 4.0 + 15.0 * x4 / 16.0 + 525.0 * x6 / 512.0 + 2205.0 * x8 / 2048.0 + 72765.0 * x10 / 65536.0;
	double c0 = 15.0 * x4 / 64.0 + 105.0 * x6 / 256.0 + 2205.0 * x8 / 4096.0 + 10395.0 * x10 / 16384.0;
	double d0 = 35.0 * x6 / 512.0 + 315.0 * x8 / 2048.0 + 31185.0 * x10 / 13072.0;

	double a1 = a0 * info.cs_a * (1.0 - e);
	double a2 = -b0 * info.cs_a * (1.0 - e) / 2.0;
	double a3 = c0 * info.cs_a * (1.0 - e) / 4.0;
	double a4 = -d0 * info.cs_a * (1.0 - e) / 6.0;

	double r0 = a1;
	double r1 = 2.0 * a2 + 4.0 * a3 + 6.0 * a4;
	double r2 = -8.0 * a3 - 32.0 * a4;
	double r3 = 32.0 * a4;

	double Tb = fltan(B);
	double Y2 = x2 * flcos(B) * flcos(B) / (1.0 - x2);
	double N = cs_a54 / flsqrt(1.0 - e * flsin(B) * flsin(B));
	
	double M = flcos(B) * (L - info.cs_cm * pi / 180.0);
	double M2 = M * M;
	double M4 = M2 * M2;
	double Tb2 = Tb * Tb;

	double X0 = r0 * B + flcos(B) * flsin(B) * (r1 + flsin(B) * flsin(B) * (r2 + flsin(B) * flsin(B) * r3));
	double x_src = X0 + N * Tb * M2 * (0.5 + M2 / 24.0 * (5.0 - Tb2 + 9.0 * Y2 + 4.0 * Y2 * Y2) + M4 / 720.0 * (61.0 + (Tb2 - 58.0) * Tb2));
	double y_src = N * M * (1.0 + M2 / 6.0 * (1.0 + Y2 - Tb2) + M4 * M / 120.0 * (5.0 + (Tb2 - 18.0) * Tb2 - (58.0 * Tb2 - 14.0) * Y2));

	return double3(x_src * info.utm_s + info.cs_dx, y_src * info.utm_s + info.cs_dy, info.cs_dz + H);
}
