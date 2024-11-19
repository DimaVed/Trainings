#include "A_star.h"
#include <filesystem>
#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

namespace fs = std::filesystem;
TEST_CASE ( "AStar Test1", "[Astar]")
{
	std::ifstream source("AStar01");
	std::ofstream out("AStar01.a");
	Graph g;
	g.Parse(source);
	g.Process(source,out);
	out.close();
	//fs::path p1  ("AStar01.a");
	//fs::path p2("AStarCorrect01.a");
	
}
TEST_CASE("AStar Test2", "[Astar]")
{
	std::ifstream source("AStar02");
	std::ofstream out("AStar02.a");
	Graph g;
	g.Parse(source);
	g.Process(source, out);
}

TEST_CASE("AStar Test3", "[Astar]")

{
	std::ifstream source("AStar03");
	std::ofstream out("AStar03.a");
	Graph g;
	g.Parse(source);
	g.Process(source, out);
}

#ifdef false


int main() {
	std::cin.tie(nullptr);
	std::ios::sync_with_stdio(false);

	TestRunner tr;
	
	Graph g;
	g.Parse();

	g.Process();
	
	return 0;
}
#endif 