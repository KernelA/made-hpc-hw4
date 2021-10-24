
find_package(BLAS REQUIRED)
find_package(OpenMP REQUIRED)

set(_include_dir ${CMAKE_CURRENT_SOURCE_DIR}/include)

file(GLOB_RECURSE _headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${_include_dir}/*.h*)


set(MAIN_FILE "main_page_rank;main_power;test_mult;test_shtrassen;test_power;test_transpose")


foreach(target ${MAIN_FILE})
    message(STATUS "Add target for: ${target}")
    add_executable(${target} ${_headers}
        ${CMAKE_CURRENT_SOURCE_DIR}/src/${target}.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/graph.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/page_rank.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/utils.cpp)

    target_compile_features(${target} PRIVATE cxx_std_17)
    target_include_directories(${target} PRIVATE ${_include_dir})
    target_precompile_headers(${target} PRIVATE ${_include_dir}/stdafx.h)
    target_link_libraries(${target} PRIVATE OpenMP::OpenMP_CXX)

    if(${target} STREQUAL "test_mult")
        target_link_libraries(${target} PRIVATE BLAS::BLAS)
    endif()
endforeach()

unset(_include_dir)

add_test(test_multiply_square_1 "test_mult" "3")
add_test(test_multiply_square_2 "test_mult" "4")
add_test(test_multiply_square_3 "test_mult" "5")
add_test(test_multiply_square_4 "test_mult" "7")
add_test(test_multiply_rectangle_1 "test_mult" "64" "32" "32" "12")
add_test(test_multiply_rectangle_2 "test_mult" "28" "11" "11" "4")
add_test(test_multiply_rectangle_3 "test_mult" "28" "20" "20" "2")
add_test(test_multiply_matrix_vector_1 "test_mult" "28" "11" "11" "1")
add_test(test_multiply_matrix_vector_2 "test_mult" "28" "20" "20" "1")

add_test(test_shtrassen_2 "test_shtrassen" "16")
add_test(test_shtrassen_3 "test_shtrassen" "64")
add_test(test_shtrassen_1 "test_shtrassen" "128")

add_test(test_matrix_power_even "test_power" "32" "2")
add_test(test_matrix_power_odd_1 "test_power" "32" "7")
add_test(test_matrix_power_odd_2 "test_power" "32" "9")
add_test(test_matrix_power_one "test_power" "4" "1")
add_test(test_matrix_power_even_1 "test_power" "32" "4")
add_test(test_matrix_power_odd_3 "test_power" "32" "3")
add_test(test_matrix_power_even_2 "test_power" "3" "12")

add_test(test_transpose_1 "test_transpose" "16")
add_test(test_transpose_2 "test_transpose" "32")
add_test(test_transpose_3 "test_transpose" "64")


