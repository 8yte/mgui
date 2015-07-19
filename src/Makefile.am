## Makefile.am for ./src
ACLOCAL_AMFLAGS = -I build ${ACLOCAL_FLAGS}
AM_CFLAGS 		=-DDATADIR=\"$(MGUI_DATADIR)\"
bin_PROGRAMS	= ilixi_mgui

ilixi_mgui_SOURCES	= main.cpp \
			Defines.h \
			MGuiApplication.cpp \
			MGuiApplication.h \
			StatusBar.cpp \
			StatusBar.h \
			BottomBar.cpp \
			BottomBar.h \
			BatteryIcon.cpp \
			BatteryIcon.h \
			SimpleIcon.cpp \
			SimpleIcon.h
ilixi_mgui_CPPFLAGS	= $(AM_CFLAGS) $(ILIXI_CFLAGS)
ilixi_mgui_LDADD	= $(AM_LDFLAGS) $(ILIXI_LIBS)
