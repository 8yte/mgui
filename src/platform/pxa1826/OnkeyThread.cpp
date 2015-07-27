#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/input.h>
#include <core/Logger.h>
#include "OnkeyThread.h"

#define ONKEY_DEV "/dev/input/event1"

namespace MGUI
{

D_DEBUG_DOMAIN(ONKEY_THREAD, "Onkey/Thread", "Onkey");

OnkeyThread::OnkeyThread(ubus_context* ubus)
	: Thread(),
	_ubus(ubus)
{
	ILOG_TRACE(ONKEY_THREAD);

	//if (!_ubus)
		//ILOG_THROW(ONKEY_THREAD, "Failed to connect to ubus\n");

	ILOG_DEBUG(ONKEY_THREAD, "%s exit\n", __func__);
}

OnkeyThread::~OnkeyThread()
{
	ILOG_TRACE(ONKEY_THREAD);

	ILOG_DEBUG(ONKEY_THREAD, "%s exit\n", __func__);
}

int
OnkeyThread::run()
{
	struct input_event data;
	unsigned int press = 0;
	int fd = -1;

	ILOG_TRACE(ONKEY_THREAD);

	fd = open(ONKEY_DEV, O_RDONLY);
	if (fd < 0)
		ILOG_THROW(ONKEY_THREAD, "Failed to open input fd\n");

	do {
		read(fd, &data, sizeof(data));

		if ((data.type != EV_KEY) || (data.code != KEY_POWER) || (!!data.value == press))
			continue;
		if (!!data.value) {
			/* key pressed */
			press = 1;
		} else {
			/* key released, dispatch event */
			press = 0;
			ILOG_ERROR(ONKEY_THREAD, "onkey pressed\n");
			//TODO - add handling
		}
	} while (1);

	close(fd);
	return 0;
}

} /* namespace MGUI */
