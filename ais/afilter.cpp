#include "ais/afilter.hpp"

#include "datum/box.hpp"
#include "datum/flonum.hpp"

using namespace WarGrey::DTPM;
using namespace WarGrey::SCADA;

/* https://gpsd.gitlab.io/gpsd/AIVDM.html#_aivdmaivdo_payload_armoring */

static inline double rate_of_turn(double turn) {
    double ROT_ais = 708.0 * double(turn) / 127.0;

    return flexpt(ROT_ais / 4.733, 2.0);
}

/*************************************************************************************************/
double WarGrey::DTPM::ais_degrees_to_DDmm_mm(double degrees) {
    double abs_deg = flabs(degrees);
    double DD = flfloor(abs_deg);
    double mm = (abs_deg - DD) * 60.0;
    double DDmm_mm = DD * 100.0 + mm;

    return (degrees >= 0.0) ? DDmm_mm : -DDmm_mm;
}

/*************************************************************************************************/
double WarGrey::DTPM::ais_latitude_filter(I<4>& lat) {
    return (lat.value() == 0x3412140) ? flnan : (lat.unbox() / 60.0);
}

double WarGrey::DTPM::ais_longitude_filter(I<4>& lon) {
    return (lon.value() == 0x6791AC0) ? flnan : (lon.unbox() / 60.0);
}

double WarGrey::DTPM::ais_turn_filter(I<3>& turn) {
    unsigned int raw = turn.value();
    double rot = turn.unbox();

    if (raw == 0x80) {
        rot = flnan;
    } else if (raw > 0) { // turn right
        if (raw == 127) {
            rot = 10.0; // deg/min, without turn indicator
        } else {
            rot = rate_of_turn(double(rot));
        }
    } else { // turn left
        if (raw == -127) {
            rot = -10.0; // deg/min, without turn indicator
        } else {
            rot = -rate_of_turn(double(rot));
        }
    }

    return rot;
}

double WarGrey::DTPM::ais_speed_filter(U<1>& speed) {
    return (speed.value() == 1023) ? flnan : speed.unbox();
}

double WarGrey::DTPM::ais_course_filter(U<1>& course) {
    return (course.value() == 0xE10) ? flnan : course.unbox();
}

double WarGrey::DTPM::ais_heading360_filter(u heading) {
    return (heading == 511) ? flnan : double(heading);
}

/*************************************************************************************************/
double WarGrey::DTPM::ais_draught_filter(U<1>& draught) {
    return draught.unbox();
}

void ais_shipbox_filter(WarGrey::DTPM::AISShape& box, double* length, double* width, double* gps_fl, double* gps_fw) {
    double l = double(box.bow + box.stern);
    double w = double(box.port + box.starboard);
    double fl = double(box.bow) / l;
    double fw = double(box.port) / w;

    SET_VALUES(length, l, width, w);
    SET_VALUES(gps_fl, fl, gps_fw, fw);
}
