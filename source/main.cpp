#include "computor.hpp"

int main()
{
	try
	{
		computorv2::computor{}();
	}
	catch(const std::exception & e)
	{
		std::cerr << e.what() << '\n';
	}
}