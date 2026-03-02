add_library(token_model token_model.c)
target_include_directories(token_model PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
)
