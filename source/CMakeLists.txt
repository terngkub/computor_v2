find_package(Boost 1.71.0)
if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  add_executable(computorv2
    ast_adapted.hpp
    ast.hpp
    computor.cpp
    computor.hpp
    evaluator.cpp
    evaluator.hpp
    main.cpp
    parser.hpp
    printer.cpp
    printer.hpp
  )
  target_link_libraries(computorv2 ${Boost_LIBRARIES})
endif()
