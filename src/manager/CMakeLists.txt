#
# Polygon-4 Data Manager
#

set(tail)
if (MSVC)
    set(tail _$(Configuration))
endif()

set(generated_dir ${CMAKE_BINARY_DIR}/gen${tail})
set(generated_dir_inc           ${generated_dir}/include)
set(generated_dir_inc_detail    ${generated_dir}/include/detail)
set(generated_dir_src           ${generated_dir}/src)
set(generated_dir_src_detail    ${generated_dir}/src/detail)

# source groups
set(SSRC ${PROJECT_SOURCE_DIR})
set(BSRC ${CMAKE_CURRENT_BINARY_DIR})

set(_CPP ".*\\.cpp")
set(CPP "${_CPP}$")

set(_H ".*\\.h")
set(H "${_H}$")

set(H_CPP "(${H}|${CPP})")

source_group("Header Files\\Detail" "${CMAKE_BINARY_DIR}/gen.*/${H}")
source_group("Source Files\\Detail" "${CMAKE_BINARY_DIR}/gen.*/${CPP}")
source_group("Header Files\\Interface" "${SSRC}/include/Polygon4/DataManager/Interface/${H_CPP}")
source_group("MOC files" ".*automoc\\.cpp")

# sources
file(GLOB db_inc1 "${PROJECT_SOURCE_DIR}/include/Polygon4/DataManager/*")
file(GLOB db_inc2 "${PROJECT_SOURCE_DIR}/include/Polygon4/DataManager/Interface/*")
file(GLOB db_src "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

if (NOT POLYGON4_ENGINE)
    add_definitions(-DPOLYGON4_STATIC)
endif()

set(generated_src
    ${generated_dir_inc_detail}/ForwardDeclarations.h
    ${generated_dir_inc_detail}/Types.h
    ${generated_dir_src_detail}/Types.cpp
    ${generated_dir_inc_detail}/Storage.h
    ${generated_dir_src_detail}/Storage.cpp
    ${generated_dir_inc_detail}/StorageImpl.h
    ${generated_dir_src_detail}/StorageImpl.cpp
    ${generated_dir_src_detail}/schema/Tokens.cpp
    ${generated_dir_inc_detail}/ObjectInterfaces.h
    ${generated_dir_inc_detail}/Enums.h
    ${generated_dir_src_detail}/Enums.cpp
)
set_source_files_properties(${generated_src} PROPERTIES
    GENERATED True
    HEADER_FILE_ONLY True
)

add_custom_command(OUTPUT ${generated_src}
    COMMAND generator ${PROJECT_SOURCE_DIR}/data/schema.txt ${generated_dir}
    DEPENDS generator ${PROJECT_SOURCE_DIR}/data/schema.txt
)

set(DATA_MANAGER_ADDITIONAL_INCLUDES ${generated_dir_inc} CACHE STRING "Data manager generated includes dir" FORCE)

include_directories(${generated_dir_inc})
include_directories(${generated_dir_src})

add_library                 (DataManager ${db_src} ${db_inc1} ${db_inc2} ${generated_src} ${PROJECT_SOURCE_DIR}/data/schema.txt)
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
