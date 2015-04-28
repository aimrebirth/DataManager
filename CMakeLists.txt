#
# Polygon-4 Data Manager
#

cmake_minimum_required(VERSION 2.8.11)

project(DatabaseManager C CXX)

include_directories(include)
include_directories(dep)

add_library(sqlite3 dep/sqlite3/sqlite3.c dep/sqlite3/sqlite3.h)

add_subdirectory(src)