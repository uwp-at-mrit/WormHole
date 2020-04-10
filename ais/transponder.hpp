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

	private class Transponder : public WarGrey::DTPM::INMEA0183Receiver {
	public:
		void on_message(int id, long long timepoint_ms,
			const unsigned char* pool, size_t head_start, size_t body_start, size_t endp1,
			WarGrey::GYDM::Syslog* logger) override;
		
	public:
		virtual void pre_interpret_payload(int id, WarGrey::GYDM::Syslog* logger) {}
		virtual void post_interpret_payload(int id, WarGrey::GYDM::Syslog* logger) {}

	public:
		virtual void on_PRCA(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::PRCA* prca, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_BSR(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BSR* bsr, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_SVD(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SVD* svd, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_UTCI(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::UTCI* utci, WarGrey::GYDM::Syslog* logger) {}
		
		virtual void on_BAM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BAM* bam, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_BA(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BA* ba, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_BBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BBM* bbc, WarGrey::GYDM::Syslog* logger) {}

		virtual void on_SRAPR(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SRAPR* srapr, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_ASRM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::ASRM* asrm, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_SRBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SRBM* srbm, WarGrey::GYDM::Syslog* logger) {}

		virtual void on_BSIQ(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::BSIQ* bsiq, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_AMC(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::AMC* amc, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_DGNSSBBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::DGNSSBBM* bbm, WarGrey::GYDM::Syslog* logger) {}

		virtual void on_PRCB(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::PRCB* cbcspr, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_PRCBE(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::PRCBE* cbepr, WarGrey::GYDM::Syslog* logger) {}
		
		virtual void on_DLM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::DLM* dlm, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_A2NR(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::A2NR* a2nr, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_CM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::CM* cm, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_GAC(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::GAC* gac, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_SDR(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SDR* sdr, WarGrey::GYDM::Syslog* logger) {}

		virtual void on_SSBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::SSBM* ssbm, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_MSBM(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::MSBM* msbm, WarGrey::GYDM::Syslog* logger) {}
		virtual void on_PR4LA(int id, long long timepoint_ms, bool self, uint16 mmsi, WarGrey::DTPM::PR4LA* pr4la, WarGrey::GYDM::Syslog* logger) {}

	protected:
		virtual void on_payload(int id, long long timepoint_ms, bool self, std::string& message, int pad_bits, WarGrey::GYDM::Syslog* logger);

	private:
		struct Sentences;
		std::map<int, WarGrey::DTPM::Transponder::Sentences*> sentences;
	};
}
