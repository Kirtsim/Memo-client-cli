cmake_minimum_required (VERSION 3.0)

project (Memo-server LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17 -g -Wall -Wextra -Wsign-conversion)

include("src/model/CMakeLists.txt")
# Use the "gold-linker" to speed-up the linking phase
if (UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif()

# variable denoting where the "absolute" include paths start in
set(BASEPATH "${CMAKE_SOURCE_DIR}/src/")

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# OpenSSL
if (APPLE)
    # This is a bug in CMake that causes it to prefer the system version over
    # the one in the specified ROOT folder.
    set(OPENSSL_ROOT_DIR ${OPENSSL_ROOT_DIR} /usr/local/opt/openssl)
    set(OPENSSL_CRYPTO_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libcrypto.dylib CACHE FILEPATH "" FORCE)
    set(OPENSSL_SSL_LIBRARY ${OPENSSL_ROOT_DIR}/lib/libssl.dylib CACHE FILEPATH "" FORCE)
endif()
find_package(OpenSSL REQUIRED)

find_package(Threads REQUIRED)

# Find Protobuf installation
# Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf REQUIRED)
message(STATUS "Using protobuf ${protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
   find_program(_PROTOBUF_PROTOC protoc)
else()
   set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Find gRPC installation
# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
   find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
    set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Building server
add_executable(memo_client_cli
    ${SOURCES})

target_link_libraries(memo_client_cli
    MemoGrpcLib
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

# Link ncurses libs
set(CURSES_NEED_NCURSES TRUE)
set(curses_lib_dir)
set(curses_include_dir)
if (CURSES_DIR)
    message("Searching for ncurses in: " ${CURSES_DIR})
    find_library(NCURSES_LIB ncurses PATHS ${CURSES_DIR}/lib REQUIRED)
    find_library(MENU_LIB menu PATHS ${CURSES_DIR}/lib REQUIRED)
    set(curses_lib_dir ${CURSES_DIR}/lib)
    set(curses_include_dir ${CURSES_DIR}/include)
else()
    find_package(Curses REQUIRED)
    find_library(menu_lib menu REQUIRED)
    get_filename_component(curses_lib_dir ${menu_lib} DIRECTORY)
    get_filename_component(curses_dir ${curses_lib_dir} DIRECTORY)
    set(curses_include_dir ${curses_lib_dir}/include)
endif()

message("Linked ncurses library directory: " ${curses_lib_dir})
message("Ncurses include directory: " ${curses_include_dir})
target_include_directories(memo_client_cli PRIVATE ${curses_include_dir})
target_link_directories(memo_client_cli PRIVATE ${curses_lib_dir})
target_link_libraries(memo_client_cli ncurses menu)

# needed for "absolute" include paths
target_include_directories(memo_client_cli PRIVATE ${BASEPATH})
