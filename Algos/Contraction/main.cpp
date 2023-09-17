#include "Contraction.h"
#include "test_runner.h"
#include "profile.h"

void TestLineGraphAfterContraction() {
  std::ifstream graph_file("Test6.txt");// One direction line graph
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  AssertEqual(g.ComputeDistance(1, 5), 2, " 0 to 0");

}

void Test12VertextGpraph() {
  std::ifstream graph_file("Test1.txt");
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  AssertEqual(g.ComputeDistance(0, 0), 0, " 0 to 0");
  AssertEqual(g.ComputeDistance(1, 1), 0, " 1 to 1");
  AssertEqual(g.ComputeDistance(0, 1), 1, " 0 to 1");
  AssertEqual(g.ComputeDistance(1, 0), -1, " 1 to 0");
}
void Test2() {
  std::ifstream graph_file("Test2.txt");
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  AssertEqual(g.ComputeDistance(0, 2), 3, " 0 to 2");
}
void Test3() {
  std::ifstream graph_file("Test3.txt");
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  AssertEqual(g.ComputeDistance(2, 0), 1, " 0to 0");
  AssertEqual(g.ComputeDistance(3, 0), 2, " 3 to 0");
}

void Test4() {
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
  AssertEqual(g.ComputeDistance(22, 88), 1616, " 0 to 0");
  AssertEqual(g.ComputeDistance(16, 56), 4633, " 3 to 0");
  AssertEqual(g.ComputeDistance(13, 84), 4534, " 13 to 84");
  AssertEqual(g.ComputeDistance(24, 8), 11574, " 13 to 84");
}

void TestNewYork() {
  std::ifstream graph_file("NYGRAPH.txt");
  Graph g;
  g.Parse(graph_file);
  std::cout << "Start NewYork Test" << std::endl;

  LOG_DURATION("Preprocess")
  {
    g.Preprocess();
  }
  AssertEqual(g.ComputeDistance(0, 1), 803, " 1 to 2");

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

void Test5() {
  std::ifstream graph_file("Test5.txt");
  Graph g;
  g.Parse(graph_file);
  g.Preprocess();
  AssertEqual(g.ComputeDistance(3, 2), 2, " 3 to 2");
}

void Test6() {
  std::ifstream graph_file("Test5.txt");
  Graph g;
  g.Parse(graph_file);

  g.Preprocess();
  AssertEqual(g.ComputeDistance(3, 2), 2, " 3 to 2");
}

int main() {
  std::cin.tie(nullptr);
  std::ios::sync_with_stdio(false);
  TestRunner tr;
  tr.RunTest(Test5, "Test5");
  tr.RunTest(Test6, "Test6");
  tr.RunTest(TestLineGraphAfterContraction, "TestLineGraph");
  tr.RunTest(Test12VertextGpraph, "Test1");
  tr.RunTest(Test2, "Test2");
  tr.RunTest(Test3, "Test3");
  tr.RunTest(Test4, "Test4");
  tr.RunTest(TestNewYork, "NewYorkTest");
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