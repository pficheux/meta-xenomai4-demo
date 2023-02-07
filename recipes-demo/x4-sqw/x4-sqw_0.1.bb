SUMMARY = "Xenomai 4 (evl) demo(s)"
DESCRIPTION = "Xenomai 4 (evl) square wave recipe"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS="xenomai4"

SRC_URI = "\
	file://x4_evl_square_wave.c \
	file://Makefile \
	"

S = "${WORKDIR}"
TARGET_CC_ARCH += "${LDFLAGS}"

EXTRA_OEMAKE = "CC='${CC}' CFLAGS='${CFLAGS}'"
do_compile() {
	oe_runmake
}

do_install() {
	install -d ${D}${bindir}
	install -m 0755 x4 ${D}${bindir}
}
