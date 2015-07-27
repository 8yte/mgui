## Makefile.am for ./src
ACLOCAL_AMFLAGS = -I build ${ACLOCAL_FLAGS}
AM_CFLAGS 	= -std=c++11 -DDATADIR=\"$(MGUI_DATADIR)\"
bin_PROGRAMS	= mgui

mgui_SOURCES	= main.cpp \
		ui/MGuiPlatform.h \
		ui/BatteryIcon.cpp \
		ui/BatteryIcon.h \
		ui/BottomBar.cpp \
		ui/BottomBar.h \
		ui/BTIcon.cpp \
		ui/BTIcon.h \
		ui/CellularIcon.cpp \
		ui/CellularIcon.h \
		ui/CellularTechIcon.cpp \
		ui/CellularTechIcon.h \
		ui/GPSIcon.cpp \
		ui/GPSIcon.h \
		ui/MGuiApplication.cpp \
		ui/MGuiApplication.h \
		ui/SDCardIcon.cpp \
		ui/SDCardIcon.h \
		ui/SimIcon.cpp \
		ui/SimIcon.h \
		ui/SimpleIcon.cpp \
		ui/SimpleIcon.h \
		ui/StatusBar.cpp \
		ui/StatusBar.h \
		ui/WiFiIcon.cpp \
		ui/WiFiIcon.h

if PXA1826
AM_CFLAGS += -DPXA1826
mgui_SOURCES += platform/pxa1826/UBusThread.cpp \
		platform/pxa1826/UBusThread.h \
		platform/pxa1826/UBusClient.cpp \
		platform/pxa1826/UBusClient.h \
		platform/pxa1826/UBusRequest.cpp \
		platform/pxa1826/UBusRequest.h \
		platform/pxa1826/MGuiRil.cpp \
		platform/pxa1826/MGuiRil.h \
		platform/pxa1826/RilRequest.cpp \
		platform/pxa1826/RilRequest.h \
		platform/pxa1826/OnkeyThread.cpp \
		platform/pxa1826/OnkeyThread.h \
		platform/pxa1826/ChargerRequest.cpp \
		platform/pxa1826/ChargerRequest.h \
		platform/pxa1826/MGuiCharger.cpp \
		platform/pxa1826/MGuiCharger.h \
		platform/pxa1826/WifiRequest.cpp \
		platform/pxa1826/WifiRequest.h \
		platform/pxa1826/MGuiWifi.cpp \
		platform/pxa1826/MGuiWifi.h \
		platform/pxa1826/StatsRequest.cpp \
		platform/pxa1826/StatsRequest.h \
		platform/pxa1826/MGuiStats.cpp \
		platform/pxa1826/MGuiStats.h \
		platform/pxa1826/HawkRequest.cpp \
		platform/pxa1826/HawkRequest.h \
		platform/pxa1826/MGuiHawk.cpp \
		platform/pxa1826/MGuiHawk.h
endif

mgui_CPPFLAGS	= $(AM_CFLAGS) $(ILIXI_CFLAGS)
mgui_LDADD	= $(AM_LDFLAGS) $(ILIXI_LIBS)
