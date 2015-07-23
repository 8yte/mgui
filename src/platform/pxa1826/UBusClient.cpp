#include <core/Logger.h>
#include "UBusClient.h"

namespace MGUI
{

D_DEBUG_DOMAIN(UBUS_CLIENT, "Ubus/Client", "Client");

UBusClient::UBusClient(ubus_context* ubus)
    : _ubus(ubus)
{
}

UBusClient::~UBusClient()
{
}

int
UBusClient::Register(ubus_handler_t cb, const char *name)
{
    int ret;

    ILOG_TRACE(UBUS_CLIENT);

    ret = ubus_lookup_id(_ubus, name, &_id);
    if (ret) {
        ILOG_ERROR(UBUS_CLIENT, "lookup id %s failed\n", name);
        return ret;
    }
    _subscriber.cb = cb;

    ret = ubus_register_subscriber(_ubus, &_subscriber);
    if (ret) {
        ILOG_ERROR(UBUS_CLIENT, "register subscriber failed\n");
        return ret;
    }
    ILOG_DEBUG(UBUS_CLIENT, "%s exit\n", __func__);

    return 0;
}

int
UBusClient::UnRegister()
{
    int ret;

    ILOG_TRACE(UBUS_CLIENT);

    ret = ubus_unregister_subscriber(_ubus, &_subscriber);
    if (ret) {
        ILOG_ERROR(UBUS_CLIENT, "unregister subscriber failed\n");
        return ret;
    }
    _subscriber.cb = NULL;
    ILOG_DEBUG(UBUS_CLIENT, "%s exit\n", __func__);

    return 0;
}

int
UBusClient::Subscribe(const std::string &event)
{
    int ret;
    uint32_t id;

    ILOG_TRACE(UBUS_CLIENT);

    ret = ubus_lookup_id(_ubus, event.c_str(), &id);
    if (ret) {
        ILOG_ERROR(UBUS_CLIENT, "lookup id %s failed\n", event.c_str());
        return ret;
    }


    ret = ubus_subscribe(_ubus, &_subscriber, id);
    if (ret) {
        ILOG_ERROR(UBUS_CLIENT,"event %s subscribe failed (err: %s)\n",
                   event.c_str(), ubus_strerror(ret));
        return ret;
    }

    ILOG_DEBUG(UBUS_CLIENT, "subscribe event %s success\n", event.c_str());

    return 0;
}

int
UBusClient::UnSubscribe(const std::string &event)
{
    int ret;
    uint32_t id;

    ILOG_TRACE(UBUS_CLIENT);

    ret = ubus_lookup_id(_ubus, event.c_str(), &id);
    if (ret) {
        ILOG_ERROR(UBUS_CLIENT, "lookup id %s failed\n", event.c_str());
        return ret;
    }

    ret = ubus_unsubscribe(_ubus, &_subscriber, _id);
    if (ret) {
        ILOG_ERROR(UBUS_CLIENT,"event %s unsubscribe failed (err: %s)\n",
                   event.c_str(), ubus_strerror(ret));
        return ret;
    }

    ILOG_DEBUG(UBUS_CLIENT, "unsubscribe event %s success\n", event.c_str());

    return 0;
}


int
UBusClient::Subscribe(const std::vector<std::string> &events)
{
    std::vector<std::string>::const_iterator it;
    int ret;

    ILOG_TRACE(UBUS_CLIENT);
    for (it = events.begin(); it != events.end(); ++it) {
        ret = Subscribe(*it);
        if (ret)
            goto unsubscribe;
    }

    ILOG_DEBUG(UBUS_CLIENT, "subscribe events success\n");

    return 0;

unsubscribe:
    for (; it != events.begin(); --it)
        UnSubscribe(*it);
    ILOG_ERROR(UBUS_CLIENT, "subscribe events fail\n");
    return -1;
}

int
UBusClient::UnSubscribe(const std::vector<std::string> &events)
{
    std::vector<std::string>::const_iterator it;

    ILOG_TRACE(UBUS_CLIENT);
    for (it = events.begin(); it != events.end(); ++it)
        UnSubscribe(*it);

    ILOG_DEBUG(UBUS_CLIENT, "unsubscribe events success\n");

    return 0;
}

} /* namespace MGUI */
