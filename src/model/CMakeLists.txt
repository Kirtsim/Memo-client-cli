cmake_minimum_required (VERSION 3.0)

project (MemoGrpcLib LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

# Use the "gold-linker" to speed-up the linking phase
if (UNIX AND NOT APPLE)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=gold")
endif()

# variable denoting where the "absolute" include paths start in
set(BASEPATH "${CMAKE_SOURCE_DIR}/src/")

set(PROTO_SVC_FILES TagSvc.proto
                    MemoSvc.proto)
set(PROTO_FILES     Tag.proto
                    Memo.proto
                    OperationStatus.proto)
list(APPEND PROTO_FILES ${PROTO_SVC_FILES})

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

# Building the lib
add_library(MemoGrpcLib
    ${PROTO_SRCS}
    ${GRPC_SRCS})

target_compile_options(MemoGrpcLib PRIVATE -std=c++17 -g -w)
target_link_libraries(MemoGrpcLib
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})
