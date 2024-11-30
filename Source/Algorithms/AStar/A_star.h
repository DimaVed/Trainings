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



const int64_t Infinity = std::numeric_limits<int64_t>::max();
struct DistTo { // simple structure used in Dijkstras algorithms, sorted by dist
	DistTo() : vertex_(0), Dist_(0) {};
	DistTo(int v, int64_t d) :vertex_(v), Dist_(d) {};
	int vertex_;
	int64_t Dist_;
};
struct  Edge { // Used for shorcut searching
	int from_;
	int to_;
	int64_t weight_;
	Edge(int from, int to, int64_t weight) : from_(from), to_(to), weight_(weight) {};
	Edge() :from_(-1), to_(-1), weight_(0) {};
};
using Importance =DistTo; // data structure used in Contraction importance PriorityQue

bool operator < (const DistTo& lhs, const DistTo& rhs);
bool operator > (const DistTo& lhs, const DistTo& rhs);
struct  Point
{
	double x =0.;
	double  y= 0.;
	double Dist(const Point& other) const
	{
		return std::hypot(x - other.x, y - other.y);
	}
};


struct Vertex {	// Vertex of the graph used for Query process
	std::unordered_map <int, int> AdjList; // adjacency list
	int64_t d;
	Point coord;
	int prev;

	Vertex() :d(Infinity), coord({0.,0.}), prev(-1) {}
};



class Graph
{
public:
	
	std::vector <Vertex> adj; 
	
	Graph();
	void Parse(std::istream& source = std::cin);
	void Process(std::istream& source = std::cin, std::ostream& output = std::cout);

	int64_t AStar(int s, int t);
	
	~Graph() = default;
private:
	void  Vipe(const std::vector <int>& changed); // ����� ���������� ������ � ���������� ����������

};


