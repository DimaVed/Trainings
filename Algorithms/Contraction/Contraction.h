#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <numeric>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include<fstream>
#include <algorithm>
#include "test_runner.h"
#include "profile.h"

const int64_t kInfinity = 9223372036854775807;
struct DistTo { // simple structure used in Dijkstras algorithms, sorted by dist
  DistTo() : vertex_(0), Dist_(0) {};
  DistTo(int v, int64_t d) : vertex_(v), Dist_(d) {};
  size_t vertex_;
  int64_t Dist_;
};
struct Edge { // Used for shorcut searching
  int from_;
  int to_;
  int64_t weight_;
  Edge(int from, int to, int64_t weight) : from_(from), to_(to), weight_(weight) {};
  Edge() : from_(-1), to_(-1), weight_(0) {};
};
using Importance = DistTo; // data structure used in Contraction importance PriorityQue

bool operator<(const DistTo &lhs, const DistTo &rhs);
bool operator>(const DistTo &lhs, const DistTo &rhs);

struct EVertex {    // Vertex of the graph used for Query process
  std::vector<std::pair<int, int>> AdjList; // adjacency list
  int64_t d; // ����� ���� // ����������
  EVertex() : d(kInfinity) {}
};

struct Vertex {     // Vertex of the graph used for Preprocess
  std::unordered_map<int, int64_t> outgoing;
  std::unordered_map<int, int64_t> incoming;
  int64_t d; // distance
  int hop;
  int rank;
  int level;
  int ContractedNeigbours;
  int prev;
  Vertex() : d(kInfinity), hop(0), rank(0), level(0), ContractedNeigbours(0), prev(-1) {}
};

class Graph {
 public:
  const int max_hop;
  int rank;
  std::vector<Vertex> adj; // �������� ���� �� �������� ��������� ���� � ���� ��������������
  std::vector<Vertex> AugmentedGraph;// ����������� ���� �� ��������
  std::vector<EVertex> forward;
  std::vector<EVertex> backward;

  Graph();
  void Parse(std::istream &source = std::cin);
  void Preprocess();

  int64_t ComputeDistance(int s, int t);
  ~Graph() = default;
 private:
  void Vipe(const std::vector<int> &changed); // ����� ���������� ������ � ���������� ����������
  void VipeForward(const std::vector<int> &changed);
  void VipeBackward(const std::vector<int> &changed);
  int GetImportance(int v);
  void MakeOrderGraphs();
  int GetEdgeDiffernce(int v);
  int GetContractedNeighbours(int v);
  int GetLevel(int v);
  std::vector<Edge> FindShorcuts(int v);
  void ContractNode(int v);
  int64_t CalcMaxIncomeOutDist(int v);
  void AddEdge(const Edge &e);
  std::unordered_set<int> SearchWitnessPath(int s,
                                            int vert,
                                            const std::unordered_map<int, int64_t> &ws,
                                            int64_t max_dist);
};


