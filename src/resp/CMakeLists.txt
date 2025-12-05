file(GLOB_RECURSE RESP_FILES ./*.cc)

add_library(resp STATIC ${RESP_FILES})

target_include_directories(resp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(resp PRIVATE spdlog::spdlog_header_only)
