#
# Polygon-4 Data Manager
#

###############################################################################
#
# cmake settings
#
###############################################################################

cmake_minimum_required(VERSION 2.8.11)

# In-source builds are disabled.
if (${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR
        "CMake generation is not possible within the source directory!"
        "\n Remove the CMakeCache.txt file and try again from another folder, e.g.:"
        "\n "
        "\n rm CMakeCache.txt"
        "\n mkdir build"
        "\n cd build"
        "\n cmake .."
    )
endif()

# Use solution folders.
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake Targets")

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${EXECUTABLE_OUTPUT_PATH}")

###############################################################################
#
# project settings
#
###############################################################################

project(DataManager C CXX)

# libraries and dependencies
if (WIN32)
    set(Boost_USE_STATIC_LIBS ON)
else()
    ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
endif(WIN32)

find_package(Boost 1.54 REQUIRED filesystem system)
find_package(BISON 2.4 REQUIRED)
find_package(FLEX 2.5.35 REQUIRED)

###############################################################################
#
# compiler and linker
#
###############################################################################

if (WIN32)
    if (MSVC)
        set(disable_msvc_warnings "/W1 /wd4996")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${disable_msvc_warnings}")
        set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${disable_msvc_warnings}")
    endif()
elseif (UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
endif()

###############################################################################
#
# build
#
###############################################################################

include_directories(${Boost_INCLUDE_DIRS})
include_directories(include)
include_directories(dep)

link_directories(${Boost_LIBRARY_DIRS})

add_library                 (sqlite3 dep/sqlite3/sqlite3.c dep/sqlite3/sqlite3.h)
if (SOLUTION_FOLDER)
    set_target_properties   (sqlite3 PROPERTIES FOLDER ${SOLUTION_FOLDER})
endif()

add_subdirectory(src)

############################################################################### 


