
# define sources and linked libraries
SET(SOURCES
	main.cpp
	MGuiPlatform.h
	BatteryIcon.cpp
	BatteryIcon.h
	BTIcon.cpp
	BTIcon.h
	CellularIcon.cpp
	CellularIcon.h
	GPSIcon.cpp
	GPSIcon.h
	HawkScreen.cpp
	HawkScreen.h
	HomeScreen.cpp
	HomeScreen.h
	MGuiApplication.cpp
	MGuiApplication.h
	ScreenBase.cpp
	ScreenBase.h
	SDCardIcon.cpp
	SDCardIcon.h
	SimIcon.cpp
	SimIcon.h
	SimpleIcon.cpp
	SimpleIcon.h
	StatScreen.cpp
	StatScreen.h
	StatusBar.cpp
	StatusBar.h
	UBusThread.cpp
	UBusThread.h
	UBusSubscriber.cpp
	UBusSubscriber.h
	MGuiRil.cpp
	MGuiRil.h
	WiFiIcon.cpp
	WiFiIcon.h
)

# create and install executable
ADD_EXECUTABLE(mgui ${SOURCES})
TARGET_LINK_LIBRARIES(mgui ${LIBS})
INSTALL(TARGETS mgui RUNTIME DESTINATION bin)
