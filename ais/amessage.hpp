#pragma once

#include <string>

#include "ais/abitfields.hpp"

#include "gps/genums.hpp"

#include "datum/natural.hpp"

// https://en.wikipedia.org/wiki/Automatic_identification_system#Messages_sent_to_other_equipment_in_the_ship

namespace WarGrey::DTPM {
    private enum class AISMessage {
        PositionReportClassA, PositionReportClassA_AssignedSchedule, PositionReportClassA_Response2Interrogation,
        BaseStationReport, StaticVoyageData, BinaryPoint2Point, BinaryAcknowledge, BinaryBroadcast,
        StdSARAircraftPositionReport, UTCInquiry, UTCReponse,
        AddressedSafty, SaftyAcknowledge, SaftyBroadcast,
        Interrogation, AssignmentModeCommand, DGNSSBinaryBroadcast,
        PositionReportClassB, PositionReportClassB_Extended,
        DataLinkManagement, Aid2NavigationReport, ChannelManagement,
        GroupAssignmentCommand, StaticDataReport,
        SingleSlotBinary, MultipleSlotBinary,
        PositionReport4LongRangeApplications,
        _
    };

    private enum class AISNavigation {
        UnderWayUsingEngine, AtAnchor, NotUnderCommand, RestrictedManoeuverability,
        ConstrainedDraught, Moored, Aground, EngagedInFishing, UnderWaySailing,
        rHSC, rWIG, r11, r12, r13, // reserved statuses
        SARTisActive, Undefined,
        _
    };

    private enum class AISManeuver {
        Unavailable, Normal, Special,
        _
    };

    private enum class AISPositionFixType {
        Undefined, GPS, GLONASS, LoranC, Chayka,
        IntegratedNavigationSystem, Surveyed, Galileo,
        _
    };

    private enum class AISShipType {
        Unavailable,

        // reserved for future use
        r1, r2, r3, r4, r5, r6, r7, r8, r9, r10,
        r11, r12, r13, r14, r15, r16, r17, r18, r19,

        /* Wing-In-Ground Vessel, with hazardous level and reserved */
        WIG, WIG_A, WIG_B, WIG_C, WIG_D,
        WID_r25, WID_r26, WID_r27, WID_r28, WID_r29,

        Fishing, Towing, LargeTowing, DredgingOps, DivingOps, MilitaryOps, Sailing, PleasureCraft,

        // reserved for future use
        r38, r39,

        /* High Speed Craft, with hazardous level, reserved and no additional information */
        HSC, HSC_A, HSC_B, HSC_C, HSC_D,
        HSC_r45, HSC_r46, HSC_r47, HSC_r48, HSC_plain,

        PilotVessel, RescueVessel, Tug, PortTender, AntiPollutionEquipment, LawEnforcement,
        SpareLocalVessel56, SpareLocalVessel57, MedicalTransport, NoncombatantShip,

        /* Passenger, with hazardous level, reserved and no additional information */
        Passenger, Passenger_A, Passenger_B, Passenger_C, Passenger_D,
        Passenger_r65, Passenger_r66, Passenger_r67, Passenger_r68, Passenger_plain,

        /* Cargo, with hazardous level, reserved and no additional information */
        Cargo, Cargo_A, Cargo_B, Cargo_C, Cargo_D,
        Cargo_r75, Cargo_r76, Cargo_r77, Cargo_r78, Cargo_plain,

        /* Tanker, with hazardous level, reserved and no additional information */
        Tanker, Tanker_A, Tanker_B, Tanker_C, Tanker_D,
        Tanker_r85, Tanker_r86, Tanker_r87, Tanker_r88, Tanker_plain,

        /* Others, with hazardous level, reserved and no additional information */
        Other, Other_A, Other_B, Other_C, Other_D,
        Other_r95, Other_r96, Other_r97, Other_r98, Other_plain,

        /* and more that not uncommon in the wild */

        _
    };

    private enum class AISAidType {
        Unspecified, ReferencePoint, RACON, FixedOffShore, Spare,
        Light, Light_Sector, LeadingLightFront, LeadingLightRear,

        Beacon_N, Beacon_E, Beacon_S, Beacon_W,
        Beacon_Port, Beacon_Starboard, Beacon_PreferredPort, Beacon_PreferredStarboard,
        Beacon_Isolated, Beacon_Safe, Beacon_Mark,

        Cardinal_N, Cardinal_E, Cardinal_S, Cardinal_W,
        Port, Starboard, PreferredPort, PreferredStarboard,

        Isolated, Safe, Mark, LANBY,
        
        _
    };

    /*********************************************************************************************/
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

    private struct PRCA {
    public:
        PRCA(WarGrey::GYDM::Natural& payload);

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

    private struct PRCB {
    public:
        PRCB(WarGrey::GYDM::Natural& payload);

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

    private struct PRCBE {
    public:
        PRCBE(WarGrey::GYDM::Natural& payload);

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

    private struct A2NR {
    public:
        A2NR(WarGrey::GYDM::Natural& payload);

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
        WarGrey::DTPM::t name_extension;
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

	/*************************************************************************************************/
	void scan_ainmea(WarGrey::DTPM::AINMEA* nmea, const unsigned char* pool, size_t* cursor, size_t endp1);

    /*************************************************************************************************/
    WarGrey::DTPM::AISMessage ais_message_type(WarGrey::GYDM::Natural& payload);
    uint8 ais_repeat_indicator(WarGrey::GYDM::Natural& payload);
    uint16 ais_mobile_marine_service_identifier(WarGrey::GYDM::Natural& payload);
}
