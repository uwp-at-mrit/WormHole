#pragma once

namespace WarGrey::DTPM {
	private enum class NMEA_PSMI : unsigned char { 
		Autonomouse, Differential, Estimated, Manual, NotValid,
		_
	};

	private enum class NMEA_GQI : unsigned char {
		Invalid, SinglePoint, PseudorangeDifferential,
		FixedRTK, FloatRTK,
		DeadReckoningMode, ManualInputMode, SimulatorMode,
		WAAS,
		_
	};

	private enum class NMEA_FIX_TYPE : unsigned char {
		None, TWO_Dimension, THREE_DIMENSION,
		_
	};
}
