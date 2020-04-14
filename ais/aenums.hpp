#pragma once

// https://en.wikipedia.org/wiki/Automatic_identification_system#Messages_sent_to_other_equipment_in_the_ship

namespace WarGrey::DTPM {
    private enum class AISMessage {
        PositionReportASO, PositionReportASO_AssignedSchedule, PositionReportASO_Response2Interrogation,
        BaseStationReport, StaticVoyageData, BinaryUnicast, BinaryAcknowledge, BinaryBroadcast,
        StdSARAircraftPositionReport, UTCInquiry, UTCResponse,
        SaftyUnicast, SaftyAcknowledge, SaftyBroadcast,
        Interrogation, AssignmentModeCommand, DGNSSBinaryBroadcast,
        PositionReportBCS, PositionReportBCS_Extended,
        DataLinkManagement, ATONReport, ChannelManagement,
        GroupAssignmentCommand, StaticDataReport,
        SingleSlotBinary, MultipleSlotBinary,
        PositionReportLongRangeBroadcast,

        // Message 28-63 are reserved for future use

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

    private enum class AISTRMode {
        TxATxB, TxA, TxB, 
        _
    };

    private enum class AISStationType {
        All, r1, All_ClassB,
        SARAircraft, Aid2Navigation,
        ClassB, Regional,
        
        r10, r11, r12, r13, r14, r15,

        _
    };

    private enum class AISStationInterval {
        Auto, M10, M6, M3, M1, S30, S15, S10, S5, Shorter, Longer,

        r10, r11, r12, r13, r14, r15,

        _
    };
}
