#pragma once

#include <string>

namespace WarGrey::DTPM {
	std::basic_string<unsigned char> ais_unarmor(std::string& payload);
}
