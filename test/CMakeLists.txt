add_executable(${exe_test} ${sources})

set(test_dir ${CMAKE_CURRENT_LIST_DIR})
target_sources(${exe_test}
    PRIVATE
        ${test_dir}/main.cpp
        ${test_dir}/remote/utils/TestModelUtils.inl
        ${test_dir}/view/tools/TestStringTools.inl
)

set(googletest_dir ${CMAKE_SOURCE_DIR}/3rdparty/googletest)
target_include_directories(${exe_test}
        PRIVATE
            ${googletest_dir}/include
            ${CMAKE_SOURCE_DIR}/src/view/tools)
target_link_directories(${exe_test} PRIVATE ${googletest_dir}/lib)
target_link_libraries(${exe_test} PRIVATE ${exe_lib} gtest)