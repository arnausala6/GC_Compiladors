add_library(tokenlist tokenlist.c)

target_link_libraries(tokenlist PUBLIC language)

target_include_directories(tokenlist PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)