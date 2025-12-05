
file(GLOB_RECURSE RESP_FILES ./*.cc)

add_library(commands STATIC ${RESP_FILES})

target_include_directories(commands INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(commands PUBLIC database server spdlog::spdlog_header_only resp)
