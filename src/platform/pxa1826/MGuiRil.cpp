#include <core/Logger.h>
#include <ui/StatusBar.h>
#include <ui/ToolButton.h>
#include <ui/HomeScreen.h>
#include "MGuiRil.h"

#define RIL_UBUS_ID		"ril"

namespace MGUI
{

D_DEBUG_DOMAIN(MGUI_RIL, "Mgui/Ril", "Ril");
D_DEBUG_DOMAIN(MGUI_OPERATOR, "Mgui/Ril/Operator", "Operator");
D_DEBUG_DOMAIN(MGUI_SIMCARD, "Mgui/Ril/Sim", "Simcard");
D_DEBUG_DOMAIN(MGUI_REGISTRATION, "Mgui/Ril/Registration", "Registration");
D_DEBUG_DOMAIN(MGUI_RSSI, "Mgui/Ril/SignalStrength", "Signal Strength");

/***************************************************************************/
/**		MGuiRil Class Implementation
/***************************************************************************/
const std::vector<std::string> MGuiRil:: _events({
	"ril.unsol.cc",
	"ril.unsol.dev",
	"ril.unsol.mm",
	"ril.unsol.msg",
	"ril.unsol.ps",
	"ril.unsol.sim",
	"ril.unsol.ss",
	"ril.unsol"
});

MGuiRil *MGuiRil:: _instance = 0;

static int ril_indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	MGuiRil *__this = MGuiRil::Instance();
	if (!__this) {
		ILOG_ERROR(MGUI_RIL, "No instance, indication ignored!\n");
		return -1;
	}
		
	return __this->Indication(ctx, obj, req, method, msg);
}

enum {
	REQID,
	ERRNO,
	DATA,
	_MAX
};

#undef blob_for_each_attr
#define blob_for_each_attr(pos, attr, rem) \
	for (rem = attr ? blob_len(attr) : 0, \
	     pos = attr ? (blob_attr*)blob_data(attr) : 0; \
	     rem > 0 && (blob_pad_len(pos) <= rem) && \
	     (blob_pad_len(pos) >= sizeof(struct blob_attr)); \
	     rem -= blob_pad_len(pos), pos = blob_next(pos))

int MGuiRil:: parse_blobmsg(struct blob_attr *attr, struct blob_attr **tb)
{
	blob_attr *pos = NULL;
	blobmsg_hdr *hdr = NULL;
	const char *policy[] = { "rilid", "resperrno" };
	int rem = 0;
	int i = 0;

	blob_for_each_attr(pos, attr, rem) {
		if (i > _MAX)
			return 0;

		hdr = (blobmsg_hdr *)blob_data(pos);
		if (i == 0) {
			if (strcmp(policy[i], (char *)hdr->name) != 0) {
				ILOG_ERROR(MGUI_RIL, "format error\n");
				return -1;
			}
		}

		tb[i] = pos;
		i++;
	}

	return 0;
}

int MGuiRil::Indication(struct ubus_context *ctx, struct ubus_object *obj,
			      struct ubus_request_data *req,
			      const char *method, struct blob_attr *msg)
{
	unsigned int id = 0;
	unsigned int errcode = 0;
	void *data = NULL;
	int len = 0;
	int ret = 0;
	blob_attr *tb[_MAX] = { 0 };

	ILOG_TRACE_F(MGUI_RIL);

	ret = parse_blobmsg(msg, tb);
	if (ret < 0) {
		ILOG_ERROR(MGUI_RIL, "parse msg error\n");
		return -1;
	}

	if (tb[REQID])
		id = blobmsg_get_u32(tb[REQID]);
	if (tb[ERRNO]) {
		errcode = blobmsg_get_u32(tb[ERRNO]);
		if (errcode) {
			ILOG_ERROR(MGUI_RIL, "unsolicited id %d blobmsg err %s\n",
				  id, errcode);
			return -1;
		}
	}
	if (tb[DATA]) {
		data = blob_data(tb[DATA]);
		len = blobmsg_data_len(tb[DATA]);
	}

	ILOG_DEBUG(MGUI_RIL, "id is %d, data = %p, len = %d\n", id, data, len);

	switch(id)
	{
		case RIL_UNSOL_SIGNAL_STRENGTH:
			ILOG_DEBUG(MGUI_RIL,"Signal Strength indication\n");
			SignalStrengthInd(data, len);
			break;
		case RIL_UNSOL_RESPONSE_SIM_STATUS_CHANGED:
			ILOG_DEBUG(MGUI_RIL,"Sim state changed\n");
		case RIL_UNSOL_RESPONSE_RADIO_STATE_CHANGED:
			ILOG_DEBUG(MGUI_RIL,"Radio state changed\n");
		case RIL_UNSOL_RESPONSE_VOICE_NETWORK_STATE_CHANGED:
			ILOG_DEBUG(MGUI_RIL,"Voice registration status changed\n");
		case RIL_UNSOL_VOICE_RADIO_TECH_CHANGED:
			ILOG_DEBUG(MGUI_RIL,"Radio technology changed\n");
			SendRequests();
			break;
		default:
			ILOG_DEBUG(MGUI_RIL, "id of %d is not supported\n",id);
			ret = 0;
	}

	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
	return 0;
}

MGuiRil* MGuiRil::Instance()
{
	return _instance;
}

int MGuiRil::Create(ubus_context *ubus, StatusBar *bar, HomeScreen *home)
{
	int ret;

	ILOG_TRACE_F(MGUI_RIL);
	if (_instance) {
		ILOG_ERROR(MGUI_RIL, "Already created\n");
		return -1;
	}

	_instance = new MGuiRil(ubus, bar, home);
	ret = _instance->Register();
	if (ret) {
		ILOG_ERROR(MGUI_RIL, "Register failed (ret=%d)\n", ret);
		goto out_error;
	}

	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
	return 0;

out_error:
	delete _instance;
	_instance = NULL;
}


void MGuiRil::Destroy()
{
	ILOG_TRACE_F(MGUI_RIL);

	if (_instance) {
		_instance->UnRegister();
		delete _instance->_operator;
		delete _instance->_simcard;
		delete _instance->_registration;
		delete _instance->_screen;
		delete _instance;
		_instance = NULL;
	}
}

MGuiRil::MGuiRil(ubus_context *ubus, StatusBar *bar, HomeScreen *home)
	: UBusClient(ubus),
	_bar(bar),
	_home(home)
{
}

MGuiRil::~MGuiRil()
{
}

void MGuiRil::SendRequests()
{
	ILOG_TRACE(MGUI_RIL);
	if (_instance) {
		_simcard->Request();
		_operator->Request();
		_registration->Request();
		_screen->Request(1);
	}
	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
}

int MGuiRil::Register()
{
	int ret;

	ILOG_TRACE(MGUI_RIL);

	ret = UBusClient::Register(ril_indication, RIL_UBUS_ID);
	if (ret) {
		ILOG_ERROR(MGUI_RIL, "Register failed %d\n", ret);
		return ret;
	}

	ret = UBusClient::Subscribe(_events);
	if (ret) {
		ILOG_ERROR(MGUI_RIL, "Subscribe events failed %d\n", ret);
		goto unregister;
	}

	_operator = new RilOperator(_subscriber, _id, _ubus);
	_simcard = new RilSimcard(_subscriber, _id, _ubus);
	_registration = new RilRegistration(_subscriber, _id, _ubus);
	_screen = new RilScreen(_subscriber, _id, _ubus);

	SendRequests();

	ILOG_DEBUG(MGUI_RIL, "%s exit\n", __FUNCTION__);
	return 0;

unregister:
	UBusClient::UnRegister();
	return ret;
}

int MGuiRil::UnRegister()
{
	ILOG_TRACE(MGUI_RIL);
	_screen->Request(0);
	return UBusClient::UnRegister();
}

int MGuiRil::OperatorCallback(blob_attr *msg)
{
	unsigned int requestid;
	unsigned int rilerrno = 0;
	void *data = NULL;
	int datalen = 0;
	int ret = 0;

	ILOG_TRACE(MGUI_OPERATOR);

	ret = rilutil_parseResponse(msg, &requestid, &rilerrno, &data, &datalen);
	rilutilstrings *resp = (rilutilstrings *)data;

	if (ret || rilerrno) {
		ILOG_ERROR(MGUI_OPERATOR, "RIL ERROR: ret=%d, requestid=%d, rilerrno=%d\n",
			  ret, requestid, rilerrno);
		return ret;
	}

	if (!resp || resp->num != 3) {
		ILOG_ERROR(MGUI_OPERATOR, "No operator found\n");
		_home->SetCellularInfo("Operator Unknown");
	} else {
		/* operator present */
		ILOG_DEBUG(MGUI_OPERATOR, "Operator = %s\n", resp->str[0]);
		_home->SetCellularInfo(resp->str[0]);
	}

	if (data)
		free(data);

	ILOG_DEBUG(MGUI_OPERATOR, "%s:%d: Exit\n", __FUNCTION__, __LINE__);
	return 0;
}

int MGuiRil::SimCallback(blob_attr *msg)
{
	unsigned int requestid;
	unsigned int rilerrno = 0;
	void *data = NULL;
	int datalen = 0;
	int ret = 0;

	ILOG_TRACE(MGUI_SIMCARD);

	ret = rilutil_parseResponse(msg, &requestid, &rilerrno, &data, &datalen);
	if (ret || rilerrno || !data) {
		ILOG_ERROR(MGUI_SIMCARD, "RIL ERROR: ret=%d, requestid=%d, rilerrno=%d, data=%p\n",
			  ret, requestid, rilerrno, data);
		return ret;
	}

	RIL_CardState SimStatus = ((RIL_CardStatus_v6 *)data)->card_state;

	switch (SimStatus) {
	case RIL_CARDSTATE_ABSENT:
		_bar->setSimState(SimMissing);
		ILOG_DEBUG(MGUI_SIMCARD, "Simcard absent\n");
		break;
	case RIL_CARDSTATE_PRESENT:
		_bar->setSimState(SimActive);
		ILOG_DEBUG(MGUI_SIMCARD, "Simcard present\n");
		break;
	case RIL_CARDSTATE_ERROR:
		_bar->setSimState(SimUnknown);
		ILOG_DEBUG(MGUI_SIMCARD, "Simcard error\n");
		break;
	}

	free(data);

	ILOG_DEBUG(MGUI_SIMCARD, "%s:%d: Exit\n", __FUNCTION__, __LINE__);
	return 0;
}

int MGuiRil::RegistrationCallback(blob_attr *msg)
{
	unsigned int requestid;
	unsigned int rilerrno = 0;
	void *data = NULL;
	int datalen = 0;
	int ret = 0;
	RegState *result;

	ILOG_TRACE(MGUI_REGISTRATION);

	ret = rilutil_parseResponse(msg, &requestid, &rilerrno, &data, &datalen);
	rilutilstrings *resp = (rilutilstrings *)data;

	if (ret || rilerrno || !data) {
		ILOG_ERROR(MGUI_SIMCARD, "RIL ERROR: ret=%d, requestid=%d, rilerrno=%d, data=%p\n",
			  ret, requestid, rilerrno, data);
		return ret;
	}

	if (resp->num < 2) {
		ILOG_ERROR(MGUI_REGISTRATION, "wrong resp->num=%d\n", resp->num);
		return -1;
	}

	result = (requestid == RIL_REQUEST_VOICE_REGISTRATION_STATE) ? &_voice : &_data;

	sscanf(resp->str[0], "%d", &result->reg_state);
	if (resp->num >= 5)
		sscanf(resp->str[3], "%d", (int *)&result->radio_tech);

	if (data)
		free(data);

	if (requestid == RIL_REQUEST_VOICE_REGISTRATION_STATE) {
		ILOG_DEBUG(MGUI_REGISTRATION, "Voice: %s registered\n", registered_voice() ? "" : "not");
		_bar->setCellularState(Cellular0NoData);
	}

	if (requestid == RIL_REQUEST_DATA_REGISTRATION_STATE) {
		ILOG_DEBUG(MGUI_REGISTRATION, "Data: %s registered\n", registered_data() ? "" : "not");
		_bar->setCellularState(Cellular0);
	}

	_bar->setCellularTechState(tech());

	ILOG_DEBUG(MGUI_REGISTRATION, "Radio Technology: %s\n", tech_to_str());
	ILOG_DEBUG(MGUI_REGISTRATION, "%s:%d: Exit\n", __FUNCTION__, __LINE__);

	return 0;
}

bool MGuiRil::registered_voice()
{
	return (_voice.reg_state == 1 || _voice.reg_state == 5);
}

bool MGuiRil::registered_data()
{
	return (_data.reg_state == 1 || _data.reg_state == 5);
}

CellularTechState
MGuiRil::tech()
{
	switch (_voice.radio_tech) {
	case RADIO_TECH_GPRS:	return CellularTech2G; /* 2G */
	case RADIO_TECH_EDGE:	return CellularTechEdge; /* E */
	case RADIO_TECH_UMTS:	return CellularTech3G; /* 3G */
	case RADIO_TECH_HSDPA:
	case RADIO_TECH_HSUPA:
	case RADIO_TECH_HSPAP:	return CellularTechHSDPAP; /* H */
	case RADIO_TECH_HSPA:	return CellularTechHSDPA; /* H+ */
	case RADIO_TECH_LTE:	return CellularTech4G; /* 4G */
	case RADIO_TECH_GSM:	return CellularTechG; /* G */
	default:		return CellularTechUnknown; /* ? */
	}
}

const char *
MGuiRil::tech_to_str()
{
	
	switch (_voice.radio_tech) {
	case RADIO_TECH_GPRS:	return "2G";
	case RADIO_TECH_EDGE:	return "EDGE";
	case RADIO_TECH_UMTS:	return "3G";
	case RADIO_TECH_HSDPA:
	case RADIO_TECH_HSUPA:
	case RADIO_TECH_HSPAP:	return "H+";
	case RADIO_TECH_HSPA:	return "H";
	case RADIO_TECH_LTE:	return "4G";
	case RADIO_TECH_GSM:	return "GSM";
	default:		return "Unknown";
	}
}

int MGuiRil::convertDbmToRssi(int rsrp)
{
	if(rsrp == INT_MAX)
		_rssi = 99;
	else if(rsrp < -113)
		_rssi = 0;
	else if(rsrp > -51)
		_rssi =31;
	else
		_rssi = (113 + rsrp) / 2;
}

void MGuiRil::SignalStrengthInd(void *data, int len)
{
	ILOG_TRACE(MGUI_RSSI);

	RIL_SignalStrength_v6 *rssi = (RIL_SignalStrength_v6 *)data;
	if (tech() == CellularTech4G) {
		ILOG_DEBUG(MGUI_RSSI, "in LTE, using rsrp == %d as rssi value\n",
			  rssi->LTE_SignalStrength.rsrp);
		convertDbmToRssi(rssi->LTE_SignalStrength.rsrp);
	} else {
		ILOG_DEBUG(MGUI_RSSI, "Not LTE, rssi = %d\n",
			   rssi->GW_SignalStrength.signalStrength);
		_rssi = rssi->GW_SignalStrength.signalStrength;
	}

	ILOG_DEBUG(MGUI_RSSI, "rssi value changed to %d\n", _rssi);
	if (!registered_voice()) {
		ILOG_DEBUG(MGUI_RSSI, "no voice, no data\n");
		_bar->setCellularState(CellularNull);
	} else if (!registered_data()) {
		ILOG_DEBUG(MGUI_RSSI, "voice, no data\n");
		if (_rssi >= 18)
			_bar->setCellularState(Cellular4NoData);
		else if (_rssi >= 13)
			_bar->setCellularState(Cellular3NoData);
		else if (_rssi >= 10)
			_bar->setCellularState(Cellular2NoData);
		else if (_rssi >= 5)
			_bar->setCellularState(Cellular1NoData);
		else
			_bar->setCellularState(Cellular0NoData);
	} else {
		ILOG_DEBUG(MGUI_RSSI, "voice and data\n");
		if (_rssi >= 18)
			_bar->setCellularState(Cellular4);
		else if (_rssi >= 13)
			_bar->setCellularState(Cellular3);
		else if (_rssi >= 10)
			_bar->setCellularState(Cellular2);
		else if (_rssi >= 5)
			_bar->setCellularState(Cellular1);
		else
			_bar->setCellularState(Cellular0);
	}
	
	ILOG_DEBUG(MGUI_RSSI, "%s:%d: Exit\n", __FUNCTION__, __LINE__);
}

};
