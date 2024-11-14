#pragma once
#include <vector>
#include <iterator>
#include <iostream>
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
private:
	std::vector<Range<T>> pages;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
	return Paginator(begin(c), end(c), page_size);
}

int main() {

	std::vector <int> data{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto ranges = Paginate(data,4);
	for (Range<std::vector<int>::iterator> range : ranges) {
		for (auto item : range) {
			std::cout << item << " ";
		}
		std::cout << "\n";
	}

	return 0;

}
