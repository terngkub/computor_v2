#pragma once
#include <unordered_map>
#include "ast.hpp"
#include "evaluator.hpp"
#include "printer.hpp"

namespace computorv2
{

class computor
{
public:
	computor() = default;
	void operator()();

private:
	evaluator evaluate;
};

} // namespace computorv2