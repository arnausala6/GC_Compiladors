add_library(automata_engine automata_engine.c)
target_include_directories(automata_engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "(${PROJECT_NAME}) automata_engine configured: Added as static library")
