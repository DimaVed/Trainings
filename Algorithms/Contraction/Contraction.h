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


const int64_t kInfinity = std::numeric_limits <int64_t>::max();
struct DistTo { // simple structure used in Dijkstras algorithms, sorted by dist
  DistTo() : vertex_(0), Dist_(0) {};
  DistTo(size_t v, int64_t d) : vertex_(v), Dist_(d) {};
  size_t vertex_;
  int64_t Dist_;
};
struct Edge { // Used for shorcut searching
  size_t from_;
  size_t to_;
  int64_t weight_;
  Edge(size_t from, size_t to, int64_t weight) : from_(from), to_(to), weight_(weight) {};
  Edge() : from_(0ull), to_(0ull), weight_(0) {};
};
using Importance = DistTo; // data structure used in Contraction importance PriorityQue

bool operator<(const DistTo &lhs, const DistTo &rhs);
bool operator>(const DistTo &lhs, const DistTo &rhs);

struct EVertex {    // Vertex of the graph used for Query process
  std::vector<std::pair<size_t, size_t>> AdjList; // adjacency list
  int64_t d; // расстояние то точки
  EVertex() : d(kInfinity) {}
};

struct Vertex {     // Vertex of the graph used for Preprocess
  std::unordered_map<size_t, int64_t> outgoing;
  std::unordered_map<size_t, int64_t> incoming;
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
  std::vector<Vertex> adj; // Исходный граф
  std::vector<Vertex> AugmentedGraph;// Модифицированный граф
  std::vector<EVertex> forward;
  std::vector<EVertex> backward;

  Graph();
  void Parse(std::istream &source = std::cin);
  void Preprocess();

  int64_t ComputeDistance(size_t s, size_t t);
  ~Graph() = default;
 private:
  void Vipe(const std::vector<int> &changed); 
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
  std::unordered_set<size_t> SearchWitnessPath(size_t s,
                                            int vert,
                                            const std::unordered_map<size_t, int64_t> &ws,
                                            int64_t max_dist);
};


