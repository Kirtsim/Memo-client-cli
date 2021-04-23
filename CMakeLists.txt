cmake_minimum_required (VERSION 3.0)

project (Memo-server LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17 -g -Wall -Wextra -Wsign-conversion)

# Generate the compile_commands.json file (for language servers)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Use the "gold-linker" to speed-up linking phase
if (UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif()

set(base_path "${CMAKE_SOURCE_DIR}/src/")
add_subdirectory(${base_path})

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")