#pragma once
#include <string>
#include <list>
#include <iterator>
#include <iostream>
#include <string_view>
#include <vector>
#include <sstream>
#include <catch2/catch_test_macros.hpp>


//stl algorithm style split alogrithm
template <typename InIt, typename OutIt, typename T, typename F>
InIt split(InIt it, InIt end_it, OutIt out_it, T split_val, F bin_func)
{
while (it != end_it) {
auto slice_end (find(it, end_it, split_val));
*out_it++ = bin_func(it, slice_end);
if (slice_end == end_it) { return end_it; }
it = next(slice_end);
}
return it;
}



std::string_view Strip(std::string_view s) {
    while (!s.empty() && isspace(s.front())) {
        s.remove_prefix(1);
    }
    while (!s.empty() && isspace(s.back())) {
        s.remove_suffix(1);
    }
    return s;
}
std::vector<std::string_view> SplitBy(std::string_view s, char sep) {
    std::vector<std::string_view> result;
    while (!s.empty()) {
        size_t pos = s.find(sep);
        result.push_back(s.substr(0, pos));
        s.remove_prefix(pos != s.npos ? pos + 1 : s.size());
    }
    return result;
}
std::vector<std::string> SplitIntoWords(const std::string& line) {
    std::istringstream words_input(line);
    return { std::istream_iterator<std::string>(words_input), std::istream_iterator<std::string>() };
}

TEST_CASE("Split Stl style split", "[Stl style]") {
    const std::string s{ "a-b-c-d-e-f-g" };
    auto binfunc([](auto it_a, auto it_b) {
        return std::string(it_a, it_b);
        });
    std::list<std::string> l;
    split(begin(s), end(s), std::back_inserter(l), '-', binfunc);
    std::list <std::string> ans{ "a", "b", "c", "d", "e", "f", "g" };
    CHECK(l == ans);
}

TEST_CASE( "Split Split into words", "[Words]") {
    std::vector <std::string> ans{ "a", "b", "c", "d", "e", "f", "g" };
    //Split by spaces (1 or more)
    const std::string spaces_s{ "  a b c   d   e f g" };
    auto words = SplitIntoWords(spaces_s);
   
    CHECK(words == ans);
}
TEST_CASE("Split String view split", "[string_view]") {
    const std::string s{ "a-b-c-d-e-f-g" };
    std::vector <std::string_view> ans{ "a", "b", "c", "d", "e", "f", "g" };
    // Stringview split algo
    auto res = SplitBy(s, '-');
  

    CHECK(res == ans);
}
