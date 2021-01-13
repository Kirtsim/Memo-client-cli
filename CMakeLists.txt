cmake_minimum_required (VERSION 3.0)

project (Memo-server LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++17 -g -Wall -Wextra -Wsign-conversion)

# Use the "gold-linker" to speed-up the linking phase
if (UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif()

# variable denoting where the "absolute" include paths start in
set(BASEPATH "${CMAKE_SOURCE_DIR}/src/")

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)


set(PROTO_SVC_FILES TagSvc.proto
                    MemoSvc.proto)
set(PROTO_FILES     Tag.proto
                    Memo.proto
                    OperationStatus.proto)
list(APPEND PROTO_FILES ${PROTO_SVC_FILES})


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

# Generated sources
set(PROTO_SRCS)
set(PROTO_HDRS)
set(GRPC_SRCS)
set(GRPC_HRDS)
set(PROTO_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/model)
set(PROTO_DIR ${PROTO_SRC_DIR}/proto)
foreach(proto_file ${PROTO_FILES})
    get_filename_component(file_name ${proto_file} NAME_WE)

    list(APPEND PROTO_SRCS "${PROTO_SRC_DIR}/${file_name}.pb.cc")
    list(APPEND PROTO_HDRS "${PROTO_SRC_DIR}/${file_name}.pb.h")
    add_custom_command(
          OUTPUT "${PROTO_SRC_DIR}/${file_name}.pb.cc"
                 "${PROTO_SRC_DIR}/${file_name}.pb.h"
          COMMAND ${_PROTOBUF_PROTOC}
          ARGS
            --cpp_out "${PROTO_SRC_DIR}"
            -I "${PROTO_DIR}"
            "${PROTO_DIR}/${proto_file}"
          DEPENDS "${PROTO_DIR}/${proto_file}")
endforeach()

foreach(proto_file ${PROTO_SVC_FILES})
    get_filename_component(file_name ${proto_file} NAME_WE)

    list(APPEND GRPC_SRCS "${PROTO_SRC_DIR}/${file_name}.grpc.pb.cc")
    list(APPEND GRPC_HDRS "${PROTO_SRC_DIR}/${file_name}.grpc.pb.h")

    add_custom_command(
          OUTPUT "${PROTO_SRC_DIR}/${file_name}.grpc.pb.cc"
                 "${PROTO_SRC_DIR}/${file_name}.grpc.pb.h"
          COMMAND ${_PROTOBUF_PROTOC}
          ARGS --grpc_out "${PROTO_SRC_DIR}"
               --cpp_out "${PROTO_SRC_DIR}"
               -I "${PROTO_DIR}"
               --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
                "${PROTO_DIR}/${proto_file}"
          DEPENDS "${PROTO_DIR}/${proto_file}")
endforeach()

# Include generated *.pb.h files
include_directories("${PROTO_SRC_DIR}")
message("Proto src directory: ${PROTO_SRC_DIR}")

# Building server
add_executable(memo_client_cli
    ${SOURCES}
    ${PROTO_SRCS}
    ${GRPC_SRCS})

target_link_libraries(memo_client_cli
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
