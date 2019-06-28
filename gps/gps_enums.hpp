#pragma once

namespace WarGrey::SCADA {
	private enum class NMEA_PSMI : unsigned char { 
		Autonomouse, Differential, Estimated, Manual, NotValid, _
	};
}
