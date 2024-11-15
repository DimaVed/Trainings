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
	size_t n, m;
	source >> n >> m;
	adj.resize(n);
	for (size_t i = 0; i < n; i++)
	{
		source >> adj[i].coord.x >> adj[i].coord.y;		
	}
	for (size_t i = 0; i < m; i++) {
		int x, y, w;
		source >> x >> y >> w;
		x--;
		y--;
		adj[x].AdjList[ y ] =w ;
		
	}
}

void Graph::Process(std::istream& source, std::ostream& output)
{
	size_t t;
	source >> t;
	for (size_t i = 0; i < t; ++i) {
		int u, v;
		source >> u >> v;
		output << AStar(u - 1, v - 1) << '\n';
	}

}




int64_t Graph::AStar(int s, int t){
	
	std::vector <int> changed;
	
	adj[s].d = 0;
	changed.push_back(s);
	std::priority_queue <DistTo, std::vector <DistTo>, std::greater<DistTo>> h;
	h.push({ s,0 });
	while (!h.empty()){
		auto u = h.top();
		h.pop();
		DistTo current{ u.vertex_,adj[u.vertex_].d };
		if (u < current) { continue;
}
		for (auto v : adj[u.vertex_].AdjList){

			if (adj[v.first].d > adj[u.vertex_].d + v.second)
			{
				adj[v.first].d = adj[u.vertex_].d + v.second;
				adj[v.first].prev = u.vertex_;
				int64_t new_cost = adj[v.first].d +  static_cast <int64_t>( adj[v.first].coord.Dist(adj[t].coord));
				changed.push_back(v.first);
				h.push({ v.first,new_cost });
			}
		}
		if (adj[t].prev> 0) { break;
}
	}
	auto current = t;
	int64_t dist = 0;
	while (current != s){
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



void Graph::Vipe(const std::vector<int>& changed){
	for (auto item : changed)
	{
		adj[item].d = Infinity;
		adj[item].prev = -1;
	}
}



