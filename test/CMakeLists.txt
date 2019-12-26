include_directories(
    ${computorv2_SOURCE_DIR}/source
    ${Boost_INCLUDE_DIRS}
)

add_executable(computorv2_test
    complex_test.cpp
    evaluator_test.cpp
    expr_test.cpp
    main.cpp
    math_test.cpp
    matrix_test.cpp
    term_test.cpp
)

target_link_libraries(computorv2_test
    computor
    ${Boost_LIBRARIES}
)
