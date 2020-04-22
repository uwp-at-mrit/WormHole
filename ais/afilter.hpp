#pragma once

#include "ais/abitfields.hpp"
#include "ais/amessage.hpp"

namespace WarGrey::DTPM {
    double ais_degrees_to_DDmm_mm(double degrees);

    double ais_latitude_filter(WarGrey::DTPM::I<4>& latitude);
    double ais_longitude_filter(WarGrey::DTPM::I<4>& longitude);

    double ais_turn_filter(WarGrey::DTPM::I<3>& speed);
    double ais_speed_filter(WarGrey::DTPM::U<1>& speed);
    double ais_course_filter(WarGrey::DTPM::U<1>& course);
    double ais_heading360_filter(WarGrey::DTPM::u heading);

    double ais_draught_filter(WarGrey::DTPM::U<1>& draught);
    
    void ais_shipbox_filter(WarGrey::DTPM::AISShape& box, double* length, double* width, double* gps_fl, double* gps_fw);
}
