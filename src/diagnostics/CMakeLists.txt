add_library(diagnostics diagnostics.c)
target_include_directories(diagnostics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../scanner)
message(STATUS "(${PROJECT_NAME}) diagnostics configured: Added as static library")
