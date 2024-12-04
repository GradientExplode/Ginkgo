# ---------------------------------------
# add myself to dependency list
# ---------------------------------------
MAKEFILE_CHAIN += $(IDEA_BASE)/n4/pkg/MrServers/Gkg/makefiles/MakeGkg.mk

# 
# GKG sources
#
SOURCES += ${IDEA_BASE}/n4/pkg/MrServers/Gkg/gkg-win32-feedback/RealTimeFeedbackSequence.cpp
SOURCES += ${IDEA_BASE}/n4/pkg/MrServers/Gkg/gkg-win32-license/KeyName.cpp
SOURCES += ${IDEA_BASE}/n4/pkg/MrServers/Gkg/gkg-win32-license/LicenseCenter.cpp
SOURCES += ${IDEA_BASE}/n4/pkg/MrServers/Gkg/gkg-win32-license/LicenseEncoder.cpp
SOURCES += ${IDEA_BASE}/n4/pkg/MrServers/Gkg/gkg-win32-license/LicenseFormat.cpp
SOURCES += ${IDEA_BASE}/n4/pkg/MrServers/Gkg/gkg-win32-license/RealTimeLicense.cpp
SOURCES += ${IDEA_BASE}/n4/pkg/MrServers/Gkg/gkg-win32-register/Register.cpp
SOURCES += ${IDEA_BASE}/n4/pkg/MrServers/Gkg/gkg-win32-sysinfo/MacAddress.cpp

# 
# GKG and MS-SDK includes, libs, and definitions
#

CPPFLAGS_LOCAL += -D__STDC_FORMAT_MACROS

WIN_INCL_PATH += -I$(IDEA_BASE)/n4/pkg/MrServers/Gkg
XLINUX_INCLUDE_PATHS += -I$(IDEA_BASE)/n4/pkg/MrServers/Gkg

WIN_INCL_PATH += -I$(IDEA_BASE)/n4/pkg/MrServers/Gkg/microsoft-sdk/include
WIN_LIB_PATH += -libpath:$(IDEA_BASE)/n4/pkg/MrServers/Gkg/microsoft-sdk/lib
