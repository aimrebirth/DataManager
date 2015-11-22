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

source_group("Source Files\\Detail" "${SSRC}/src/manager/detail/${CPP}")
source_group("Header Files\\Detail" "${SSRC}/include/Polygon4/DataManager/detail/${H}")
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

set(generated_dir ${PROJECT_SOURCE_DIR})
set(generated_dir_inc ${generated_dir}/include/Polygon4/DataManager/detail)
set(generated_dir_src ${generated_dir}/src/manager/detail)
set(generated_src
    ${generated_dir_inc}/Types.h
    ${generated_dir_src}/Types.cpp
    ${generated_dir_inc}/Storage.h
    ${generated_dir_src}/Storage.cpp
    ${generated_dir_inc}/StorageImpl.h
    ${generated_dir_src}/StorageImpl.cpp
    ${generated_dir_src}/schema/Tokens.cpp
    ${generated_dir_inc}/ObjectTypes.h
    ${generated_dir_inc}/ObjectInterfaces.h
    ${generated_dir_inc}/Enums.h
    ${generated_dir_src}/Enums.cpp
)
set_source_files_properties(${generated_src} PROPERTIES
    GENERATED True
    HEADER_FILE_ONLY True
)

add_custom_command(OUTPUT ${generated_src}
    COMMAND generator ${PROJECT_SOURCE_DIR}/data/schema.txt ${PROJECT_SOURCE_DIR}
    DEPENDS generator ${PROJECT_SOURCE_DIR}/data/schema.txt
)

set(DATA_MANAGER_ADDITIONAL_INCLUDES ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "Data manager generated includes dir" FORCE)

add_library                 (DataManager ${db_src} ${db_inc1} ${db_inc2} ${db_src_hdr} ${generated_src} ${PROJECT_SOURCE_DIR}/data/schema.txt)
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
