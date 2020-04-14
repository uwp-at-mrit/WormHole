#pragma once

#include <string>

#include "ais/abitfields.hpp"
#include "ais/aenums.hpp"

#include "datum/natural.hpp"

// https://en.wikipedia.org/wiki/Automatic_identification_system#Messages_sent_to_other_equipment_in_the_ship
// https://www.navcen.uscg.gov/?pageName=AISMessages

namespace WarGrey::DTPM {
	private struct AINMEA {
		int s_size = 0;
		int s_idx = 0;
		int msg_id = 0;
		unsigned char channel;
		std::string payload;
		int pad_bits;
	};

    /*********************************************************************************************/
    private struct AISShape {
        WarGrey::DTPM::u bow;
        WarGrey::DTPM::u stern;
        WarGrey::DTPM::u port;
        WarGrey::DTPM::u starboard;
    };

    private struct AISArea {
        WarGrey::DTPM::I<1> ne_longitude;
        WarGrey::DTPM::I<1> ne_latitude;
        WarGrey::DTPM::I<1> sw_longitude;
        WarGrey::DTPM::I<1> sw_latitude;
    };

    /*********************************************************************************************/
    private struct ASO {
    public:
        ASO(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::AISNavigation status;
        WarGrey::DTPM::I<3> turn; 
        WarGrey::DTPM::U<1> speed;
        WarGrey::DTPM::b accuracy;
        WarGrey::DTPM::I<4> longitude;
        WarGrey::DTPM::I<4> latitude;
        WarGrey::DTPM::U<1> course;
        WarGrey::DTPM::u heading;
        WarGrey::DTPM::u timetamp;
        WarGrey::DTPM::AISManeuver maneuver;
        WarGrey::DTPM::b raim;
        WarGrey::DTPM::u radio;
    };

    private struct BSR {
    public:
        BSR(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::u year;
        WarGrey::DTPM::u month;
        WarGrey::DTPM::u day;
        WarGrey::DTPM::u hour;
        WarGrey::DTPM::u minute;
        WarGrey::DTPM::u second;
        WarGrey::DTPM::b accuracy;
        WarGrey::DTPM::I<4> longitude;
        WarGrey::DTPM::I<4> latitude;
        WarGrey::DTPM::AISPositionFixType epfd;
        WarGrey::DTPM::b raim;
        WarGrey::DTPM::u radio;
    };

    private struct SVD {
    public:
        SVD(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::u ais_version;
        WarGrey::DTPM::u imo;
        WarGrey::DTPM::t callsign;
        WarGrey::DTPM::t shipname;
        WarGrey::DTPM::AISShipType shiptype;
        WarGrey::DTPM::AISShape shipbox;
        WarGrey::DTPM::AISPositionFixType epfd;
        WarGrey::DTPM::u month;
        WarGrey::DTPM::u day;
        WarGrey::DTPM::u hour;
        WarGrey::DTPM::u minute;
        WarGrey::DTPM::U<1> draught;
        WarGrey::DTPM::t destination;
        WarGrey::DTPM::b dte;
    };

    private struct BAM {
    public:
        BAM(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::u seqno;
        WarGrey::DTPM::u dest_mmsi;
        WarGrey::DTPM::b retransmit;
        WarGrey::DTPM::u dac;
        WarGrey::DTPM::u fid;
    };

    private struct BA {
    public:
        BA(WarGrey::GYDM::Natural& payload);

    public:
        size_t slots_count;
        WarGrey::DTPM::u mmsis[4];
        WarGrey::DTPM::u mmsiseqs[4];
    };

    private struct BBM {
    public:
        BBM(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::u dac;
        WarGrey::DTPM::u fid;
    };

    private struct SRAPR {
    public:
        SRAPR(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::u altitude;
        WarGrey::DTPM::u speed;
        WarGrey::DTPM::b accuracy;
        WarGrey::DTPM::I<4> longitude;
        WarGrey::DTPM::I<4> latitude;
        WarGrey::DTPM::U<1> course;
        WarGrey::DTPM::u timetamp;
        WarGrey::DTPM::b dte;
        WarGrey::DTPM::b assigned;
        WarGrey::DTPM::b raim;
        WarGrey::DTPM::u radio;
    };

    private struct UTCI {
    public:
        UTCI(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::u dest_mmsi;
    };

    private struct ASRM {
    public:
        ASRM(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::u seqno;
        WarGrey::DTPM::u dest_mmsi;
        WarGrey::DTPM::b retransmit;
    };

    private struct SRBM {
    public:
        SRBM(WarGrey::GYDM::Natural& payload);
    };

    private struct BSIQ {
    public:
        BSIQ(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::u mmsi1;
        WarGrey::DTPM::u type11;
        WarGrey::DTPM::u offset11;
        WarGrey::DTPM::u type12;
        WarGrey::DTPM::u offset12;
        WarGrey::DTPM::u mmsi2;
        WarGrey::DTPM::u type21;
        WarGrey::DTPM::u offset21;
    };

    private struct AMC {
    public:
        AMC(WarGrey::GYDM::Natural& payload);

    public:
        size_t slots_count;
        WarGrey::DTPM::u mmsis[2];
        WarGrey::DTPM::u offsets[2];
        WarGrey::DTPM::u increments[2];
    };

    private struct GBBM {
    public:
        GBBM(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::I<1> longitude;
        WarGrey::DTPM::I<1> latitude;
    };

    private struct BCS {
    public:
        BCS(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::U<1> speed;
        WarGrey::DTPM::b accuracy;
        WarGrey::DTPM::I<4> longitude;
        WarGrey::DTPM::I<4> latitude;
        WarGrey::DTPM::U<1> course;
        WarGrey::DTPM::u heading;
        WarGrey::DTPM::u timetamp;
        WarGrey::DTPM::u regional;
        WarGrey::DTPM::b cs;
        WarGrey::DTPM::b display;
        WarGrey::DTPM::b dsc;
        WarGrey::DTPM::b band;
        WarGrey::DTPM::b msg22;
        WarGrey::DTPM::b assigned;
        WarGrey::DTPM::b raim;
        WarGrey::DTPM::u radio;
    };

    private struct BCSE {
    public:
        BCSE(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::U<1> speed;
        WarGrey::DTPM::b accuracy;
        WarGrey::DTPM::I<4> longitude;
        WarGrey::DTPM::I<4> latitude;
        WarGrey::DTPM::U<1> course;
        WarGrey::DTPM::u heading;
        WarGrey::DTPM::u timetamp;
        WarGrey::DTPM::u regional;
        WarGrey::DTPM::t shipname;
        WarGrey::DTPM::AISShipType shiptype;
        WarGrey::DTPM::AISShape shipbox;
        WarGrey::DTPM::AISPositionFixType epfd;
        WarGrey::DTPM::b raim;
        WarGrey::DTPM::b dte;
        WarGrey::DTPM::b assigned;
    };

    private struct DLM {
    public:
        DLM(WarGrey::GYDM::Natural& payload);

    public:
        size_t slots_count;
        WarGrey::DTPM::u offsets[4];
        WarGrey::DTPM::u numbers[4];
        WarGrey::DTPM::u timeouts[4];
        WarGrey::DTPM::u increments[4];
    };

    private struct ATON {
    public:
        ATON(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::AISAidType aid_type;
        WarGrey::DTPM::t name;
        WarGrey::DTPM::b accuracy;
        WarGrey::DTPM::I<4> longitude;
        WarGrey::DTPM::I<4> latitude;
        WarGrey::DTPM::AISShape shipbox;
        WarGrey::DTPM::AISPositionFixType epfd;
        WarGrey::DTPM::u timetamp;
        WarGrey::DTPM::b off_position;
        WarGrey::DTPM::u regional;
        WarGrey::DTPM::b raim;
        WarGrey::DTPM::b virtual_aid;
        WarGrey::DTPM::b assigned;
    };

    private struct CM {
    public:
        struct Target {
            WarGrey::DTPM::u dest1;
            WarGrey::DTPM::u dest2;
        };

        union Cast {
            Cast() {}
            ~Cast() noexcept {}

            WarGrey::DTPM::AISArea area;
            WarGrey::DTPM::CM::Target target;
        };

    public:
        CM(WarGrey::GYDM::Natural& payload);
        ~CM() noexcept;

    public:
        WarGrey::DTPM::u channel_a;
        WarGrey::DTPM::u channel_b;
        WarGrey::DTPM::AISTRMode txrx;
        WarGrey::DTPM::b power;
        WarGrey::DTPM::CM::Cast cast;
        WarGrey::DTPM::b addressed;
        WarGrey::DTPM::b band_a;
        WarGrey::DTPM::b band_b;
        WarGrey::DTPM::u zonesize;
    };

    private struct GAC {
    public:
        GAC(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::AISArea area;
        WarGrey::DTPM::AISStationType station_type;
        WarGrey::DTPM::AISShipType ship_type;
        WarGrey::DTPM::AISTRMode txrx;
        WarGrey::DTPM::AISStationInterval interval;
        WarGrey::DTPM::u quit_time;
    };

    private struct SDR {
    public:
        enum class Format { PartA, PartB, _ };

        union Craft {
            WarGrey::DTPM::AISShape box;
            WarGrey::DTPM::u mothership_mmsi;
        };

        struct A {
            WarGrey::DTPM::t shipname;
        };

        struct B {
            WarGrey::DTPM::AISShipType shiptype;
            WarGrey::DTPM::t vendorid;
            WarGrey::DTPM::u model;
            WarGrey::DTPM::u serial;
            WarGrey::DTPM::u callsign;

            bool auxiliary;
            WarGrey::DTPM::SDR::Craft craft;
        };

        union Part {
            Part() {}
            ~Part() noexcept {}

            WarGrey::DTPM::SDR::A a;
            WarGrey::DTPM::SDR::B b;
        };
    
    public:
        SDR(WarGrey::GYDM::Natural& payload);
        ~SDR() noexcept;

    public:
        WarGrey::DTPM::SDR::Format partno;
        WarGrey::DTPM::SDR::Part part;
    };

    private struct SSBM { // extremely rare message
    public:
        SSBM(WarGrey::GYDM::Natural& payload);
        
    public:
        WarGrey::DTPM::b addressed;
        WarGrey::DTPM::b structured;
    };

    private struct MSBM { // extremely rare message
    public:
        MSBM(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::b addressed;
        WarGrey::DTPM::b structured;
    };

    private struct LRB {
    public:
        LRB(WarGrey::GYDM::Natural& payload);

    public:
        WarGrey::DTPM::b accuracy;
        WarGrey::DTPM::b raim;
        WarGrey::DTPM::AISNavigation status;
        WarGrey::DTPM::I<4> longitude;
        WarGrey::DTPM::I<4> latitude;
        WarGrey::DTPM::u speed;
        WarGrey::DTPM::u course;
        WarGrey::DTPM::b gnss;
    };

	/*************************************************************************************************/
	void scan_ainmea(WarGrey::DTPM::AINMEA* nmea, const unsigned char* pool, size_t* cursor, size_t endp1);

    /*************************************************************************************************/
    WarGrey::DTPM::AISMessage ais_message_type(WarGrey::GYDM::Natural& payload);
    uint8 ais_repeat_indicator(WarGrey::GYDM::Natural& payload);
    uint16 ais_mobile_marine_service_identifier(WarGrey::GYDM::Natural& payload);
}
