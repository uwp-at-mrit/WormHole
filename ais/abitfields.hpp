#pragma once

#include <string>

#include "datum/natural.hpp"
#include "datum/bytes.hpp"

namespace WarGrey::DTPM {
    template<typename I, int S>
    private struct ScaledInteger {
    public:
        ScaledInteger(I integer) { this->box(integer); }

    public:
        void box(I integer) { this->scalar = integer; }
        float unbox() { return float(this->scalar) / (float(S) * 10.0F); }

    private:
        I scalar;
    };

    template<int S>
    private struct U : public WarGrey::DTPM::ScaledInteger<unsigned int, S> {
    public:
        U() : U(0U) {}
        U(unsigned int uint) : ScaledInteger(uint) {}
    };

    template<int S>
    private struct I : public WarGrey::DTPM::ScaledInteger<signed int, S> {
    public:
        I() : I(0) {}
        I(signed int sint) : ScaledInteger(sint) {}
    };

    typedef unsigned int           u;
    typedef signed int             i;
    typedef bool                   b;
    typedef WarGrey::SCADA::bytes  x;
    typedef WarGrey::GYDM::Natural t;
    typedef WarGrey::SCADA::bytes  d;

    WarGrey::GYDM::Natural ais_unarmor(std::string& payload, int pad_bits);

    // 0-origin indice
    bool ais_is_bit_set(WarGrey::GYDM::Natural& payload, size_t idx);
    WarGrey::GYDM::Natural ais_bit_field(WarGrey::GYDM::Natural& payload, size_t idx, size_t length);

    uint32 ais_uint32_field(WarGrey::GYDM::Natural& payload, size_t idx, size_t length);
    uint64 ais_uint64_field(WarGrey::GYDM::Natural& payload, size_t idx, size_t length);
}
