/*
 * Xenomai 4 (evl core) program to generate square
 * wave
 *
 * gcc -v -Wall x4_evl_square_wave.c -levl -o x4
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <error.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>
#include <uapi/evl/devices/gpio.h>
#include <evl/evl.h>
#include <evl/sched.h>
#include <evl/timer.h>
#include <evl/clock.h>
#include <evl/proxy.h>

static int gpio_outfd = -1;

static int setup_gpio_out_pin(int fd, unsigned char gn) {
	struct gpiohandle_request out;
	int ret;

	out.lineoffsets[0] = gn; /* Output pin number */
        out.lines = 1;
	out.flags = GPIOHANDLE_REQUEST_OUTPUT | GPIOHANDLE_REQUEST_OOB;
        out.default_values[0] = 1;
	strcpy(out.consumer_label, "x4-wave");

	ret = ioctl(fd, GPIO_GET_LINEHANDLE_IOCTL, &out);
	if (ret)
		error(1, errno, "ioctl(GPIO_GET_LINEHANDLE_IOCTL)");

	/* out.fd now contains the oob-capable line handle descriptor. */

	return out.fd;
}

void timespec_add_ns(struct timespec *__restrict r,
	     		     const struct timespec *__restrict t,
			     long ns) {
	long s, rem;

	s = ns / 1000000000;
	rem = ns - s * 1000000000;
	r->tv_sec = t->tv_sec + s;
	r->tv_nsec = t->tv_nsec + rem;
	if (r->tv_nsec >= 1000000000) {
	     r->tv_sec++;
	     r->tv_nsec -= 1000000000;
	}
}

int main(int argc, char **argv) {
	const char *devname = "/dev/gpiochip0";
	struct gpiohandle_data data = { 0 };
	int tfd, tmfd, ret, oob_gpio_fd;
	unsigned char gpionum = 27;
	unsigned char val = 0;
	struct timespec now;
	__u64 ticks;
	struct itimerspec value, ovalue;
	struct evl_sched_attrs attrs = {
		.sched_policy = SCHED_FIFO,
		.sched_priority = 99, /* [1-99] */
	};
	
	gpio_outfd = open(devname, O_RDONLY);
	if (gpio_outfd < 0)
		error(1, errno, "gpio open failed");

	oob_gpio_fd = setup_gpio_out_pin(gpio_outfd, gpionum);
	if (oob_gpio_fd < 0)
		error(1, errno, "setup_gpio_out_pin() failed");

	tfd = evl_attach_self("/x4-sq-wave:%d", getpid());
	if (tfd < 0)
		error(1, errno, "evl_attach_self() failed");

	ret = evl_set_schedattr(tfd, &attrs);
	if (ret)
		error(1, errno,	"evl_set_schedattr() failed");

	/* Setup timerfd */
	tmfd = evl_new_timer(EVL_CLOCK_MONOTONIC);
	if (tmfd < 0)
		error(1, errno, "evl_new_timer() failed");

	ret = evl_read_clock(EVL_CLOCK_MONOTONIC, &now);
	if (ret)
		error(1, errno,	"evl_read_clock() failed");
	
	/* EVL always uses absolute timeouts, add 1000us to the current date */
	timespec_add_ns(&value.it_value, &now, 1000000ULL);
	value.it_interval.tv_sec = 0;
	value.it_interval.tv_nsec = 1000000;
	ret = evl_set_timer(tmfd, &value, &ovalue);
	if (ret)
		error(1, errno,	"evl_set_timer() failed");

		
	while(1) {
		/* Wait for the next tick to be notified. */
		ret = oob_read(tmfd, &ticks, sizeof(ticks));
		if (ret < 0)
			error(1, errno,	"oob timer read failed");

		if (ticks > 1) {
			fprintf(stderr, "timer overrun! %lld ticks late\n",
			 	ticks - 1);
			break;
		}

		data.values[0] = val;
		ret = oob_ioctl(oob_gpio_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		if (ret)
			error(1, errno,
			"ioctl(GPIOHANDLE_SET_LINE_VALUES_IOCTL) failed");

		val = !val;
		
	}
	
	return 0;
}
