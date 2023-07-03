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
struct  Point
{
	double x =0.;
	double  y= 0.;
	double dist(const Point& other) const
	{
		return std::hypot(x - other.x, y - other.y);
	}
};


struct Vertex {	// Vertex of the graph used for Query process
	std::unordered_map <int, int> AdjList; // adjacency list
	long long d; // номер слоя // расстояние
	Point coord;
	int prev;

	Vertex() :d(Infinity), prev(-1), coord({0.,0.}) {}
};



class Graph
{
public:
	
	std::vector <Vertex> adj; 
	
	Graph();
	void Parse(std::istream& source = std::cin);
	void Process(std::istream& source = std::cin, std::ostream& output = std::cout);

	long long AStar(int s, int t);
	
	~Graph() = default;
private:
	void  Vipe(const std::vector <int>& changed); // откат параметров вершин к начальному состооянию

};


