## Makefile.am for ./src
ACLOCAL_AMFLAGS = -I build ${ACLOCAL_FLAGS}
AM_CFLAGS 		=-DDATADIR=\"$(MGUI_DATADIR)\"
bin_PROGRAMS	= ilixi_mgui

ilixi_mgui_SOURCES	= main.cpp \
			MGuiPlatform.h \
			BatteryIcon.cpp \
			BatteryIcon.h \
			BottomBar.cpp \
			BottomBar.h \
			BTIcon.cpp \
			BTIcon.h \
			CellularIcon.cpp \
			CellularIcon.h \
			GPSIcon.cpp \
			GPSIcon.h \
			MGuiApplication.cpp \
			MGuiApplication.h \
			SDCardIcon.cpp \
			SDCardIcon.h \
			SimIcon.cpp \
			SimIcon.h \
			SimpleIcon.cpp \
			SimpleIcon.h \
			StatusBar.cpp \
			StatusBar.h \
			WiFiIcon.cpp \
			WiFiIcon.h

ilixi_mgui_CPPFLAGS	= $(AM_CFLAGS) $(ILIXI_CFLAGS)
ilixi_mgui_LDADD	= $(AM_LDFLAGS) $(ILIXI_LIBS)
