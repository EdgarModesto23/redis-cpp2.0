file(GLOB_RECURSE SERVER_FILES ./*.cc)

add_library(server STATIC ${SERVER_FILES})

target_include_directories(server INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(server PRIVATE asio::asio spdlog::spdlog_header_only database resp commands)
