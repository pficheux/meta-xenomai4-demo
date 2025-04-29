FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# This value corresponds to linux-evl Xenomai
PV = "5.15.84"
LINUX_VERSION = "5.15.84"
LINUX_VERSION_EXTENSION:append = "-evl"
LINUX_RPI_EVL_BRANCH = "master"
SRCREV_machine = "d04bf7883260966d8b493e24190e86eb7767fc55"

SRC_URI = " \
	git://github.com/pficheux/linux-evl.git;name=machine;branch=${LINUX_RPI_EVL_BRANCH};protocol=https \
	git://git.yoctoproject.org/yocto-kernel-cache;type=kmeta;name=meta;branch=${LINUX_RPI_KMETA_BRANCH};destsuffix=${KMETA} \
	file://xenomai4-evl.cfg \
	"
