#include "RawMemVector.h"
#include "NaiveVector.h"
#include <catch2/catch_test_macros.hpp>
#include <numeric>
#include <algorithm>


struct constructor_t {
	int val;
	static int created;
	constructor_t() {
		created++;
		val = 0;
	}
	constructor_t(int data) {
		created++;
		val = data;
	}
	~constructor_t (){
		created--;
	}



	
};


template <typename V>
void StandardCheck(V & v) {
	//constructor_t::created = 0;
	v.resize(10);
	std::iota(v.begin(), v.end(), -4);
	CHECK(v.size() == 10);
	CHECK(v[0] == -4);
	v.resize(5);
	CHECK(v.size() == 5);
	CHECK(v[0] == -4);
	v.reserve(1000);
	CHECK(v.capacity() == 1000);
	v.resize(0);
	CHECK(v.capacity() == 1000);
	CHECK(v.size() == 0);

	v.push_back(10);
	CHECK(v.size() == 1);
	CHECK(v.back() == 10);

	v.push_back(-4);
	CHECK(v.size() == 2);
	CHECK(v.back() == -4);
	
	v.push_back(-3);
	CHECK(v.size() == 3);
	CHECK(v.back() == -3);
}




TEST_CASE("Vectors RMV simple Test", "[Vectors]") {
	rmv::vector <int> v;
	StandardCheck(v);
}


TEST_CASE("Vectors Naive simple Test", "[Vectors]") {
	nv::vector <int> v;
	StandardCheck(v);
}
TEST_CASE("Vectors STL", "[Vectors]") {
	std::vector <int> v;
	StandardCheck(v);
}