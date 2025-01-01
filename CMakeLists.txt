cmake_minimum_required(VERSION 3.31.2)

if(NOT DEFINED ${CMAKE_TOOLCHAIN_FILE})
	set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
endif()

option(VCPKG_ROOT_TRIPLET "Vcpkg triplet" OFF)
set(VCPKG_OVERLAY_TRIPLETS ${CMAKE_CURRENT_SOURCE_DIR}/triplets)
if(NOT VCPKG_ROOT_TRIPLET)
	if (WIN32)
		set(VCPKG_HOST_TRIPLET x64-windows-mixed)
		set(VCPKG_TARGET_TRIPLET x64-windows-mixed)
	# elseif (UNIX)
	# 	set(VCPKG_HOST_TRIPLET x64-linux-mixed)
	# 	set(VCPKG_TARGET_TRIPLET x64-linux-mixed)
    endif()
else()
	set(VCPKG_HOST_TRIPLET ${VCPKG_ROOT_TRIPLET})
	set(VCPKG_TARGET_TRIPLET ${VCPKG_ROOT_TRIPLET})
endif()

if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Default install path" FORCE)
endif()

project(qmp VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_INSTALL_BINDIR .)

find_package(Qt6 REQUIRED COMPONENTS MultimediaWidgets Network)
qt_standard_project_setup()
if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GSTREAMER REQUIRED IMPORTED_TARGET gstreamer-1.0 gstreamer-gl-egl-1.0)
endif()

if(CMAKE_GENERATOR MATCHES "Visual Studio")
    set_directory_properties(
        PROPERTIES
            VS_STARTUP_PROJECT qmp
    )
endif()



add_subdirectory(src)
if(!UNIX)
    target_link_libraries(qmp PUBLIC 
                                    Qt::MultimediaWidgets
                                    Qt::Network
    )
else()
    target_link_libraries(qmp PUBLIC 
                                    Qt::MultimediaWidgets
                                    Qt::Network
                                    ${GSTREAMER_LIBRARIES}
    )
endif()
target_compile_features(qmp PRIVATE cxx_std_17)
qt_finalize_executable(qmp)

set_target_properties(qmp PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

set(CMAKE_INSTALL_MESSAGE "-- Installing: ${CMAKE_INSTALL_PREFIX}")
install(TARGETS qmp
    BUNDLE  DESTINATION .
)

qt_generate_deploy_script(
    TARGET qmp
    OUTPUT_SCRIPT deploy_script
    CONTENT "
qt_deploy_runtime_dependencies(
    EXECUTABLE \"./$<TARGET_FILE_NAME:qmp>\"
    GENERATE_QT_CONF
	PLUGINS_DIR QtPlugins
    VERBOSE
	NO_TRANSLATIONS
)")
install(SCRIPT ${deploy_script})