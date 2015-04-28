#
# Polygon-4 Data Manager
#

file(GLOB_RECURSE db_inc "${CMAKE_CURRENT_SOURCE_DIR}/../include/Polygon4/*")
file(GLOB db_src "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

set(db_src
    ${db_src}
    Database.cpp
)

add_library             (DatabaseManager ${db_src} ${db_inc})
target_link_libraries   (DatabaseManager sqlite3)
