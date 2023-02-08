/*
 * libgpiod program to generate square wave
 *
 * gcc -v -Wall square_wave.c -lgpiod -o sqw
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <sched.h>
#include <string.h>
#include <time.h>
#include <gpiod.h>
#include <sys/ioctl.h>
#include <sys/timerfd.h>
#include <sys/select.h>
#include <linux/gpio.h>

int main(int argc, char **argv)
{
	char *chipname = "gpiochip0";
	unsigned int line_num = 27;
	unsigned int val;
	struct gpiod_chip *chip;
	struct gpiod_line *line;
	struct sched_param attrs = {
		.sched_priority = 99, /* [1-99] */
	};
	struct itimerspec spec =
        {
            { 0, 500000 },
            { 0, 500000 }
        };
	char dummybuf[8];
	int ret, tfd;

	if (sched_setscheduler(0, SCHED_FIFO, &attrs) != 0)
		error(1, errno, "Failed to set the scheduler\n");

	chip = gpiod_chip_open_by_name(chipname);
	if (!chip)
		error(1, errno, "Open chip failed");

	line = gpiod_chip_get_line(chip, line_num);
	if (!line) {
		gpiod_chip_close(chip);
		error(1, errno, "Get line failed");
	}

	ret = gpiod_line_request_output(line, "square-wave", 0);
	if (ret < 0) {
		gpiod_line_release(line);
		gpiod_chip_close(chip);
		error(1, errno, "Request line as output failed");
	}

	tfd = timerfd_create(CLOCK_MONOTONIC,  0);
	if (tfd <= 0)
		error(1, errno, "timerfd_create() failed");

	ret = timerfd_settime(tfd, 0, &spec, NULL);
	if (ret == -1)
		error(1, errno, "timerfd_settime() failed");
	
	val = 0;
	while (1) {
		ret = read(tfd, &dummybuf, sizeof(dummybuf));
		if (ret < 0)
			error(1, errno, "read() on tfd failed");

		ret = gpiod_line_set_value(line, val);
		if (ret < 0) {
			perror("Set line output failed\n");
		}
		val = !val;
	}

	return 0;
}
