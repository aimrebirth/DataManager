#
# Polygon-4 Data Manager
#

# source groups
set(SSRC ${PROJECT_SOURCE_DIR})
set(BSRC ${CMAKE_CURRENT_BINARY_DIR})

set(_CPP ".*\\.cpp")
set(CPP "${_CPP}$")

set(_H ".*\\.h")
set(H "${_H}$")

set(H_CPP "(${H}|${CPP})")

source_group("Source Files\\Detail" "${BSRC}/detail/${CPP}")
source_group("Header Files\\Detail" "${BSRC}/detail/${H}")
source_group("Header Files\\Interface" "${SSRC}/include/Polygon4/DataManager/Interface/${H_CPP}")
source_group("MOC files" ".*automoc\\.cpp")

# sources
file(GLOB db_inc1 "${PROJECT_SOURCE_DIR}/include/Polygon4/DataManager/*")
file(GLOB db_inc2 "${PROJECT_SOURCE_DIR}/include/Polygon4/DataManager/Interface/*")
file(GLOB db_src "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB db_src_hdr "${CMAKE_CURRENT_SOURCE_DIR}/detail/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/detail/*.h")
set_source_files_properties(${db_src_hdr} PROPERTIES HEADER_FILE_ONLY True)

if (NOT POLYGON4_ENGINE)
    add_definitions(-DPOLYGON4_STATIC)
endif()

set(generated_dir ${CMAKE_CURRENT_BINARY_DIR}/detail)
set(generated_src
    ${generated_dir}/Types.h
    ${generated_dir}/Types.cpp
    ${generated_dir}/Storage.h
    ${generated_dir}/Storage.cpp
    ${generated_dir}/StorageImpl.h
    ${generated_dir}/StorageImpl.cpp
    ${generated_dir}/Helpers.h
    ${generated_dir}/Helpers.cpp
    ${generated_dir}/schema/Tokens.cpp
    ${generated_dir}/ObjectTypes.h
    ${generated_dir}/ObjectInterfaces.h
)
set_source_files_properties(${generated_src} PROPERTIES
    GENERATED True
    HEADER_FILE_ONLY True
)

add_custom_command(OUTPUT ${generated_src}
    COMMAND generator ${PROJECT_SOURCE_DIR}/schema.txt ${CMAKE_CURRENT_BINARY_DIR}/detail
    DEPENDS generator ${PROJECT_SOURCE_DIR}/schema.txt
)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
set(DATA_MANAGER_ADDITIONAL_INCLUDES ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "Data manager generated includes dir" FORCE)

add_library                 (DataManager ${db_src} ${db_inc1} ${db_inc2} ${db_src_hdr} ${generated_src} ${PROJECT_SOURCE_DIR}/schema.txt)
target_link_libraries       (DataManager sqlite3 schema)
if (MSVC)
    target_compile_options  (DataManager PUBLIC /bigobj)
endif()
if (Qt5Core_FOUND)
    set_target_properties   (DataManager PROPERTIES AUTOMOC True)
endif()
if (SOLUTION_FOLDER)
    set_target_properties   (DataManager PROPERTIES FOLDER ${SOLUTION_FOLDER})
endif()
