#pragma once
#include <vector>
#include <iterator>
#include <iostream>
#include <catch2/catch_test_macros.hpp>
//simple range class




template <typename It>
class Range {
public:
  using ValueType = typename std::iterator_traits<It>::value_type;

  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }

private:
  It begin_;
  It end_;
};

template <typename C>
auto AsRange(const C& container) {
  return Range{std::begin(container), std::end(container)};
}

template <typename T>
class Paginator {


public:
	Paginator(T begin, T end, size_t page_size) {
		for (size_t left = distance(begin, end); left > 0; ) {
			size_t current_page_size = (std::min)(page_size, left);
			T current_page_end = next(begin, current_page_size);
			pages.push_back({ begin, current_page_end });

			left -= current_page_size;
			begin = current_page_end;
		}
	}

	auto begin() const {
		return pages.begin();
	}

	auto end() const {
		return pages.end();
	}

	size_t size() const {
		return pages.size();
	}
	Range<T> operator [](size_t i) const {
		return pages[i];
	}
private:
	std::vector<Range<T>> pages;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
	return Paginator(begin(c), end(c), page_size);
}

TEST_CASE ("RangePaginataion", "[pagination]") {

	std::vector <int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto ranges = Paginate(data,4);
	REQUIRE(ranges.size() == 3);
	CHECK(std::vector <int> {ranges[0].begin(), ranges[0].end() } == std::vector <int> {1, 2, 3, 4});
	CHECK(std::vector <int> {ranges[1].begin(), ranges[1].end() } == std::vector <int> {5, 6, 7, 8});
	CHECK(std::vector <int> {ranges[2].begin(), ranges[2].end() } == std::vector <int> {9, 10});



}

