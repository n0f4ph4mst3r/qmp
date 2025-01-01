set(SOURCE_FILES 
				main.cpp
				MediaApp.cpp
				ControlPanel.cpp
				VidWidget.cpp
)

set(HEADERS 
			MediaApp.h
			ControlPanel.h
			VidWidget.h
)

qt_add_resources(RESOURCES res/icons.qrc)

if(WIN32)
	source_group("Resource files" FILES res/app_icon.rc res/icons.qrc)
	qt_add_executable(qmp MANUAL_FINALIZATION ${HEADERS} ${SOURCE_FILES} ${RESOURCES} res/app_icon.rc)
else()
	qt_add_executable(qmp MANUAL_FINALIZATION ${HEADERS} ${SOURCE_FILES} ${RESOURCES})
endif()