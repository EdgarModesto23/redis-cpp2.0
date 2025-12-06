add_subdirectory(commands)
add_subdirectory(resp)
add_subdirectory(server)

file(GLOB_RECURSE SOURCE_FILES ./*.cc)

add_executable(redis ${SOURCE_FILES})

add_library(database database.cc)

target_include_directories(database PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(database PRIVATE asio::asio)
target_link_libraries(redis
    PRIVATE
        resp
        commands
        server
        database
        asio::asio
        Threads::Threads
        spdlog::spdlog_header_only
)

