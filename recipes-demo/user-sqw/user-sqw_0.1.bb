SUMMARY = "Xenomai 4 (evl) demo(s)"
DESCRIPTION = "User space (with libgpiod) square wave recipe"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS="libgpiod"

SRC_URI = "\
	file://square_wave.c \
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
	install -m 0755 sqw ${D}${bindir}

	install -d ${D}/usr/src/${PN}
	install -m 0755 ${WORKDIR}/square_wave.c ${D}/usr/src/${PN}
	install -m 0755 ${WORKDIR}/Makefile ${D}/usr/src/${PN}
}

FILES:${PN} = "\
	    ${bindir} \
	    /usr/src/${PN} \
	    "
