#pragma once

#include <list>
#include <cinttypes>

#include "gps/gps_enums.hpp"

#include "datum/object.hpp"

#include "network/tcp.hpp"
#include "network/stream.hpp"

#include "syslog.hpp"

namespace WarGrey::SCADA {
#define GPS_RECEIVER_POOL_LENGTH 128

	private class IGPSReceiver abstract {
	public:
		virtual bool available() { return true; }

	public:
		virtual void on_GGA(long long timepoint_ms, double utc, double latitude, double longitude, NMEA_GQI gqi,
			unsigned long long satellites, double precision_hdillution, double altitude, double undulation,
			double age, unsigned long long diffbase_station_id,
			WarGrey::SCADA::Syslog* logger) = 0;

		virtual void on_VTG(long long timepoint_ms, double true_tmg_deg, double magnetic_tmg_deg, double s_kn, double s_kmph, NMEA_PSMI mode,
			WarGrey::SCADA::Syslog* logger) = 0;
		
		virtual void on_HDT(long long timepoint_ms, double heading_deg, WarGrey::SCADA::Syslog* logger) = 0;
		virtual void on_GLL(long long timepoint_ms, double utc, double latitude, double longitude, bool validity, NMEA_PSMI mode, WarGrey::SCADA::Syslog* logger) = 0;
	};

	private class IGPS abstract : public WarGrey::SCADA::ITCPStatedConnection {
    public:
        virtual ~IGPS() noexcept;

		IGPS(WarGrey::SCADA::Syslog* logger, Platform::String^ server, uint16 service, WarGrey::SCADA::IGPSReceiver* confirmation);

	public:
		Platform::String^ device_hostname() override;
		Platform::String^ device_description() override;

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
		void apply_confirmation(const unsigned char* pool, size_t start, size_t endp1);

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
		uint8 data_pool[GPS_RECEIVER_POOL_LENGTH];
		size_t message_start;
		size_t refresh_data_size;
		size_t data_size;
		size_t field0_idx;
		size_t checksum_idx;
		size_t CR_LF_idx;
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
		void on_GGA(long long timepoint_ms, double utc, double latitude, double longitude, NMEA_GQI gqi,
			unsigned long long satellites, double precision_hdillution, double altitude, double undulation,
			double age, unsigned long long diffbase_station_id,
			WarGrey::SCADA::Syslog* logger) override {}

		void on_VTG(long long timepoint_ms, double true_tmg_deg, double magnetic_tmg_deg, double s_kn, double s_kmph, NMEA_PSMI mode,
			WarGrey::SCADA::Syslog* logger) override {}
		
		void on_HDT(long long timepoint_ms, double heading_deg, WarGrey::SCADA::Syslog* logger) override {}
		void on_GLL(long long timepoint_ms, double utc, double latitude, double longitude, bool validity, NMEA_PSMI mode, WarGrey::SCADA::Syslog* logger) override {}
	};
}
