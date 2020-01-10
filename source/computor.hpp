#pragma once
#include "ast.hpp"
#include "evaluator.hpp"

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