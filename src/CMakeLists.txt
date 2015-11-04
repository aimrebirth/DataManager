#
# Polygon-4 Data Manager
#

# source groups
set(SSRC ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(SSRC "${SSRC}" DIRECTORY)

set(_CPP ".*\\.cpp")
set(CPP "${_CPP}$")

set(_H ".*\\.h")
set(H "${_H}$")

set(H_CPP "(${H}|${CPP})")

source_group("Source Files\\detail" "${SSRC}/src/detail/${H_CPP}")
source_group("Header Files\\detail" "${SSRC}/include/Polygon4/DataManager/detail/${H_CPP}")
source_group("Header Files\\interface" "${SSRC}/include/Polygon4/DataManager/interface/${H_CPP}")

# sources
file(GLOB_RECURSE db_inc "${CMAKE_CURRENT_SOURCE_DIR}/../include/*")
file(GLOB db_src "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB db_src_hdr "${CMAKE_CURRENT_SOURCE_DIR}/detail/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/detail/*.h")
set_source_files_properties(${db_src_hdr} PROPERTIES HEADER_FILE_ONLY True)

if (NOT POLYGON4_ENGINE)
    add_definitions(-DPOLYGON4_STATIC)
endif()

add_library                 (DataManager ${db_src} ${db_inc} ${db_src_hdr})
target_link_libraries       (DataManager sqlite3)
if (MSVC)
target_compile_options      (DataManager PUBLIC /bigobj)
endif()
if (Qt5Core_FOUND)
    set_target_properties   (DataManager PROPERTIES AUTOMOC True)
endif()
