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
#include <sys/ioctl.h>
#include <linux/gpio.h>
#include <uapi/evl/devices/gpio.h>
#include <evl/evl.h>
#include <evl/sched.h>

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

int main(int argc, char **argv) {
	const char *devname = "/dev/gpiochip0";
	struct gpiohandle_data data = { 0 };
	int tfd, ret, oob_gpio_fd;
	unsigned char gpionum = 27;
	unsigned char val = 0;
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
	
	while(1) {
		data.values[0] = val;
		ret = oob_ioctl(oob_gpio_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		if (ret)
			error(1, errno,
			"ioctl(GPIOHANDLE_SET_LINE_VALUES_IOCTL) failed");

		evl_usleep(50);
		
		/* Start flipping the output pin. */
		data.values[0] = !val;
		ret = oob_ioctl(oob_gpio_fd, GPIOHANDLE_SET_LINE_VALUES_IOCTL, &data);
		if (ret)
			error(1, errno,
				"ioctl(GPIOHANDLE_SET_LINE_VALUES_IOCTL) failed");

		evl_usleep(50);
	}
	
	return 0;
}
