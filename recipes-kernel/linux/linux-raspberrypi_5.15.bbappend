FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

# This value corresponds to linux-evl Xenomai
PV = "5.15.84"
LINUX_VERSION = "5.15.84"
LINUX_VERSION_EXTENSION:append = "-evl"
SRCREV_machine = "3f4092766eaf692ed79b69f59c98dbe38e557fe7"

SRC_URI:append = " file://xenomai4-evl.cfg"
