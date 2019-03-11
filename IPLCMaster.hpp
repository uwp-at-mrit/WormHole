#pragma once

#include "network/tcp.hpp"

namespace WarGrey::SCADA {
	private class IPLCStatusListener : public WarGrey::SCADA::ITCPStatusListener {
	public:
		virtual void on_confirm_data(WarGrey::SCADA::ITCPConnection* master, long long bytes, double span_ms, double timestamp_ms) {}
	};

	private class IPLCMaster abstract : public WarGrey::SCADA::ITCPFeedBackConnection<WarGrey::SCADA::IPLCStatusListener> {	
	public:
		void notify_data_confirmed(long long bytes, double span_ms);
	};
}
