#include <cmath>
#include <assert.h>

#pragma warning( push )
#pragma warning( disable : 26812)
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#pragma warning( pop ) 
using namespace   Catch::Matchers;


#define DECLARE_PROPERTY(type ,name)\
private: \
type name##_{};\
public:\
const type& get_##name() const { return name##_; } \
void set_##name(const type& v) { name##_ = v; }

 class cat {
DECLARE_PROPERTY(std::string, name);
DECLARE_PROPERTY(int, age);
};

TEST_CASE ("Preprocessor PropertyMacro", "[Preprocessor]") {

	cat Vasyan;
	Vasyan.set_name("Vasyan");
	Vasyan.set_age(10);
	CHECK(Vasyan.get_age() == 10);
	CHECK(Vasyan.get_name() == "Vasyan");
	

}

TEST_CASE("Preprocessor BadMacro", "[Preprocessor]") {

// Не тот макрос подъехал с какой-то библиотекой 
#define Sqr(q) q*q

	CHECK(Sqr(3) == 9);
	CHECK(Sqr(3+1)!= 16);

// сделать stash кривого макрос
#pragma push_macro ("Sqr")
// Переорпределили макрос
#ifdef  Sqr
#undef Sqr
#define  Sqr(q) ((q)*(q))
#endif //  Sqr

	CHECK(Sqr(3+2) == 25);
	CHECK(Sqr(3 + 1) == 16);

#pragma pop_macro ("Sqr")
	CHECK(Sqr(3) == 9);
	CHECK(Sqr(3 + 1) == 16);
}

TEST_CASE("Preprocessor Warning Supress", "[Preprocessor]") {

	int i = 0;
	size_t b = 10;

// В проекте выставлен флаг warning as error 

#pragma warning( push )
#pragma warning( disable : 4267)
	i = b;
#pragma warning( pop )
	CHECK(i == b);
	
}