
find_package(BLAS REQUIRED)

set(_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE _headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${_include_dir}/*.h*)

if(NOT MSVC)
add_compile_options($<$<CONFIG:Release>:-O3>)
endif()

add_executable(${PROJECT_NAME} ${_headers}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp)


target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${PROJECT_NAME} PRIVATE ${_include_dir})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${_include_dir}/stdafx.h)

SET(TEST_PROJECT_NAME "${PROJECT_NAME}-test")

add_executable(${TEST_PROJECT_NAME} ${_headers}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
)

target_compile_features(${TEST_PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${TEST_PROJECT_NAME} PRIVATE ${_include_dir})
target_precompile_headers(${TEST_PROJECT_NAME} PRIVATE ${_include_dir}/stdafx.h)
target_link_libraries(${TEST_PROJECT_NAME} PRIVATE BLAS::BLAS)

SET(TEST_SHTRASSEB_PROJECT_NAME "${PROJECT_NAME}-test-shtrassen")

add_executable(${TEST_SHTRASSEB_PROJECT_NAME} ${_headers}
    ${CMAKE_CURRENT_SOURCE_DIR}/src/test_shtrassen.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/matrix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vector.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp
)

target_compile_features(${TEST_SHTRASSEB_PROJECT_NAME} PRIVATE cxx_std_17)
target_include_directories(${TEST_SHTRASSEB_PROJECT_NAME} PRIVATE ${_include_dir})
target_precompile_headers(${TEST_SHTRASSEB_PROJECT_NAME} PRIVATE ${_include_dir}/stdafx.h)
unset(_include_dir)
target_link_libraries(${TEST_SHTRASSEB_PROJECT_NAME} PRIVATE BLAS::BLAS)

add_test(test_multiply_square_1 ${TEST_PROJECT_NAME} "256")
add_test(test_multiply_square_2 ${TEST_PROJECT_NAME} "128")
add_test(test_multiply_rectangle_1 ${TEST_PROJECT_NAME} "64" "32" "32" "12")
add_test(test_multiply_rectangle_2 ${TEST_PROJECT_NAME} "28" "11" "11" "4")
add_test(test_multiply_rectangle_3 ${TEST_PROJECT_NAME} "28" "20" "20" "2")
add_test(test_multiply_matrix_vector_1 ${TEST_PROJECT_NAME} "28" "11" "11" "1")
add_test(test_multiply_matrix_vector_2 ${TEST_PROJECT_NAME} "28" "20" "20" "1")

add_test(test_shtrassen_2 ${TEST_SHTRASSEB_PROJECT_NAME} "16")
add_test(test_shtrassen_3 ${TEST_SHTRASSEB_PROJECT_NAME} "64")
add_test(test_shtrassen_1 ${TEST_SHTRASSEB_PROJECT_NAME} "128")


