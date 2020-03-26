#pragma once

#include <cinttypes>

#include "syslog.hpp"

namespace WarGrey::SCADA {
	uint8 modbus_illegal_function(uint8 function_code, WarGrey::GYDM::Syslog* logger);
	uint8 modbus_illegal_address(uint16 address, uint16 start, uint16 amount, WarGrey::GYDM::Syslog* logger);
	uint8 modbus_illegal_address(uint16 address, uint16 count, uint16 start, uint16 amount, WarGrey::GYDM::Syslog* logger);

	uint8 modbus_identification_not_found(uint16 id, WarGrey::GYDM::Syslog* logger);
	uint8 modbus_identification_not_found(uint16 id, uint8 start, uint8 end, WarGrey::GYDM::Syslog* logger);
	uint8 modbus_identification_not_found(uint16 id, uint8 start, uint8 end, uint8 product, WarGrey::GYDM::Syslog* logger);

	uint8 modbus_illegal_data_value(uint16 value, uint16 vexpected, WarGrey::GYDM::Syslog* logger);
	uint8 modbus_illegal_data_value(uint16 value, uint16 vmin, uint16 vmax, WarGrey::GYDM::Syslog* logger);
	uint8 modbus_illegal_enum_value(uint16 value, uint16 v1, uint16 v2, WarGrey::GYDM::Syslog* logger);

	void modbus_protocol_fatal();
	void modbus_protocol_fatal(WarGrey::GYDM::Syslog* logger, Platform::String^ message);
	void modbus_protocol_fatal(WarGrey::GYDM::Syslog* logger, const wchar_t *fmt, ...);

	void modbus_discard_current_adu();
	void modbus_discard_current_adu(Platform::String^ message, WarGrey::GYDM::Syslog* logger);
	void modbus_discard_current_adu(WarGrey::GYDM::Syslog* logger, const wchar_t *fmt, ...);
}
