add_library(output_writer output_writer.c)

target_include_directories(output_writer PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../token_model
)

target_link_libraries(output_writer PUBLIC token_model)

message(STATUS "(${PROJECT_NAME}) output_writer configured: Added as static library")
