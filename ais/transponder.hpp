#pragma once

#include <map>

#include "gps/nmea0183.hpp"

#include "ais/amessage.hpp"

namespace WarGrey::DTPM {
	private class AIS : public WarGrey::DTPM::INMEA0183 {
	public:
		AIS(WarGrey::GYDM::Syslog* logger, Platform::String^ server, uint16 port, INMEA0183Receiver* receiver = nullptr)
			: INMEA0183(WarGrey::SCADA::TCPType::AIS, logger, server, port, receiver) {}

	public:
		void send_scheduled_request(long long count, long long interval, long long uptime) {}
	};

	private class Transponder : public WarGrey::DTPM::INMEA0183Receiver {
	public:
		void on_message(int id, long long timepoint_ms,
			const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1,
			WarGrey::GYDM::Syslog* logger) override;
		
	public:
		virtual void pre_scan_data(int id, WarGrey::GYDM::Syslog* logger) {}
		virtual void post_scan_data(int id, WarGrey::GYDM::Syslog* logger) {}

	private:
		void on_message(int id, long long timepoint_ms, bool self, WarGrey::DTPM::AINMEA* message, WarGrey::GYDM::Syslog* logger);

	private:
		struct Sentences;
		std::map<int, WarGrey::DTPM::Transponder::Sentences*> sentences;
	};
}
