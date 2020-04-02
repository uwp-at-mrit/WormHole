#pragma once

#include <map>

#include "gps/nmea0183.hpp"

namespace WarGrey::DTPM {
	private class Transponder : public WarGrey::DTPM::INMEA0183Receiver {
	public:
		void on_message(int id, long long timepoint_ms,
			const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1,
			WarGrey::GYDM::Syslog* logger) override;
		
	public:
		virtual void pre_scan_data(int id, WarGrey::GYDM::Syslog* logger) {}
		virtual void post_scan_data(int id, WarGrey::GYDM::Syslog* logger) {}

	private:
		struct Sentences;
		std::map<int, WarGrey::DTPM::Transponder::Sentences> sentences;
	};
}
