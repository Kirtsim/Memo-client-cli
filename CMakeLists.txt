cmake_minimum_required (VERSION 3.19)

project (Memo-server LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17 -g -Wall -Wextra -Wsign-conversion)

# Generate the compile_commands.json file (for language servers)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use the "gold-linker" to speed-up linking phase
if (UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(memo_proto_lib_include_dir ${CMAKE_SOURCE_DIR}/Memo-proto/model)

set(exe memo_client_cli)
set(exe_test memo_client_cli_test)
set(exe_lib memo_client_cli_lib)
set(memo_proto_lib memo_proto_lib)
set(proto_src_dir ${CMAKE_SOURCE_DIR}/Memo-proto/model)
include_directories(src)

add_subdirectory(${CMAKE_SOURCE_DIR}/Memo-proto/)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/)
add_subdirectory(${CMAKE_SOURCE_DIR}/test/)
