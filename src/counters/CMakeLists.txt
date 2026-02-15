add_library(counters counters.c)
target_include_directories(counters PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
message(STATUS "(${PROJECT_NAME}) counters configured: Added as static library")
