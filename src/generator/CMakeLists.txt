#
# Polygon-4 Data generator
#

set(generator_src
    main.cpp
)

add_executable              (generator ${generator_src})
target_link_libraries       (generator schema)
if (SOLUTION_FOLDER)
    set_target_properties   (generator PROPERTIES FOLDER ${SOLUTION_FOLDER})
endif()
