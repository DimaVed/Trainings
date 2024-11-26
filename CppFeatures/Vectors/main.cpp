#include "RawMemVector.h"
#include "NaiveVector.h"
#include <catch2/catch_test_macros.hpp>
#include <numeric>
#include <algorithm>


struct constructor_t {
	 int val;
	 inline static int created;
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


template <typename V>
void ConstructDestrucCheck(V& v) {
	constructor_t::created = 0;
	v.resize(10);
	CHECK(v.size() == 10);
	CHECK(constructor_t::created == 10);
	v.resize(0);
	CHECK(v.size() == 0);

	//должен
	CHECK(constructor_t::created == 0);

	v.reserve(10);
	CHECK(v.capacity() == 10);
	CHECK(constructor_t::created == 0);

}

template <typename V>
void MoveCopyCheck(V& v) {
	  V v1{ 1,2,3,4,5,6,7,8,9,10 };

	// Работа копирующего конструктора копирования
	auto v2 = v;
	CHECK(v2.size() == 10);
	CHECK(v2.capacity() == 10);
	for (int i = 0; i < v.size(); ++i) {
		v[i] = v2[i];
	}
	// Работа копирующего оператора присваивания
	V v3;
	v3 = v2;
	CHECK(v2.size() == 10);
	CHECK(v2.capacity() == 10);
	for (int i = 0; i < v.size(); ++i) {
		v[i] = v2[i];
	}

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


TEST_CASE("Vectors Naive  Construc/Destruct", "[Vectors]") {
	// Это тест илюстрирует недееспособность наивного подхода
	// конструкторы и деструкторы работают не так как ожидается
	nv::vector <constructor_t> v;
	//ConstructDestrucCheck(v);
}

TEST_CASE("Vectors RMV  Construc/Destruct", "[Vectors]") {
	rmv::vector <constructor_t> v;
	ConstructDestrucCheck(v);
}
TEST_CASE("Vectors STL  Construc/Destruct", "[Vectors]") {
	rmv::vector <constructor_t> v;
	ConstructDestrucCheck(v);
}
TEST_CASE("Vectors Naive  MoveCopyCheck", "[Vectors]") {
	nv::vector <int> v;
	
	MoveCopyCheck(v);
}