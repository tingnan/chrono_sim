cmake_minimum_required(VERSION 2.8)
project("chrono_sim")
add_definitions(-std=c++11 -g)
set(CHRONO_DIR /usr/local/chrono)
set(IRRLICHT_INCLUDE_DIRS /usr/include/irrlicht /usr/local/include/irrlicht)
include_directories(.)
include_directories(/usr/include/eigen3)
include_directories(/usr/local/include/eigen3)
include_directories(third_party)
include_directories(${IRRLICHT_INCLUDE_DIRS})
include_directories(${CHRONO_DIR}/include)
include_directories(${CHRONO_DIR}/include/chrono_irrlicht)
include_directories(${CHRONO_DIR}/include/chrono)
include_directories(${CHRONO_DIR}/include/chrono/collision/bullet)
link_directories(${CHRONO_DIR}/lib)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	add_definitions(-DNOMINMAX)
endif()

file(GLOB SOURCES "src/*.cpp")
file(GLOB HEADERS "include/*.h")
add_executable(main MACOSX_BUNDLE ${HEADERS} ${SOURCES})
find_library(CHRONO_LIB NAMES ChronoEngine PATHS ${CHRONO_DIR}/lib)
find_library(CHRONO_IRRLICHT_LIB NAMES ChronoEngine_irrlicht PATHS ${CHRONO_DIR}/lib)
set(CHRONO_LIBS ${CHRONO_LIB} ${CHRONO_IRRLICHT_LIB})
find_library(IRRLICHT_LIBS NAMES Irrlicht)

target_link_libraries(main ${CHRONO_LIBS} ${IRRLICHT_LIBS})
