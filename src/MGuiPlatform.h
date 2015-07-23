#ifndef MGUI_PLATFORM_H_
#define MGUI_PLATFORM_H_

namespace MGUI
{

enum PlatformRequest
{
    SdcardStatusReq,
    BatteryStatusReq,
    BluetoothStatusReq,
    WifiStatusReq,
    WifiInfoReq,
    CellularSimStatusReq,
    CellularOperatorReq,
    CellularVoiceRegStatusReq,
    CellularDataRegStatusReq,
    HawkResetReq,
    HawkNoDataAssertReq,
    HawkFotaReq,
    HawkKeepAliveReq,
};

enum PlatformEvent
{
    OnkeyEvent,
    BatteryEvent,
    BluetoothEvent,
    WifiEvent,
    CellularEvent,
    GpsEvent,
    HawkEvent,
};

enum BatteryState
{
    BatteryUnknown,
    BatteryAlert,
    Battery100,
    Battery90,
    Battery80,
    Battery60,
    Battery50,
    Battery30,
    Battery20,
    BatteryCharging100,
    BatteryCharging90,
    BatteryCharging80,
    BatteryCharging60,
    BatteryCharging50,
    BatteryCharging30,
    BatteryCharging20,
};

enum BluetoothState
{
    BluetoothOff,
    BluetoothSearching,
    BluetoothConnected,
};

enum WifiIndications
{
    WifiInfoInd,
    WifiStatusInd,
    WifiSignalStrengthInd,
};

enum CellularIndications
{
    CellularOperatorInd,
    CellularVoiceRegInd,
    CellularDataRegInd, /* includes cellular technology */
    CellularSimStatusInd,
    CellularSignalStrengthInd,
};

enum GpsState
{
    GpsOff,
    GpsSearching,
    GpsConnected,
};

enum SimState
{
    SimUnknown,
    SimActive,
    SimMissing
};

enum SDCardState
{
    SDCardUnknown,
    SDCardActive,
    SDCardMissing
};

enum WifiState
{
    WifiUnknown,
    WifiOff,
    WifiTethering,
    Wifi0Bar,
    Wifi1Bar,
    Wifi1BarLock,
    Wifi2Bar,
    Wifi2BarLock,
    Wifi3Bar,
    Wifi3BarLock,
    Wifi4Bar,
    Wifi4BarLock,
};

enum CellularState
{
    CellularNull,
    CellularOff,
    Cellular0,
    Cellular0NoData,
    Cellular1,
    Cellular1NoData,
    Cellular2,
    Cellular2NoData,
    Cellular3,
    Cellular3NoData,
    Cellular4,
    Cellular4NoData
};

enum CellularTechState
{
    CellularTechUnknown,
    CellularTechHSDPAP,
    CellularTechHSDPA,
    CellularTechEdge,
    CellularTechG,
    CellularTech2G,
    CellularTech3G,
    CellularTech4G
};

} /* namespace MGUI */
#endif /* MGUI_PLATFORM_H_ */
