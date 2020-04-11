#include "ais/amessage.hpp"
#include "ais/abitfields.hpp"

#include "gps/gparser.hpp"

#include "datum/enum.hpp"
#include "datum/fixnum.hpp"

using namespace WarGrey::DTPM;
using namespace WarGrey::GYDM;
using namespace WarGrey::SCADA;

/*************************************************************************************************/
void WarGrey::DTPM::scan_ainmea(AINMEA* nmea, const unsigned char* pool, size_t* cursor, size_t endp1) {
	nmea->s_size = int(scan_natural(pool, cursor, endp1));
	nmea->s_idx = int(scan_natural(pool, cursor, endp1));
	nmea->msg_id = int(scan_natural(pool, cursor, endp1));
	nmea->channel = scan_char(pool, cursor, endp1, '\0');
	nmea->payload = scan_text(pool, cursor, endp1);
	nmea->pad_bits = int(scan_natural(pool, cursor, endp1));
}

/*************************************************************************************************/
AISMessage WarGrey::DTPM::ais_message_type(Natural& payload) {
	return ais_e_ref(payload, 0, 6, AISMessage::_, -1);
}

uint8 WarGrey::DTPM::ais_repeat_indicator(Natural& payload) {
	return static_cast<uint8>(ais_u_ref(payload, 6, 2));
}

uint16 WarGrey::DTPM::ais_mobile_marine_service_identifier(Natural& payload) {
	return static_cast<uint16>(ais_u_ref(payload, 8, 30));
}

/*************************************************************************************************/
ASO::ASO(Natural& payload) {
	this->status = ais_e_ref(payload, 38, 4, AISNavigation::Undefined);
	this->turn.box(ais_i_ref(payload, 42, 8));
	this->speed.box(ais_u_ref(payload, 50, 10));
	this->accuracy = ais_b_ref(payload, 60);
	this->longitude.box(ais_i_ref(payload, 61, 28));
	this->latitude.box(ais_i_ref(payload, 89, 27));
	this->course.box(ais_u_ref(payload, 116, 12));
	this->heading = ais_u_ref(payload, 128, 9);
	this->timetamp = ais_u_ref(payload, 137, 6);
	this->maneuver = ais_e_ref(payload, 143, 2, AISManeuver::Unavailable);
	this->raim = ais_b_ref(payload, 144);
	this->radio = ais_u_ref(payload, 149, 19);
}

BSR::BSR(Natural& payload) {
	this->year = ais_u_ref(payload, 38, 14);
	this->month = ais_u_ref(payload, 52, 4);
	this->day = ais_u_ref(payload, 56, 5);
	this->hour = ais_u_ref(payload, 61, 5);
	this->minute = ais_u_ref(payload, 66, 6);
	this->second = ais_u_ref(payload, 72, 6);
	this->accuracy = ais_b_ref(payload, 78);
	this->longitude.box(ais_i_ref(payload, 79, 28));
	this->latitude.box(ais_i_ref(payload, 107, 27));
	this->epfd = ais_e_ref(payload, 134, 4, AISPositionFixType::Undefined);
	this->raim = ais_b_ref(payload, 148);
	this->radio = ais_u_ref(payload, 149, 19);
}

SVD::SVD(Natural& payload) {
	this->ais_version = ais_u_ref(payload, 38, 2);
	this->imo = ais_u_ref(payload, 40, 30);
	this->callsign = ais_t_ref(payload, 70, 42);
	this->shipname = ais_t_ref(payload, 112, 120);
	this->shiptype = ais_e_ref(payload, 232, 8, AISShipType::Unavailable);
	this->shipbox.bow = ais_u_ref(payload, 240, 9);
	this->shipbox.stern = ais_u_ref(payload, 249, 9);
	this->shipbox.port = ais_u_ref(payload, 258, 6);
	this->shipbox.starboard = ais_u_ref(payload, 264, 6);
	this->epfd = ais_e_ref(payload, 270, 4, AISPositionFixType::Undefined);
	this->month = ais_u_ref(payload, 274, 4);
	this->day = ais_u_ref(payload, 278, 5);
	this->hour = ais_u_ref(payload, 283, 5);
	this->minute = ais_u_ref(payload, 288, 6);
	this->draught.box(ais_u_ref(payload, 294, 8));
	this->destination = ais_t_ref(payload, 302, 120);
	this->dte = ais_b_ref(payload, 422);
}

UTCI::UTCI(Natural& payload) {
	this->dest_mmsi = ais_u_ref(payload, 40, 30);
}

BAM::BAM(Natural& payload) {
	this->seqno = ais_u_ref(payload, 38, 2);
	this->dest_mmsi = ais_u_ref(payload, 40, 30);
	this->retransmit = ais_b_ref(payload, 70);
	this->dac = ais_u_ref(payload, 72, 10);
	this->fid = ais_u_ref(payload, 82, 6);
}

BA::BA(Natural& payload) {
	size_t indice[] = { 40U, 72U, 104U, 136U };
	size_t total = payload.integer_length(6);
	size_t offset = indice[0] + 1U;
	size_t span = indice[1] - indice[0];

	this->slots_count = fxmin((total - offset) / span, sizeof(indice) / sizeof(size_t));

	for (size_t idx = 0; idx < this->slots_count; idx++) {
		this->mmsis[idx] = ais_u_ref(payload, indice[idx], 30);
		this->mmsiseqs[idx] = ais_u_ref(payload, indice[idx] + 30, 2);
	}
}

BBM::BBM(Natural& payload) {
	this->dac = ais_u_ref(payload, 40, 10);
	this->fid = ais_u_ref(payload, 50, 6);
}

ASRM::ASRM(Natural& payload) {
	this->seqno = ais_u_ref(payload, 38, 2);
	this->dest_mmsi = ais_u_ref(payload, 40, 30);
	this->retransmit = ais_b_ref(payload, 70);
}

SRBM::SRBM(Natural& payload) {
}

BSIQ::BSIQ(Natural& payload) {
	this->mmsi1 = ais_u_ref(payload, 40, 30);
	this->type11 = ais_u_ref(payload, 70, 6);
	this->offset11 = ais_u_ref(payload, 76, 12);
	this->type12 = ais_u_ref(payload, 90, 6);
	this->offset12 = ais_u_ref(payload, 96, 12);
	this->mmsi2 = ais_u_ref(payload, 110, 30);
	this->type21 = ais_u_ref(payload, 140, 6);
	this->offset21 = ais_u_ref(payload, 146, 12);
}

AMC::AMC(Natural& payload) {
	size_t indice[] = { 40U, 92U };
	size_t total = payload.integer_length(6);
	size_t offset = indice[0] + 1U;
	size_t span = indice[1] - indice[0];

	this->slots_count = fxmin((total - offset) / span, sizeof(indice) / sizeof(size_t));

	for (size_t idx = 0; idx < this->slots_count; idx++) {
		this->mmsis[idx] = ais_u_ref(payload, indice[idx], 30);
		this->offsets[idx] = ais_u_ref(payload, indice[idx] + 30, 12);
		this->increments[idx] = ais_u_ref(payload, indice[idx] + 42, 10);
	}
}

GBBM::GBBM(Natural& payload) {
	this->longitude.box(ais_i_ref(payload, 40, 18));
	this->latitude.box(ais_i_ref(payload, 58, 17));
}

SRAPR::SRAPR(Natural& payload) {
	this->altitude = ais_u_ref(payload, 38, 12);
	this->speed = ais_u_ref(payload, 50, 10);
	this->accuracy = ais_b_ref(payload, 60);
	this->longitude.box(ais_i_ref(payload, 61, 28));
	this->latitude.box(ais_i_ref(payload, 89, 27));
	this->course.box(ais_u_ref(payload, 116, 12));
	this->timetamp = ais_u_ref(payload, 128, 6);
	this->dte = ais_b_ref(payload, 142);
	this->assigned = ais_b_ref(payload, 146);
	this->raim = ais_b_ref(payload, 147);
	this->radio = ais_u_ref(payload, 148, 20);
}

BCS::BCS(Natural& payload) {
	this->speed.box(ais_u_ref(payload, 46, 10));
	this->accuracy = ais_b_ref(payload, 56);
	this->longitude.box(ais_i_ref(payload, 57, 28));
	this->latitude.box(ais_i_ref(payload, 85, 27));
	this->course.box(ais_u_ref(payload, 112, 12));
	this->heading = ais_u_ref(payload, 124, 9);
	this->timetamp = ais_u_ref(payload, 133, 6);
	this->regional = ais_u_ref(payload, 139, 2);
	this->cs = ais_b_ref(payload, 141);
	this->display = ais_b_ref(payload, 142);
	this->dsc = ais_b_ref(payload, 143);
	this->band = ais_b_ref(payload, 144);
	this->msg22 = ais_b_ref(payload, 145);
	this->assigned = ais_b_ref(payload, 146);
	this->raim = ais_b_ref(payload, 147);
	this->radio = ais_u_ref(payload, 148, 20);
}

BCSE::BCSE(Natural& payload) {
	this->speed.box(ais_u_ref(payload, 46, 10));
	this->accuracy = ais_b_ref(payload, 56);
	this->longitude.box(ais_i_ref(payload, 57, 28));
	this->latitude.box(ais_i_ref(payload, 85, 27));
	this->course.box(ais_u_ref(payload, 112, 12));
	this->heading = ais_u_ref(payload, 124, 9);
	this->timetamp = ais_u_ref(payload, 133, 6);
	this->regional = ais_u_ref(payload, 139, 2);
	this->shipname = ais_t_ref(payload, 143, 120);
	this->shiptype = ais_e_ref(payload, 263, 8, AISShipType::Unavailable);
	this->shipbox.bow = ais_u_ref(payload, 271, 9);
	this->shipbox.stern = ais_u_ref(payload, 280, 9);
	this->shipbox.port = ais_u_ref(payload, 289, 6);
	this->shipbox.starboard = ais_u_ref(payload, 295, 6);
	this->epfd = ais_e_ref(payload, 301, 4, AISPositionFixType::Undefined);
	this->raim = ais_b_ref(payload, 305);
	this->dte = ais_b_ref(payload, 306);
	this->assigned = ais_b_ref(payload, 307);
}

DLM::DLM(Natural& payload) {
	size_t indice[] = { 40U, 70U, 100U, 130U };
	size_t total = payload.integer_length(6);
	size_t offset = indice[0] + 1U;
	size_t span = indice[1] - indice[0];

	this->slots_count = fxmin((total - offset) / span, sizeof(indice) / sizeof(size_t));

	for (size_t idx = 0; idx < this->slots_count; idx++) {
		this->offsets[idx] = ais_u_ref(payload, indice[idx], 12);
		this->numbers[idx] = ais_u_ref(payload, indice[idx] + 12, 4);
		this->timeouts[idx] = ais_u_ref(payload, indice[idx] + 16, 3);
		this->increments[idx] = ais_u_ref(payload, indice[idx] + 19, 11);
	}
}

ATON::ATON(Natural& payload) {
	this->aid_type = ais_e_ref(payload, 38, 5, AISAidType::_);
	this->name = ais_t_ref(payload, 43, 120);
	this->accuracy = ais_b_ref(payload, 163);
	this->longitude.box(ais_i_ref(payload, 164, 28));
	this->latitude.box(ais_i_ref(payload, 192, 27));
	this->shipbox.bow = ais_u_ref(payload, 219, 9);
	this->shipbox.stern = ais_u_ref(payload, 228, 9);
	this->shipbox.port = ais_u_ref(payload, 237, 6);
	this->shipbox.starboard = ais_u_ref(payload, 243, 6);
	this->epfd = ais_e_ref(payload, 249, 4, AISPositionFixType::Undefined);
	this->timetamp = ais_u_ref(payload, 253, 6);
	this->off_position = ais_b_ref(payload, 259);
	this->regional = ais_u_ref(payload, 260, 8);
	this->raim = ais_b_ref(payload, 268);
	this->virtual_aid = ais_b_ref(payload, 269);
	this->assigned = ais_b_ref(payload, 270);
	this->name_extension = ais_t_ref(payload, 272, 88);
}

GAC::GAC(Natural& payload) {
	this->area.ne_longitude.box(ais_i_ref(payload, 40, 18));
	this->area.ne_latitude.box(ais_i_ref(payload, 58, 17));
	this->area.sw_longitude.box(ais_i_ref(payload, 75, 18));
	this->area.sw_latitude.box(ais_i_ref(payload, 93, 17));
	this->station_type = ais_e_ref(payload, 110, 4, AISStationType::All);
	this->ship_type = ais_e_ref(payload, 114, 8, AISShipType::Unavailable);
	this->txrx = ais_e_ref(payload, 144, 2, AISTRMode::TxATxB);
	this->interval = ais_e_ref(payload, 146, 4, AISStationInterval::Auto);
}

/*************************************************************************************************/
CM::CM(Natural& payload) {
	this->channel_a = ais_u_ref(payload, 40, 12);
	this->channel_b = ais_u_ref(payload, 52, 12);
	this->txrx = ais_e_ref(payload, 64, 4, AISTRMode::TxATxB);
	this->power = ais_b_ref(payload, 68);
	
	this->addressed = ais_b_ref(payload, 139);

	if (this->addressed) {
		new (&this->cast.target) CM::Target();

		this->cast.target.dest1 = ais_u_ref(payload, 69, 30);
		this->cast.target.dest1 = ais_u_ref(payload, 104, 30);
	} else {
		new (&this->cast.area) AISArea();

		this->cast.area.ne_longitude.box(ais_i_ref(payload, 69, 18));
		this->cast.area.ne_latitude.box(ais_i_ref(payload, 87, 17));
		this->cast.area.sw_longitude.box(ais_i_ref(payload, 104, 18));
		this->cast.area.sw_latitude.box(ais_i_ref(payload, 122, 17));
	}

	this->band_a = ais_b_ref(payload, 140);
	this->band_b = ais_b_ref(payload, 141);
	this->zonesize = ais_u_ref(payload, 142, 3);
}

CM::~CM() {
	if (this->addressed) {
		this->cast.target.~Target();
	} else {
		this->cast.area.~AISArea();
	}
}

/*************************************************************************************************/
SDR::SDR(Natural& payload) {
	this->partno = ais_e_ref(payload, 38, 2, SDR::Format::_);

	switch (this->partno) {
	case SDR::Format::PartA: {
		new (&this->part.a) SDR::A();
		
		this->part.a.shipname = ais_t_ref(payload, 40, 120);
	}; break;
	case SDR::Format::PartB: {
		new (&this->part.b) SDR::B();

		this->part.b.shiptype = ais_e_ref(payload, 232, 8, AISShipType::Unavailable);
		this->part.b.vendorid = ais_t_ref(payload, 48, 18);
		this->part.b.model = ais_u_ref(payload, 66, 4);
		this->part.b.serial = ais_u_ref(payload, 70, 20);
		this->part.b.callsign = ais_u_ref(payload, 90, 42);

		{ // variable bitfields
			u mmsi = ais_mobile_marine_service_identifier(payload);

			this->part.b.auxiliary = ((mmsi >= 980000000U) && (mmsi <= 990000000));
		
			if (this->part.b.auxiliary) {
				this->part.b.craft.mothership_mmsi = ais_u_ref(payload, 132, 30);
			} else {
				this->part.b.craft.box.bow = ais_u_ref(payload, 132, 9);
				this->part.b.craft.box.stern = ais_u_ref(payload, 141, 9);
				this->part.b.craft.box.port = ais_u_ref(payload, 150, 6);
				this->part.b.craft.box.starboard = ais_u_ref(payload, 156, 6);
			}
		}
	}; break;
	}
}

SDR::~SDR() {
	switch (this->partno) {
	case SDR::Format::PartA: this->part.a.~A(); break;
	case SDR::Format::PartB: this->part.b.~B(); break;
	}
}

/*************************************************************************************************/
SSBM::SSBM(Natural& payload) {
	this->addressed = ais_b_ref(payload, 38);
	this->structured = ais_b_ref(payload, 39);
}

MSBM::MSBM(Natural& payload) {
	this->addressed = ais_b_ref(payload, 38);
	this->structured = ais_b_ref(payload, 39);
}

LRB::LRB(Natural& payload) {
	this->accuracy = ais_b_ref(payload, 38);
	this->raim = ais_b_ref(payload, 39);
	this->status = ais_e_ref(payload, 40, 4, AISNavigation::Undefined);
	this->longitude.box(ais_i_ref(payload, 44, 28));
	this->latitude.box(ais_i_ref(payload, 62, 27));
	this->speed = ais_u_ref(payload, 79, 6);
	this->course = ais_u_ref(payload, 85, 9);
	this->gnss = !ais_b_ref(payload, 94);
}
