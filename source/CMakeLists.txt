include_directories(${Boost_INCLUDE_DIRS})

add_library(computor
  ast.hpp
  computor.cpp
  computor.hpp
  complex.cpp
  complex.hpp
  evaluator.cpp
  evaluator.hpp
  expr.cpp
  expr.hpp
  function.cpp
  function.hpp
  math.cpp
  math.hpp
  matrix.cpp
  matrix.hpp
  parser.hpp
  term.cpp
  term.hpp
  utils.hpp
)

add_executable(computorv2
  main.cpp
)

target_link_libraries(computorv2 
  computor
  ${Boost_LIBRARIES}
)
