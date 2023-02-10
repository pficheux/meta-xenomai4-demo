FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# This value corresponds to linux-evl Xenomai
PV = "5.15.84"
LINUX_VERSION = "5.15.84"
LINUX_VERSION_EXTENSION:append = "-evl"
LINUX_RPI_EVL_BRANCH = "rpi4-v5.15.84-evl"
SRCREV_machine = "a583e0f9b251371c64bd61cdaaf6de040c81a247"

SRC_URI = " \
	git://source.denx.de/lukma/linux-evl.git;name=machine;branch=${LINUX_RPI_EVL_BRANCH};protocol=https \
	git://git.yoctoproject.org/yocto-kernel-cache;type=kmeta;name=meta;branch=${LINUX_RPI_KMETA_BRANCH};destsuffix=${KMETA} \
	file://xenomai4-evl.cfg \
	"
