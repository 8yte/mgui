
# define sources and linked libraries
SET(SOURCES
	main.cpp
	MGuiPlatform.h
	BatteryIcon.cpp
	BatteryIcon.h
	BottomBar.cpp
	BottomBar.h
	BTIcon.cpp
	BTIcon.h
	CellularIcon.cpp
	CellularIcon.h
	GPSIcon.cpp
	GPSIcon.h
	MGuiApplication.cpp
	MGuiApplication.h
	SDCardIcon.cpp
	SDCardIcon.h
	SimIcon.cpp
	SimIcon.h
	SimpleIcon.cpp
	SimpleIcon.h
	StatusBar.cpp
	StatusBar.h
	UBusThread.cpp
	UBusThread.h
	WiFiIcon.cpp
	WiFiIcon.h
)

# create and install executable
ADD_EXECUTABLE(mgui ${SOURCES})
TARGET_LINK_LIBRARIES(mgui ${LIBS})
INSTALL(TARGETS mgui RUNTIME DESTINATION bin)
