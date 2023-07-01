#include "A_star.h"
#include "test_runner.h"
#include "profile.h"
#include <filesystem>
namespace fs = std::filesystem;
void Test1()
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
void Test2()
{
	std::ifstream source("AStar02");
	std::ofstream out("AStar02.a");
	Graph g;
	g.Parse(source);
	g.Process(source, out);
}
void Test3()
{
	std::ifstream source("AStar03");
	std::ofstream out("AStar03.a");
	Graph g;
	g.Parse(source);
	g.Process(source, out);
}


int main() {
	std::cin.tie(nullptr);
	std::ios::sync_with_stdio(false);

	TestRunner Tr;
	Tr.RunTest(Test1, "Test1");
	Tr.RunTest(Test2, "Test2");
	Tr.RunTest(Test3, "Test3");
	Graph g;
	g.Parse();

	g.Process();
	
	return 0;
}