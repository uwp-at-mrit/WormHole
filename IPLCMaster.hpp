#pragma once

#include "network/tcp.hpp"

namespace WarGrey::SCADA {
	class IPLCMaster;

	private class IPLCStatusListener {
	public:
		virtual void on_plc_connectivity_changed(WarGrey::SCADA::IPLCMaster* master, bool connected) {}

	public:
		virtual void on_send_data(WarGrey::SCADA::IPLCMaster* master, long long bytes, double span_ms, double timestamp_ms) {}
		virtual void on_receive_data(WarGrey::SCADA::IPLCMaster* master, long long bytes, double span_ms, double timestamp_ms) {}
		virtual void on_confirm_data(WarGrey::SCADA::IPLCMaster* master, long long bytes, double span_ms, double timestamp_ms) {}
	};

	private class IPLCMaster abstract : public WarGrey::SCADA::ITCPConnection {
	public:
		virtual ~IPLCMaster() noexcept;

	public:
		void set_suicide_timeout(long long ms);
		void push_plc_status_listener(WarGrey::SCADA::IPLCStatusListener* listener);
		void notify_connectivity_changed();
		void notify_data_sent(long long bytes, double span_ms);
		void notify_data_received(long long bytes, double span_ms);
		void notify_data_confirmed(long long bytes, double span_ms);

	private:
		std::list<WarGrey::SCADA::IPLCStatusListener*> listeners;
		Platform::Object^ killer;
	};
}
