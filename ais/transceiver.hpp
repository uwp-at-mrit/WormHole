#pragma once

#include <map>

#include "ais/amessage.hpp"

#include "gps/nmea0183.hpp"

namespace WarGrey::DTPM {
	private class AIS : public WarGrey::DTPM::INMEA0183 {
	public:
		AIS(WarGrey::GYDM::Syslog* logger, Platform::String^ server, uint16 port, INMEA0183Receiver* receiver = nullptr)
			: INMEA0183(WarGrey::SCADA::TCPType::AIS, logger, server, port, receiver) {}

	public:
		void send_scheduled_request(long long count, long long interval, long long uptime) {}
	};

	private class Transceiver : public WarGrey::DTPM::INMEA0183Receiver {
	public:
		void on_message(int id, long long timepoint_ms,
			const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1,
			WarGrey::GYDM::Syslog* logger) override;
		
	public:
		virtual void pre_interpret_payload(int id, WarGrey::GYDM::Syslog* logger) {}
		virtual void post_interpret_payload(int id, WarGrey::GYDM::Syslog* logger) {}

	public:
		virtual void on_ASO(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::ASO* aso, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_BSR(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BSR* bsr, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_SVD(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SVD* svd, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_UTCI(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::UTCI* utci, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		
		virtual void on_BAM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BAM* bam, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_BA(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BA* ba, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_BBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BBM* bbc, uint8 priority, WarGrey::GYDM::Syslog* logger) {}

		virtual void on_SRAPR(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SRAPR* srapr, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_ASRM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::ASRM* asrm, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_SRBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SRBM* srbm, uint8 priority, WarGrey::GYDM::Syslog* logger) {}

		virtual void on_BSIQ(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BSIQ* bsiq, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_AMC(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::AMC* amc, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_GBBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::GBBM* gbbm, uint8 priority, WarGrey::GYDM::Syslog* logger) {}

		virtual void on_BCS(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BCS* bcs, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_BCSE(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BCSE* bcse, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		
		virtual void on_DLM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::DLM* dlm, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_ATON(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::ATON* aton, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_CM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::CM* cm, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_GAC(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::GAC* gac, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_SDR(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SDR* sdr, uint8 priority, WarGrey::GYDM::Syslog* logger) {}

		virtual void on_SSBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SSBM* ssbm, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_MSBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::MSBM* msbm, uint8 priority, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_LRB(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::LRB* lrb, uint8 priority, WarGrey::GYDM::Syslog* logger) {}

	protected:
		virtual void on_payload(int id, long long timepoint_ms, bool self, std::string& message, int pad_bits, WarGrey::GYDM::Syslog* logger);

	private:
		struct Sentences;
		std::map<int, WarGrey::DTPM::Transceiver::Sentences*> sentences;
	};
}
