#include "Contraction.h"
#include "test_runner.h"
#include "profile.h"
#include <catch2/catch_test_macros.hpp>


TEST_CASE("TestLineGraphAfterContraction", "[Contraction]")
 {
  std::ifstream graph_file("Test6.txt");// One direction line graph
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  CHECK(g.ComputeDistance(1, 5)== 2);

}

TEST_CASE("Test12VertextGpraph", "[Contraction]") {
  std::ifstream graph_file("Test1.txt");
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  CHECK(g.ComputeDistance(0, 0) == 0);
  CHECK(g.ComputeDistance(1, 1) == 0);
  CHECK(g.ComputeDistance(0, 1)== 1);
  CHECK(g.ComputeDistance(1, 0)== -1);
}
TEST_CASE("Test2", "[Contraction]")
{
  std::ifstream graph_file("Test2.txt");
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  CHECK(g.ComputeDistance(0, 2)== 3);
}
TEST_CASE("Test3", "[Contraction]") {
  std::ifstream graph_file("Test3.txt");
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  CHECK(g.ComputeDistance(2, 0)== 1);
  CHECK(g.ComputeDistance(3, 0)== 2);
}

TEST_CASE("Test4", "[Contraction]") {
  std::ifstream graph_file("Test4.txt");
  Graph g;
  g.Parse(graph_file);
  LOG_DURATION("Preprocess")
  {
    g.Preprocess();
  }

  LOG_DURATION("WO")
  {
    std::ifstream query("Query.txt");

    int t;
    query >> t;

    std::ofstream answer("AnswerWO.txt");
    for (int i = 0; i < t; ++i) {
      int u, v;
      query >> u >> v;
      answer << g.ComputeDistance(u - 1, v - 1) << '\n';
    }
  }

  LOG_DURATION("WITH")
  {
    std::ifstream query("Query.txt");

    int t;
    query >> t;

    std::ofstream answer("AnswerWITH.txt");
    for (int i = 0; i < t; ++i) {
      int u, v;
      query >> u >> v;
      answer << g.ComputeDistance(u - 1, v - 1) << '\n';
    }
  }
  CHECK(g.ComputeDistance(22, 88)== 1616);
  CHECK(g.ComputeDistance(16, 56)== 4633);
  CHECK(g.ComputeDistance(13, 84)== 4534);
  CHECK(g.ComputeDistance(24, 8)== 11574);
}
TEST_CASE("TestNewYork", "[Contraction]")
 {
  std::ifstream graph_file("NYGRAPH.txt");
  Graph g;
  g.Parse(graph_file);
  std::cout << "Start NewYork Test" << std::endl;

  LOG_DURATION("Preprocess")
  {
    g.Preprocess();
  }
  CHECK(g.ComputeDistance(0, 1)== 803);

  LOG_DURATION("WO")
  {
    std::ifstream query("Query.txt");
    int t;
    query >> t;
    std::ofstream answer("AnswerWO.txt");
    for (int i = 0; i < t; ++i) {
      int u, v;
      query >> u >> v;
      answer << g.ComputeDistance(u - 1, v - 1) << '\n';
    }
  }

}

TEST_CASE("Test5", "[Contraction]") {
  std::ifstream graph_file("Test5.txt");
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  CHECK(g.ComputeDistance(3, 2)== 2);
}


#ifdef  false
int main() {
    std::cin.tie(nullptr);
    std::ios::sync_with_stdio(false);

    Graph g;
    g.Parse();
    g.Preprocess();
    std::cout << "Ready" << std::endl;
    int t;
    std::cin >> t;
    for (int i = 0; i < t; ++i) {
        int u, v;
        std::cin >> u >> v;
        std::cout << g.ComputeDistance(u - 1, v - 1) << '\n';
    }
}
#endif // 0

