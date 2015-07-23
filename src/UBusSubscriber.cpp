
#include <core/Logger.h>
#include "UBusSubscriber.h"

namespace MGUI
{

D_DEBUG_DOMAIN(UBUS_SUBSCRIBER, "Ubus/Subscriber", "Subscriber");

UBusSubscriber::UBusSubscriber(ubus_context* ubus)
{
    ILOG_TRACE(UBUS_SUBSCRIBER);
    _ubus = ubus;
    ILOG_DEBUG(UBUS_SUBSCRIBER, "%s exit\n", __func__);
}

UBusSubscriber::~UBusSubscriber()
{
    ILOG_TRACE(UBUS_SUBSCRIBER);
    _ubus = NULL;
    ILOG_DEBUG(UBUS_SUBSCRIBER, "%s exit\n", __func__);
}


int
UBusSubscriber::Register(ubus_handler_t cb, const char *name)
{
    int ret;

    ILOG_TRACE(UBUS_SUBSCRIBER);

    ret = ubus_lookup_id(_ubus, name, &_id);
    if (ret) {
        ILOG_ERROR(UBUS_SUBSCRIBER, "lookup id %s failed\n", name);
        return ret;
    }
    _subscriber.cb = cb;
    ret = ubus_register_subscriber(_ubus, &_subscriber);
    if (ret) {
        ILOG_ERROR(UBUS_SUBSCRIBER, "register subscriber failed\n");
        return ret;
    }
    ILOG_DEBUG(UBUS_SUBSCRIBER, "%s exit\n", __func__);

    return 0;
}

int
UBusSubscriber::UnRegister()
{
    int ret;

    ILOG_TRACE(UBUS_SUBSCRIBER);

    ret = ubus_unregister_subscriber(_ubus, &_subscriber);
    if (ret) {
        ILOG_ERROR(UBUS_SUBSCRIBER, "unregister subscriber failed\n");
        return ret;
    }
    _subscriber.cb = NULL;
    ILOG_DEBUG(UBUS_SUBSCRIBER, "%s exit\n", __func__);

    return 0;
}

int
UBusSubscriber::Subscribe(std::string &event)
{
    int ret;

    ILOG_TRACE(UBUS_SUBSCRIBER);

    ret = ubus_subscribe(_ubus, &_subscriber, _id);
    if (ret) {
        ILOG_ERROR(UBUS_SUBSCRIBER,"event %s subscribe failed (err: %s)\n",
                   event.c_str(), ubus_strerror(ret));
        return ret;
    }

    ILOG_DEBUG(UBUS_SUBSCRIBER, "subscribe event %s success\n", event.c_str());

    return 0;
}

int
UBusSubscriber::UnSubscribe(std::string &event)
{
    int ret;

    ILOG_TRACE(UBUS_SUBSCRIBER);

    ret = ubus_unsubscribe(_ubus, &_subscriber, _id);
    if (ret) {
        ILOG_ERROR(UBUS_SUBSCRIBER,"event %s unsubscribe failed (err: %s)\n",
                   event.c_str(), ubus_strerror(ret));
        return ret;
    }

    ILOG_DEBUG(UBUS_SUBSCRIBER, "unsubscribe event %s success\n", event.c_str());

    return 0;
}


int
UBusSubscriber::Subscribe(std::vector<std::string> &events)
{
    std::vector<std::string>::iterator it;
    int ret;

    ILOG_TRACE(UBUS_SUBSCRIBER);
    for (it = events.begin(); it != events.end(); ++it) {
        ret = Subscribe(*it);
        if (ret)
            goto unsubscribe;
    }

    ILOG_DEBUG(UBUS_SUBSCRIBER, "subscribe events success\n");

    return 0;

unsubscribe:
    for (; it != events.begin(); --it)
        UnSubscribe(*it);
    ILOG_ERROR(UBUS_SUBSCRIBER, "subscribe events fail\n");
    return -1;
}

int
UBusSubscriber::UnSubscribe(std::vector<std::string> &events)
{
    std::vector<std::string>::iterator it;

    ILOG_TRACE(UBUS_SUBSCRIBER);
    for (it = events.begin(); it != events.end(); ++it)
        UnSubscribe(*it);

    ILOG_DEBUG(UBUS_SUBSCRIBER, "unsubscribe events success\n");

    return 0;
}

} /* namespace MGUI */
