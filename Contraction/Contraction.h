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


const long long Infinity = 9223372036854775807;
struct DistTo { // simple structure used in Dijkstras algorithms, sorted by dist
	DistTo() : vertex_(0), Dist_(0) {};
	DistTo(int v, long long d) :vertex_(v), Dist_(d) {};
	int vertex_;
	long long Dist_;
};
struct  Edge { // Used for shorcut searching
	int from_;
	int to_;
	long long weight_;
	Edge(int from, int to, long long weight) : from_(from), to_(to), weight_(weight) {};
	Edge() :from_(-1), to_(-1), weight_(0) {};
};
using Importance =DistTo; // data structure used in Contraction importance PriorityQue

bool operator < (const DistTo& lhs, const DistTo& rhs);
bool operator > (const DistTo& lhs, const DistTo& rhs);

struct EVertex {	// Vertex of the graph used for Query process
	std::vector <std::pair <int, int>> AdjList; // adjacency list
	long long d; // номер слоя // расстояние
	EVertex() :d(Infinity) {}
};

struct Vertex {	 // Vertex of the graph used for Preprocess
	std::unordered_map <int, long long> outgoing;
	std::unordered_map <int, long long> incoming;
	long long d; // distance
	std::pair <long long, long long > MaxEdges;
	int hop;
	int rank;
	int level;
	int ContractedNeigbours;
	int prev;
	Vertex() :hop(0), rank(0), prev(-1), ContractedNeigbours(0), level(0), d(Infinity) {}
};

class Graph
{
public:
	const int max_hop;
	int rank;
	std::vector <Vertex> adj; // исходный граф из которого удаляются узлы в ходе препроцессинга
	std::vector <Vertex> AugmentedGraph;// Расширенный граф со срезками
	std::vector <EVertex> forward;
	std::vector <EVertex> backward;

	Graph();
	void Parse(std::istream& source = std::cin);
	void Preprocess(); void  Vipe(); // откат параметров вершин к начальному состооянию

	long long ComputeDistance(int s, int t);
	~Graph() = default;
private:
	void  Vipe(const std::vector <int>& changed); // откат параметров вершин к начальному состооянию
	void VipeForward(const std::vector <int>& changed);
	void VipeBackward(const std::vector <int>& changed);
	int GetImportance(int v);
	void MakeOrderGraphs();
	int GetEdgeDiffernce(int v);
	int GetContractedNeighbours(int v);
	int GetLevel(int v);
	std::vector<Edge>  FindShorcuts(int v);
	std::unordered_set<int> SearchWitnessPath(int u, int v, const std::unordered_map<int, long long>& ws, long long max_dist);
	void ContractNode(int v);
	long long CalcMaxIncomeOutDist(int v);
	void AddEdge(const Edge& e);
};


