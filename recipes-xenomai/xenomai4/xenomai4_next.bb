SUMMARY = "Xenomai 4"
DESCRIPTION = "Xenomai 4 user support (libevl)"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=2500c28da9dacbee31a3f4d4f69b74d0"

SRCBRANCH = "next"
SRC_URI = "git://source.denx.de/Xenomai/xenomai4/libevl.git;protocol=https;branch=${SRCBRANCH}"
SRCREV = "${AUTOREV}"
S = "${WORKDIR}/git"

# We need the kernel sources to be available, so build it first.
DEPENDS += "virtual/kernel"

# We may need this for 64bit atomic ops.
RDEPENDS:${PN} = "libgcc"

inherit pkgconfig meson

PACKAGECONFIG[enable-debug] = "-Doptimization=0 -Ddebug=true,,,"

do_compile[depends] += "virtual/kernel:do_deploy"
EXTRA_OEMESON = "-Duapi=${STAGING_KERNEL_DIR}"

FILES:${PN} = "\
    ${bindir}/evl \
    ${bindir}/latmus \
    ${bindir}/hectic \
    ${bindir}/oob-spi \
    ${bindir}/oob-net-icmp \
    ${libdir}/libevl.so.* \
    ${libexecdir}/evl \
    ${prefix}/tests \
    "

FILES:${PN}-dev = "\
    ${libdir}/libevl.so \
    ${libdir}/pkgconfig/evl.pc \
    ${includedir}/evl \
    ${includedir}/uapi/evl \
    "

FILES:${PN}-dbg = "\
    ${libdir}/.debug/libevl.so \
    "

BBCLASSEXTEND = "native nativesdk"
