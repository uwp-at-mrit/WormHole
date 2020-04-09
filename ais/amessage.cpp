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
void WarGrey::DTPM::ais_extract_prca(PRCA* prca, Natural& payload) {
	prca->status = ais_e_ref(payload, 38, 4, AISNavigation::Undefined);
	prca->turn.box(ais_i_ref(payload, 42, 8));
	prca->speed.box(ais_u_ref(payload, 50, 10));
	prca->accuracy = ais_b_ref(payload, 60);
	prca->longitude.box(ais_i_ref(payload, 61, 28));
	prca->latitude.box(ais_i_ref(payload, 89, 27));
	prca->course.box(ais_u_ref(payload, 116, 12));
	prca->heading = ais_u_ref(payload, 128, 9);
	prca->timetamp = ais_u_ref(payload, 137, 6);
	prca->maneuver = ais_e_ref(payload, 143, 2, AISManeuver::Unavailable);
	prca->raim = ais_b_ref(payload, 144);
	prca->radio = ais_u_ref(payload, 149, 19);
}

void WarGrey::DTPM::ais_extract_bsr(BSR* bsr, Natural& payload) {
	bsr->year = ais_u_ref(payload, 38, 14);
	bsr->month = ais_u_ref(payload, 52, 4);
	bsr->day = ais_u_ref(payload, 56, 5);
	bsr->hour = ais_u_ref(payload, 61, 5);
	bsr->minute = ais_u_ref(payload, 66, 6);
	bsr->second = ais_u_ref(payload, 72, 6);
	bsr->accuracy = ais_b_ref(payload, 78);
	bsr->longitude.box(ais_i_ref(payload, 79, 28));
	bsr->latitude.box(ais_i_ref(payload, 107, 27));
	bsr->epfd = ais_e_ref(payload, 134, 4, AISPositionFixType::Undefined);
	bsr->raim = ais_b_ref(payload, 148);
	bsr->radio = ais_u_ref(payload, 149, 19);
}

void WarGrey::DTPM::ais_extract_svd(SVD* svd, Natural& payload) {
	svd->ais_version = ais_u_ref(payload, 38, 2);
	svd->imo = ais_u_ref(payload, 40, 30);
	svd->callsign = ais_t_ref(payload, 70, 42);
	svd->shipname = ais_t_ref(payload, 112, 120);
	svd->shiptype = ais_e_ref(payload, 232, 8, AISShipType::Unavailable);
	svd->bow_dimension = ais_u_ref(payload, 240, 9);
	svd->stern_dimension = ais_u_ref(payload, 249, 9);
	svd->port_dimension = ais_u_ref(payload, 258, 6);
	svd->starboard_dimension = ais_u_ref(payload, 264, 6);
	svd->epfd = ais_e_ref(payload, 270, 4, AISPositionFixType::Undefined);
	svd->month = ais_u_ref(payload, 274, 4);
	svd->day = ais_u_ref(payload, 278, 5);
	svd->hour = ais_u_ref(payload, 283, 5);
	svd->minute = ais_u_ref(payload, 288, 6);
	svd->draught.box(ais_u_ref(payload, 294, 8));
	svd->destination = ais_t_ref(payload, 302, 120);
	svd->dte = ais_b_ref(payload, 422);
}

void WarGrey::DTPM::ais_extract_prcb(PRCB* prcb, Natural& payload) {
	prcb->speed.box(ais_u_ref(payload, 46, 10));
	prcb->accuracy = ais_b_ref(payload, 56);
	prcb->longitude.box(ais_i_ref(payload, 57, 28));
	prcb->latitude.box(ais_i_ref(payload, 85, 27));
	prcb->course.box(ais_u_ref(payload, 112, 12));
	prcb->heading = ais_u_ref(payload, 124, 9);
	prcb->timetamp = ais_u_ref(payload, 133, 6);
	prcb->regional = ais_u_ref(payload, 139, 2);
	prcb->cs = ais_b_ref(payload, 141);
	prcb->display = ais_b_ref(payload, 142);
	prcb->dsc = ais_b_ref(payload, 143);
	prcb->band = ais_b_ref(payload, 144);
	prcb->msg22 = ais_b_ref(payload, 145);
	prcb->assigned = ais_b_ref(payload, 146);
	prcb->raim = ais_b_ref(payload, 147);
	prcb->radio = ais_u_ref(payload, 148, 20);
}

void WarGrey::DTPM::ais_extract_prcbe(PRCBE* prcbe, Natural& payload) {
	prcbe->speed.box(ais_u_ref(payload, 46, 10));
	prcbe->accuracy = ais_b_ref(payload, 56);
	prcbe->longitude.box(ais_i_ref(payload, 57, 28));
	prcbe->latitude.box(ais_i_ref(payload, 85, 27));
	prcbe->course.box(ais_u_ref(payload, 112, 12));
	prcbe->heading = ais_u_ref(payload, 124, 9);
	prcbe->timetamp = ais_u_ref(payload, 133, 6);
	prcbe->regional = ais_u_ref(payload, 139, 2);
	prcbe->shipname = ais_t_ref(payload, 143, 120);
	prcbe->shiptype = ais_e_ref(payload, 263, 8, AISShipType::Unavailable);
	prcbe->bow_dimension = ais_u_ref(payload, 271, 9);
	prcbe->stern_dimension = ais_u_ref(payload, 280, 9);
	prcbe->port_dimension = ais_u_ref(payload, 289, 6);
	prcbe->starboard_dimension = ais_u_ref(payload, 295, 6);
	prcbe->epfd = ais_e_ref(payload, 301, 4, AISPositionFixType::Undefined);
	prcbe->raim = ais_b_ref(payload, 305);
	prcbe->dte = ais_b_ref(payload, 306);
	prcbe->assigned = ais_b_ref(payload, 307);
}

void WarGrey::DTPM::ais_extract_dlm(DLM* dlm, Natural& payload) {
	size_t indice[] = { 40U, 70U, 100U, 130U };
	size_t total = payload.integer_length(6);
	size_t offset = indice[0] + 1U;
	size_t span = indice[1] - indice[0];

	dlm->slots_count = fxmin((total - offset) / span, sizeof(indice) / sizeof(size_t));
	for (size_t idx = 0; idx < dlm->slots_count; idx++) {
		dlm->offsets[idx] = ais_u_ref(payload, indice[idx], 12);
		dlm->numbers[idx] = ais_u_ref(payload, indice[idx] + 12, 4);
		dlm->timeouts[idx] = ais_u_ref(payload, indice[idx] + 16, 3);
		dlm->increments[idx] = ais_u_ref(payload, indice[idx] + 19, 11);
	}
}

void WarGrey::DTPM::ais_extract_a2nr(A2NR* a2nr, Natural& payload) {
	a2nr->aid_type = ais_e_ref(payload, 38, 5, AISAidType::_);
	a2nr->name = ais_t_ref(payload, 43, 120);
	a2nr->accuracy = ais_b_ref(payload, 163);
	a2nr->longitude.box(ais_i_ref(payload, 164, 28));
	a2nr->latitude.box(ais_i_ref(payload, 192, 27));
	a2nr->bow_dimension = ais_u_ref(payload, 219, 9);
	a2nr->stern_dimension = ais_u_ref(payload, 228, 9);
	a2nr->port_dimension = ais_u_ref(payload, 237, 6);
	a2nr->starboard_dimension = ais_u_ref(payload, 243, 6);
	a2nr->epfd = ais_e_ref(payload, 249, 4, AISPositionFixType::Undefined);
	a2nr->timetamp = ais_u_ref(payload, 253, 6);
	a2nr->off_position = ais_b_ref(payload, 259);
	a2nr->regional = ais_u_ref(payload, 260, 8);
	a2nr->raim = ais_b_ref(payload, 268);
	a2nr->virtual_aid = ais_b_ref(payload, 269);
	a2nr->assigned = ais_b_ref(payload, 270);
	a2nr->name_extension = ais_t_ref(payload, 272, 88);
}

void WarGrey::DTPM::ais_extract_sdr(SDR* sdr, Natural& payload) {
	sdr->partno = ais_u_ref(payload, 38, 2);

	switch (sdr->partno) {
	case 0: {
		sdr->part.a.shipname = ais_t_ref(payload, 40, 120);
	}; break;
	case 1: {
		sdr->part.b.shiptype = ais_e_ref(payload, 232, 8, AISShipType::Unavailable);
		sdr->part.b.vendorid = ais_t_ref(payload, 48, 18);
		sdr->part.b.model = ais_u_ref(payload, 66, 4);
		sdr->part.b.serial = ais_u_ref(payload, 70, 20);
		sdr->part.b.callsign = ais_u_ref(payload, 90, 42);

		{ // variable bitfields
			u mmsi = ais_mobile_marine_service_identifier(payload);

			if ((mmsi >= 980000000U) && (mmsi <= 990000000)) {
				sdr->part.b.craft.mothership_mmsi = ais_u_ref(payload, 132, 30);
			} else {
				sdr->part.b.craft.dimension.bow = ais_u_ref(payload, 132, 9);
				sdr->part.b.craft.dimension.stern = ais_u_ref(payload, 141, 9);
				sdr->part.b.craft.dimension.port = ais_u_ref(payload, 150, 6);
				sdr->part.b.craft.dimension.starboard = ais_u_ref(payload, 156, 6);
			}
		}
	}; break;
	}
}
