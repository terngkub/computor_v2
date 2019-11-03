find_package(Boost)
if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(computorv2
    ast_adapted.hpp
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
    parser.hpp
    printer.cpp
    printer.hpp
  )
  target_link_libraries(computorv2 ${Boost_LIBRARIES})
endif()
