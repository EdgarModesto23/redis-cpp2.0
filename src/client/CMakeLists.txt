file(GLOB_RECURSE CLIENT_FILES ./*.cc)

add_library(client STATIC ${CLIENT_FILES})

target_include_directories(client INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(client PRIVATE asio::asio server spdlog::spdlog_header_only resp commands)
