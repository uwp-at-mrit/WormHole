#pragma once

#include <list>

#include "gps/genums.hpp"
#include "gps/gmessage.hpp"

#include "network/tcp.hpp"
#include "network/stream.hpp"

#include "syslog.hpp"

namespace WarGrey::DTPM {
#define GPS_RECEIVER_POOL_LENGTH 256

	private class INMEA0183Receiver abstract {
	public:
		virtual bool available(int id) { return true; }

	public:
		virtual void on_message(int id, long long timepoint_ms,
			const unsigned char* pool, size_t head_start, size_t start, size_t endp1,
			WarGrey::GYDM::Syslog* logger) = 0;
	};

	private class INMEA0183 abstract : public WarGrey::SCADA::ITCPStatedConnection {
    public:
        virtual ~INMEA0183() noexcept;

		INMEA0183(WarGrey::SCADA::TCPType type, WarGrey::GYDM::Syslog* logger, Platform::String^ server, uint16 service,
			WarGrey::DTPM::INMEA0183Receiver* receiver, int id = 0);

	public:
		Platform::String^ device_hostname() override;
		Platform::String^ device_description() override;
		int device_identity();

	public:
		WarGrey::GYDM::Syslog* get_logger() override;
		void shake_hands() override;
		bool connected() override;
		void suicide() override;

	public:
		void push_receiver(WarGrey::DTPM::INMEA0183Receiver* receiver);
		void tolerate_bad_checksum(bool yes_no);

	private:
		void clear();
		void wait_process_confirm_loop();
		size_t check_message();
		void realign_message();
		void apply_confirmation(const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1);

	protected:
		std::list<WarGrey::DTPM::INMEA0183Receiver*> receivers;
		Windows::Networking::HostName^ device;
		WarGrey::GYDM::Syslog* logger;

    private:
		Windows::Networking::Sockets::StreamSocket^ socket;
        Platform::String^ service;

	private:
		Windows::Storage::Streams::DataReader^ nmeain;

	private:
		uint8 message_pool[GPS_RECEIVER_POOL_LENGTH];
		size_t message_start;
		size_t refresh_data_size;
		size_t data_size;
		size_t field0_idx;
		size_t checksum_idx;
		size_t CR_LF_idx;

	private:
		bool tolerate_checksum;
		int id;
    };

    private class GPS : public WarGrey::DTPM::INMEA0183 {
    public:
        GPS(WarGrey::GYDM::Syslog* logger, Platform::String^ server, uint16 port, INMEA0183Receiver* receiver = nullptr)
			: INMEA0183(WarGrey::SCADA::TCPType::GPS, logger, server, port, receiver) {}

	public:
		void send_scheduled_request(long long count, long long interval, long long uptime) {}
	};

	private class GPSReceiver : public WarGrey::DTPM::INMEA0183Receiver {
	public:
		void on_message(int id, long long timepoint_ms,
			const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1,
			WarGrey::GYDM::Syslog* logger) override;

	public:
		virtual void pre_scan_data(int id, WarGrey::GYDM::Syslog* logger) {}
		virtual void post_scan_data(int id, WarGrey::GYDM::Syslog* logger) {}

	public:
		virtual void on_GGA(int id, long long timepoint_ms, WarGrey::DTPM::GGA* gga, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_VTG(int id, long long timepoint_ms, WarGrey::DTPM::VTG* vtg, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_GLL(int id, long long timepoint_ms, WarGrey::DTPM::GLL* gll, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_GSA(int id, long long timepoint_ms, WarGrey::DTPM::GSA* gsa, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_GSV(int id, long long timepoint_ms, WarGrey::DTPM::GSV* gsv, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_ZDA(int id, long long timepoint_ms, WarGrey::DTPM::ZDA* zda, WarGrey::GYDM::Syslog* logger) {}

		virtual void on_HDT(int id, long long timepoint_ms, WarGrey::DTPM::HDT* hdt, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_ROT(int id, long long timepoint_ms, WarGrey::DTPM::ROT* rot, WarGrey::GYDM::Syslog* logger) {}
	};
}
