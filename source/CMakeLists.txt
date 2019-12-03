find_package(Boost)
if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(computorv2
    ast.hpp
    computor.cpp
    computor.hpp
    complex.cpp
    complex.hpp
    evaluator.cpp
    evaluator.hpp
    expr.cpp
    expr.hpp
    main.cpp
    math.cpp
    math.hpp
    matrix.cpp
    matrix.hpp
    parser.hpp
    printer.cpp
    printer.hpp
    term.cpp
    term.hpp
    utils.hpp
  )
  target_link_libraries(computorv2 ${Boost_LIBRARIES})
endif()
