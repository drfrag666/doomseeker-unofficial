add_subdirectory(cszdoom)
add_subdirectory(odamex)

# For unmaintained source ports
option(BUILD_LEGACY_PLUGINS "Build plugins for discontinued source ports." OFF)
if(BUILD_LEGACY_PLUGINS)
	add_subdirectory(chocolate-doom)
	add_subdirectory(skulltag)
	add_subdirectory(vavoom)
	add_subdirectory(zandronum)
	add_subdirectory(zdaemon)
endif(BUILD_LEGACY_PLUGINS)

option(BUILD_FAKE_PLUGINS "Build plugins that are used for development purposes only." OFF)
if(BUILD_FAKE_PLUGINS)
	add_subdirectory(fakeplugin)
endif(BUILD_FAKE_PLUGINS)

