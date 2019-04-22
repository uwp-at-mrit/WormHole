#include "IPLCMaster.hpp"

#include "datum/time.hpp"

using namespace WarGrey::SCADA;

void IPLCMaster::notify_data_confirmed(long long bytes, double span_ms) {
	double timestamp = current_inexact_milliseconds();

	for (auto it = this->listeners.begin(); it != this->listeners.end(); it++) {
		(*it)->on_confirm_data(this, bytes, span_ms, timestamp);
	}
}
