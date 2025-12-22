cmake_minimum_required(VERSION 3.13)

project(redis)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Threads REQUIRED)
find_package(asio CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

include(FetchContent)
FetchContent_Declare(
    argparse
    GIT_REPOSITORY https://github.com/p-ranav/argparse.git
)
FetchContent_MakeAvailable(argparse)

add_subdirectory(src)

