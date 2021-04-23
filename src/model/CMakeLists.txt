set(proto_svc_files TagSvc.proto
                    MemoSvc.proto)
set(proto_files     Tag.proto
                    Memo.proto
                    OperationStatus.proto)
list(APPEND proto_files ${proto_svc_files})

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
set(proto_srcs)
set(proto_hdrs)
set(grpc_srcs)
set(grpc_hdrs)
set(models_dir ${CMAKE_CURRENT_LIST_DIR})
set(proto_dir ${models_dir}/proto)
foreach(proto_file ${proto_files})
    get_filename_component(file_name ${proto_file} NAME_WE)

    list(APPEND proto_srcs "${models_dir}/${file_name}.pb.cc")
    list(APPEND proto_hdrs "${models_dir}/${file_name}.pb.h")
    add_custom_command(
          OUTPUT "${models_dir}/${file_name}.pb.cc"
                 "${models_dir}/${file_name}.pb.h"
          COMMAND ${_PROTOBUF_PROTOC}
          ARGS
            --cpp_out "${models_dir}"
            -I "${proto_dir}"
            "${proto_dir}/${proto_file}"
          DEPENDS "${proto_dir}/${proto_file}")
endforeach()

foreach(proto_file ${proto_svc_files})
    get_filename_component(file_name ${proto_file} NAME_WE)

    list(APPEND grpc_srcs "${models_dir}/${file_name}.grpc.pb.cc")
    list(APPEND GRPC_HDRS "${models_dir}/${file_name}.grpc.pb.h")

    add_custom_command(
          OUTPUT "${models_dir}/${file_name}.grpc.pb.cc"
                 "${models_dir}/${file_name}.grpc.pb.h"
          COMMAND ${_PROTOBUF_PROTOC}
          ARGS --grpc_out "${models_dir}"
               --cpp_out "${models_dir}"
               -I "${proto_dir}"
               --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
                "${proto_dir}/${proto_file}"
          DEPENDS "${proto_dir}/${proto_file}")
endforeach()

# Include generated *.pb.h files
include_directories("${models_dir}")
message("Proto src directory: ${models_dir}")

# Building the lib
add_library(MemoGrpcLib "")
target_sources(MemoGrpcLib
    PRIVATE
        ${proto_srcs}
    PUBLIC
        ${grpc_srcs})

target_compile_options(MemoGrpcLib PRIVATE -std=c++17 -g -w)
target_link_libraries(MemoGrpcLib
    PUBLIC
        ${_REFLECTION}
        ${_GRPC_GRPCPP}
        ${_PROTOBUF_LIBPROTOBUF})
