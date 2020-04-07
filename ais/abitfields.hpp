#pragma once

#include <string>

#include "datum/natural.hpp"
#include "datum/bytes.hpp"

namespace WarGrey::DTPM {
    template<typename I, int S>
    private struct ScaledInteger {
    public:
        ScaledInteger(I integer) : scalar(integer) {}

    public:
        float value() { return float(this->scalar) / (float(S) * 10.0F); }

    private:
        I scalar;
    };

    template<int S> private struct U : public WarGrey::DTPM::ScaledInteger<unsigned int, S> { };
    template<int S> private struct I : public WarGrey::DTPM::ScaledInteger<signed int, S> { };

    typedef unsigned int           u;
    typedef signed int             i;
    typedef bool                   b;
    typedef WarGrey::SCADA::bytes  x;
    typedef WarGrey::GYDM::Natural t;
    typedef WarGrey::SCADA::bytes  d;

    WarGrey::GYDM::Natural ais_unarmor(std::string& payload, int pad_bits);
    
    bool ais_is_bit_set(WarGrey::GYDM::Natural& payload, size_t idx);
    WarGrey::GYDM::Natural ais_bit_field(WarGrey::GYDM::Natural& payload, size_t idx, size_t length);
}
