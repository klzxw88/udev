PROJECT(udev)

FIND_PACKAGE(PkgConfig REQUIRED)

PKG_CHECK_MODULES(GLIB REQUIRED glib-2.0)
pkg_check_modules(UDEV REQUIRED libudev)

set(BIN_NAME udev)
file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${GLIB_INCLUDE_DIRS})

LINK_DIRECTORIES(${GLIB_LIBRARY_DIRS})

message(STATUS "${SRC_FILES}")

add_executable(${BIN_NAME} ${SRC_FILES} )

message(STATUS "${UDEV_LDFLAGS}")
target_link_libraries(${BIN_NAME}
	${UDEV_LDFLAGS}
	${GLIB_LIBRARIES}
	pthread
	)
