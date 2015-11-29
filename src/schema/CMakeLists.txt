#
# Polygon-4 Data generator
#

set(BSRC ${CMAKE_CURRENT_BINARY_DIR})

set(_CPP ".*\\.cpp")
set(CPP "${_CPP}$")

set(_H "(.*\\.h|.*\\.hpp)")
set(H "${_H}$")

set(H_CPP "(${H}|${CPP})")

source_group("Generated" "${BSRC}/${H_CPP}")

BISON_TARGET(grammar
    "${CMAKE_CURRENT_SOURCE_DIR}/grammar.yy"
    "${CMAKE_CURRENT_BINARY_DIR}/grammar.cpp"
)
FLEX_TARGET(lexer
    "${CMAKE_CURRENT_SOURCE_DIR}/lexer.ll"
    "${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp"
    COMPILE_FLAGS --header-file="${CMAKE_CURRENT_BINARY_DIR}/lexer.h"
)
ADD_FLEX_BISON_DEPENDENCY(lexer grammar)

include_directories(${PROJECT_SOURCE_DIR}/include/Polygon4/DataManager/Schema)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(parser_src
    ${BISON_grammar_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
    ${CMAKE_CURRENT_BINARY_DIR}/lexer.h
)
set_source_files_properties(${CMAKE_CURRENT_BINARY_DIR}/lexer.h PROPERTIES GENERATED TRUE)

file(GLOB schema_inc "${PROJECT_SOURCE_DIR}/include/Polygon4/DataManager/Schema/*")
file(GLOB schema_src "*.cpp" "*.h")

set(schema_src ${schema_src}
    grammar.yy
    lexer.ll
    ${parser_src}
    ${SCHEMA_FILE}
)

add_library                 (schema ${schema_src} ${schema_inc})
target_link_libraries       (schema ${Boost_LIBRARIES})
if (SOLUTION_FOLDER)
    set_target_properties   (schema PROPERTIES FOLDER ${SOLUTION_FOLDER})
endif()
