#pragma once

#include <string>

#include "gps/genums.hpp"

#include "datum/natural.hpp"

// https://en.wikipedia.org/wiki/Automatic_identification_system#Messages_sent_to_other_equipment_in_the_ship

namespace WarGrey::DTPM {
    private enum class AISMessage {
        PositionReportClassA, PositionReportClassA_AssignedSchedule, PositionReportClassA_Response2Interrogation,
        BaseStationReport, StaticVoyageData, BinaryAddressed, BinaryAcknowledge, BinaryBroadcast,
        StdSARAircraftPositionReport, UTCInquiry, UTCReponse,
        AddressedSafty, SaftyAcknowledge, SaftyBroadcast,
        Interrogation, AssignmentModeCommand, DGNSSBinaryBroadcast,
        StdClassBCSPositionReport, ExtClassBEquipmentPositionReport,
        DataLinkManagement, Aid2NavigationReport, ChannelManagement,
        GroupAssignmentCommand, StaticDataReport,
        SingleSlotBinary, MultipleSlotBinary,
        PositionReport4LongRangeApplications
    };

    private enum class AISNavigation {
        UnderWayUsingEngine, AtAnchor, NotUnderCommand, RestrictedManoeuverability,
        ConstrainedDraught, Moored, Aground, EngagedInFishing, UnderWaySailing,
        rHSC, rWIG, r11, r12, r13, // reserved statuses
        SARTisActive, Undefined
    };

    private enum class AISManeuver {
        None, Normal, Special
    };

	private struct AINMEA {
		int s_size = 0;
		int s_idx = 0;
		int msg_id = 0;
		unsigned char channel;
		std::string payload;
		int pad_bits;
	};

    private struct PRCA {
        WarGrey::DTPM::u repeat;
        WarGrey::DTPM::u mmsi;
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

	/*************************************************************************************************/
	void scan_ainmea(WarGrey::DTPM::AINMEA* nmea, const unsigned char* pool, size_t* cursor, size_t endp1);

    /*************************************************************************************************/
    WarGrey::DTPM::AISMessage ais_message_type(WarGrey::GYDM::Natural& payload);

    void ais_extract_prca(WarGrey::DTPM::PRCA* prca, WarGrey::GYDM::Natural& payload);
}
