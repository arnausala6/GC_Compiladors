add_library(token_model token_model.c)
target_include_directories(token_model PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/../scanner
)
target_link_libraries(token_model PUBLIC counters)
message(STATUS "(${PROJECT_NAME}) token_model configured: Added as static library")
