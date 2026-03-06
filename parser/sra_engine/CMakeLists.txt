add_library(sra_engine STATIC sra_engine.c)
target_include_directories(sra_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sra_engine PUBLIC automata stack parser_debug tokenlist)
