SUMMARY = "Xenomai 4 (evl) demo(s)"
DESCRIPTION = "Xenomai 4 (evl) square wave recipe"
LICENSE = "MIT"

DEPENDS_${PN}="xenomai4"

SRC_URI = "\
	file://x4_evl_square_wave.c \
	file://Makefile \
	"
