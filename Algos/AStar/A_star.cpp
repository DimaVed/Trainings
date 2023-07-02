#include "A_star.h"
#include "test_runner.h"
#include "profile.h"
#include <numeric>



bool operator < (const DistTo& lhs, const DistTo& rhs) {

	return lhs.Dist_ < rhs.Dist_;
}
bool operator > (const DistTo& lhs, const DistTo& rhs) {

	return lhs.Dist_ > rhs.Dist_;
}


Graph::Graph() {	
}

void Graph::Parse(std::istream& source)
{
	int n, m;
	source >> n >> m;
	adj.resize(n);
	for (int i = 0; i < n; i++)
	{
		int x, y;
		source >> adj[i].coord.x >> adj[i].coord.y;		
	}
	for (int i = 0; i < m; i++) {
		int x, y, w;
		source >> x >> y >> w;
		adj[x - 1].AdjList[ y - 1] =w ;
		
	}
}

void Graph::Process(std::istream& source, std::ostream& output)
{
	int t;
	source >> t;
	for (int i = 0; i < t; ++i) {
		int u, v;
		source >> u >> v;
		output << AStar(u - 1, v - 1) << '\n';
	}

}




long long Graph::AStar(int s, int t){
	
	std::vector <int> changed;
	
	adj[s].d = 0;
	changed.push_back(s);
	std::priority_queue <DistTo, std::vector <DistTo>, std::greater<DistTo>> H;
	H.push({ s,0 });
	while (!H.empty())
	{
		auto u = H.top();
		H.pop();
		DistTo current{ u.vertex_,(double)adj[u.vertex_].d };
		if (u < current) continue;
		for (auto v : adj[u.vertex_].AdjList)
		{

			if (adj[v.first].d > adj[u.vertex_].d + v.second)
			{
				adj[v.first].d = adj[u.vertex_].d + v.second;
				adj[v.first].prev = u.vertex_;
				double new_cost = (double)adj[v.first].d + adj[v.first].coord.dist(adj[t].coord);
				changed.push_back(v.first);
				H.push({ v.first,new_cost });
			}
		}
		if (adj[t].prev> 0) break;
	}
	auto current = t;
	long long dist = 0;
	while (current != s)
	{
		auto pi = adj[current].prev;
		if (pi < 0) {
			dist = -1;
			break;
		
		}
		else
		{
			dist += adj[pi].AdjList.at(current);
			current = pi;
		}
	}
	Vipe(changed);
	return dist;
}



void Graph::Vipe(const std::vector<int>& changed)
{
	for (auto item : changed)
	{
		adj[item].d = Infinity;
		adj[item].prev = -1;
	}
}



