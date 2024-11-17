cmake_minimum_required (VERSION 3.0)

project (kdegames)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)

find_package(KDE1 REQUIRED)

add_definitions(-DHAVE_CONFIG_H)

include(CMakePackageConfigHelpers)
include(Qt1Macros)
include(KDE1Macros)
include(KDE1InstallDirs)

if (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-write-strings")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++98")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--no-undefined")
endif()

if (CMAKE_SYSTEM_NAME MATCHES "NetBSD")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -I/usr/X11R7/include -I/usr/pkg/include -I/usr/local/include -I/usr/kde1/include -L/usr/X11R7/lib -L/usr/pkg/lib -L/usr/local/lib -L/usr/kde1/lib -Wl,-R/usr/X11R7/lib -Wl,-R/usr/pkg/lib -Wl,-R/usr/local/lib -Wl,-R/usr/kde1/lib")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -I/usr/X11R7/include -I/usr/pkg/include -I/usr/local/include -I/usr/kde1/include -L/usr/X11R7/lib -L/usr/pkg/lib -L/usr/local/lib -L/usr/kde1/lib -Wl,-R/usr/X11R7/lib -Wl,-R/usr/pkg/lib -Wl,-R/usr/local/lib -Wl,-R/usr/kde1/lib")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -L/usr/X11R7/lib -L/usr/pkg/lib -L/usr/local/lib -L/usr/kde1/lib -R/usr/X11R7/lib -R/usr/pkg/lib -R/usr/local/lib -R/usr/kde1/lib")
elseif (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -I/usr/local/include -I/usr/kde1/include")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -I/usr/local/include -I/usr/kde1/include")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -L/usr/local/lib -L/usr/kde1/lib -R/usr/local/lib -R/usr/kde1/lib")
elseif (CMAKE_SYSTEM_NAME MATCHES "OpenBSD")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -I/usr/local/include -I/usr/kde1/include -I/usr/X11R6/include")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -I/usr/local/include -I/usr/kde1/include -I/usr/X11R6/include")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -L/usr/local/lib -L/usr/kde1/lib -L/usr/X11R6/lib -R/usr/local/lib -R/usr/kde1/lib -R/usr/X11R6/lib")
endif()

configure_file(common/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#add_subdirectory(bsd-port)
add_subdirectory(kabalone)
add_subdirectory(kasteroids) # need kdesupport-1.1.2 for that (QwSpriteField.h)
add_subdirectory(kblackbox)
#add_subdirectory(kenolaba) # Apparently this is the old name for kabalone
add_subdirectory(kmahjongg)
add_subdirectory(kmines)
add_subdirectory(konquest)
add_subdirectory(kpat)
add_subdirectory(kpoker)
add_subdirectory(kreversi)
add_subdirectory(ksame)
add_subdirectory(kshisen)
add_subdirectory(ksirtet)
add_subdirectory(ksnake)
add_subdirectory(ksmiletris)
add_subdirectory(ksokoban) # build system hell with povray and bin2c and crap

add_subdirectory(po)
