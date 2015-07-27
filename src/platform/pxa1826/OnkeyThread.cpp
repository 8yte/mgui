#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/input.h>
#include <core/Logger.h>
#include <iostream>
#include <MGuiPlatform.h>
#include "OnkeyThread.h"

#define ONKEY_DEV "/dev/input/event1"

namespace MGUI
{

D_DEBUG_DOMAIN(ONKEY_THREAD, "Onkey/Thread", "Onkey");

OnkeyThread::OnkeyThread(int fd)
	: Thread(),
	_pipe(fd),
	_event(OnkeyEvent),
	_dev(ONKEY_DEV)
{
	ILOG_TRACE(ONKEY_THREAD);

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

	fd = open(_dev, O_RDONLY);
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
			sigOnkeyPress();
			write(_pipe, &_event, sizeof(_event));
		}
	} while (1);

	close(fd);
	return 0;
}

} /* namespace MGUI */
