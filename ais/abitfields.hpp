#pragma once

#include <string>

#include "datum/natural.hpp"

namespace WarGrey::DTPM {
    WarGrey::GYDM::Natural ais_unarmor(std::string& payload, int pad_bits);
}
