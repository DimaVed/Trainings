#include <cmath>
#include <assert.h>
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
using namespace   Catch::Matchers;


#define DECLARE_PROPERTY(name ,type)/
private:/
	type name;

 class cat {
		   DECLARE_PROPERTY(std::string, name);
};

TEST_CASE ("Preprocessor CodeGeneration", "[operator overloads]") {

	cat Vasyan;
	

}
