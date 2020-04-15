#pragma once

#include "ais/abitfields.hpp"

namespace WarGrey::DTPM {
    double ais_latitude_filter(WarGrey::DTPM::I<4>& latitude);
    double ais_longitude_filter(WarGrey::DTPM::I<4>& longitude);

    double ais_turn_filter(WarGrey::DTPM::I<3>& speed);
    double ais_speed_filter(WarGrey::DTPM::U<1>& speed);
    double ais_course_filter(WarGrey::DTPM::U<1>& course);
    double ais_heading360_filter(WarGrey::DTPM::u heading);
}
