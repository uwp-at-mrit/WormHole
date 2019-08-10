#pragma once

#include <list>
#include <cinttypes>

#include "gps/genums.hpp"
#include "gps/gmessage.hpp"

#include "datum/object.hpp"

#include "network/tcp.hpp"
#include "network/stream.hpp"

#include "syslog.hpp"

namespace WarGrey::SCADA {
#define GPS_RECEIVER_POOL_LENGTH 256

	private class IGPSReceiver abstract {
	public:
		virtual bool available(int id) { return true; }

	public:
		virtual void on_message(int id, long long timepoint_ms,
			const unsigned char* pool, size_t head_start, size_t start, size_t endp1,
			WarGrey::SCADA::Syslog* logger) = 0;
	};

	private class IGPS abstract : public WarGrey::SCADA::ITCPStatedConnection {
    public:
        virtual ~IGPS() noexcept;

		IGPS(WarGrey::SCADA::Syslog* logger, Platform::String^ server, uint16 service,
			WarGrey::SCADA::IGPSReceiver* confirmation, int id = 0);

	public:
		Platform::String^ device_hostname() override;
		Platform::String^ device_description() override;
		int device_identity();

	public:
		Syslog* get_logger() override;
		void shake_hands() override;
		bool connected() override;
		void suicide() override;

	public:
		void push_confirmation_receiver(WarGrey::SCADA::IGPSReceiver* confirmation);

	private:
		void clear();
		void wait_process_confirm_loop();
		size_t check_message();
		void realign_message();
		void apply_confirmation(const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1);

	protected:
		std::list<WarGrey::SCADA::IGPSReceiver*> confirmations;
		Windows::Networking::HostName^ device;
		WarGrey::SCADA::Syslog* logger;

    private:
		Windows::Networking::Sockets::StreamSocket^ socket;
        Platform::String^ service;

	private:
		Windows::Storage::Streams::DataReader^ gpsin;

	private:
		uint8 message_pool[GPS_RECEIVER_POOL_LENGTH];
		size_t message_start;
		size_t refresh_data_size;
		size_t data_size;
		size_t field0_idx;
		size_t checksum_idx;
		size_t CR_LF_idx;

	private:
		int id;
    };

    private class GPS : public WarGrey::SCADA::IGPS {
    public:
        GPS(WarGrey::SCADA::Syslog* logger, Platform::String^ server, uint16 port, IGPSReceiver* confirmation = nullptr)
			: IGPS(logger, server, port, confirmation) {}

	public:
		void send_scheduled_request(long long count, long long interval, long long uptime) {}
	};

	private class GPSReceiver : public WarGrey::SCADA::IGPSReceiver {
	public:
		void on_message(int id, long long timepoint_ms,
			const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1,
			WarGrey::SCADA::Syslog* logger) override;
		
	public:
		virtual void pre_scan_data(int id, WarGrey::SCADA::Syslog* logger) {}
		virtual void post_scan_data(int id, WarGrey::SCADA::Syslog* logger) {}

	public:
		virtual void on_GGA(int id, long long timepoint_ms, WarGrey::SCADA::GGA* gga, WarGrey::SCADA::Syslog* logger) {}
		virtual void on_VTG(int id, long long timepoint_ms, WarGrey::SCADA::VTG* vtg, WarGrey::SCADA::Syslog* logger) {}
		virtual void on_GLL(int id, long long timepoint_ms, WarGrey::SCADA::GLL* gll, WarGrey::SCADA::Syslog* logger) {}
		virtual void on_GSA(int id, long long timepoint_ms, WarGrey::SCADA::GSA* gsa, WarGrey::SCADA::Syslog* logger) {}
		virtual void on_GSV(int id, long long timepoint_ms, WarGrey::SCADA::GSV* gsv, WarGrey::SCADA::Syslog* logger) {}
		virtual void on_ZDA(int id, long long timepoint_ms, WarGrey::SCADA::ZDA* zda, WarGrey::SCADA::Syslog* logger) {}

		virtual void on_HDT(int id, long long timepoint_ms, WarGrey::SCADA::HDT* hdt, WarGrey::SCADA::Syslog* logger) {}
		virtual void on_ROT(int id, long long timepoint_ms, WarGrey::SCADA::ROT* rot, WarGrey::SCADA::Syslog* logger) {}

	};
}
