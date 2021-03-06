#include "Contraction.h"
#include "test_runner.h"
#include "profile.h"



bool operator < (const DistTo& lhs, const DistTo& rhs) {

	return lhs.Dist_ < rhs.Dist_;
}
bool operator > (const DistTo& lhs, const DistTo& rhs) {

	return lhs.Dist_ > rhs.Dist_;
}


Graph::Graph():max_hop(5),rank(0) {
	
}

void Graph::Parse(std::istream& source)
{
	int n, m;
	source >> n >> m;
	adj.resize(n);
	AugmentedGraph.resize(n);
	for (int i = 0; i < m; i++) {
		int x, y, w;
		source >> x >> y >> w;
		adj[x - 1].outgoing[y - 1] = w;
		adj[y - 1].incoming[x - 1] = w;

		AugmentedGraph[x - 1].outgoing[y - 1] = w;
		AugmentedGraph[y - 1].incoming[x - 1] = w;
	}
}

void Graph::Preprocess()
{
	std::priority_queue <Importance, std::vector <Importance>, std::greater<Importance>> H;
	std::ofstream log("log.txt");

		for (int i = 0; i < adj.size(); i++)
		{
			H.push({i,GetImportance(i)});
		}



	
		while (!H.empty())
		{
			log << H.size() << '\n';
			auto least = H.top();
			H.pop();
			auto currentimportance = GetImportance(least.vertex_);
			if (H.empty())
			{
				ContractNode(least.vertex_);
			}
			else if (currentimportance > H.top().Dist_)
			{
				H.push({ least.vertex_, currentimportance });
			}
			else
			{
				ContractNode(least.vertex_);
			}


		}
	
	MakeOrderGraphs();

}

void Graph::Vipe()
{
	for (auto& item : adj)
	{
		item.d = Infinity; // номер слоя // расстояние
		item.hop = 0;
		item.prev = -1;
	}
	
}

void Graph::Vipe(const std::vector<int>& changed)
{
	for (auto item : changed)
	{
		adj[item].d = Infinity;
		adj[item].hop = 0;
		adj[item].prev = -1;

	}
}





void Graph::VipeForward(const std::vector<int>& changed)
{
	for (auto item : changed)
	{
		forward[item].d = Infinity;
		
		//forward[item].pi = -1;

	}
}

void Graph::VipeBackward(const std::vector<int>& changed)
{
	for (auto item : changed)
	{
		backward[item].d = Infinity;

	}
}

int Graph::GetImportance(int v)
{
	int res = 0;
	//res-= 0.5*adj[v].nei.size() * adj[v].pred.size();
	res+=  GetEdgeDiffernce(v);
	res += GetContractedNeighbours(v);
	res += GetLevel(v);
	//res += 10*GetShortcutCover(v);
	return res;
}

int Graph::GetEdgeDiffernce(int v)
{
	return  static_cast <int>(FindShorcuts(v).size()) -adj[v].outgoing.size()-adj[v].incoming.size();
}

int Graph::GetContractedNeighbours(int v)
{
	return adj[v].ContractedNeigbours;
	
}

int Graph::GetLevel(int v)
{
	return adj[v].level;
}


void Graph::MakeOrderGraphs(){
	forward.resize(AugmentedGraph.size());
	backward.resize(AugmentedGraph.size());
	for (int i =0;i< AugmentedGraph.size();i++)
	{
		for (auto item : AugmentedGraph[i].outgoing)
		{
			if (AugmentedGraph[i].rank < AugmentedGraph[item.first].rank)
			{

				forward[i].AdjList.push_back({ item.first,item.second });
			}
			else
			{
				backward[item.first].AdjList.push_back({ i,item.second });
				
			}
		}
	}
}

std::vector<Edge> Graph::FindShorcuts(int v){
	 std::vector<Edge> res;
	long long max_len = CalcMaxIncomeOutDist(v);
	for (auto pred : adj[v].incoming){
		auto WithessPaths =  SearchWitnessPath(pred.first, v, adj[v].outgoing, max_len);
		for (auto neig : adj[v].outgoing){
			if (!WithessPaths.count(neig.first) && neig.first != pred.first) {

			res.push_back(Edge({ pred.first,neig.first,adj[pred.first].outgoing[v] + adj[v].outgoing[neig.first] }));
			}
		}
	}
	return res;
}

 std::unordered_set<int> Graph::SearchWitnessPath(int s, int vert, const std::unordered_map<int, long long>& ws, long long max_dist){
	std::unordered_set<int> res;
	std::vector <int> ChangedNodes;
	int currenthops = 0;
	 std::vector <int> changed;
	 adj[s].d = 0;
	 changed.push_back(s);
	 std::priority_queue <DistTo, std::vector <DistTo>, std::greater<DistTo>> H;
	 H.push({ s,0 });
	 while (!H.empty() && H.top().Dist_ < max_dist ){
		 auto u = H.top();
		 H.pop();
		 if (u.vertex_ == vert) continue;
		 DistTo current{ u.vertex_,adj[u.vertex_].d };
		 if (u < current) continue;
		 for (auto v : adj[u.vertex_].outgoing){
			 if (v.first == vert) continue;
			 if (adj[v.first].d > adj[u.vertex_].d + v.second){
				 changed.push_back(v.first);
				 adj[v.first].d = adj[u.vertex_].d + v.second;
				 adj[v.first].prev = u.vertex_;
				 adj[v.first].hop = adj[adj[v.first].prev].hop + 1;
				 if (currenthops < adj[v.first].hop) currenthops = adj[v.first].hop;
				 if (adj[v.first].hop<=max_hop)
				 H.push({ v.first,adj[v.first].d });
			 }
		 }
	 }

	 for (auto item : ws){
		 auto pred = adj[vert].incoming.at(s);
		 auto further = adj[vert].outgoing.at(item.first);
		 if (adj[item.first].d < pred+ further )res.insert(item.first);
	 }
	 Vipe(changed);	
	 return res;
}

void Graph::ContractNode(int v){
	for (auto& e : FindShorcuts(v))	{
		AddEdge(e);
	}
	
	for (auto& item : adj[v].outgoing){
		adj[item.first].level = std::max(adj[v].level + 1, adj[item.first].level);
	}
	for (auto& item : adj[v].incoming){
		adj[item.first].level = std::max(adj[v].level + 1, adj[item.first].level);
	}
	for (auto &item : adj[v].incoming){
		adj[item.first].outgoing.erase(v);
		adj[item.first].ContractedNeigbours++;
	}
	for (auto &item : adj[v].outgoing){
		adj[item.first].incoming.erase(v);
		adj[item.first].ContractedNeigbours++;
	}
	adj[v].outgoing.clear();
	adj[v].incoming.clear();
	rank++;
	adj[v].rank = rank;
	AugmentedGraph[v].rank = rank;	
}

long long Graph::CalcMaxIncomeOutDist(int v){
	long long max_len = 0;
	long long current_max = 0;
	for (auto item : adj[v].incoming){	
		if (item.second > current_max) current_max = item.second;
	}
	max_len += current_max;
	current_max = 0;
	for (auto item : adj[v].outgoing){		
		if (item.second > current_max) current_max = item.second;
	}
	max_len += current_max;
	return max_len;
}


void Graph::AddEdge(const Edge & e){
	AugmentedGraph[e.from_].outgoing[e.to_] = e.weight_;
	AugmentedGraph[e.to_].incoming[e.from_] = e.weight_;
	adj[e.from_].outgoing[e.to_] = e.weight_;
	adj[e.to_].incoming[e.from_] = e.weight_;
}



long long Graph::ComputeDistance(int s, int t){
	std::vector <int> changedf;
	std::vector <int> changedb;
	changedb.reserve(1000);
	changedf.reserve(1000);
	forward[s].d = 0;
	backward[t].d = 0;
	long long estimate = Infinity;

	std::priority_queue <DistTo, std::vector <DistTo>, std::greater<DistTo>> H;
	std::priority_queue <DistTo, std::vector <DistTo>, std::greater<DistTo>> HR;
	std::priority_queue <DistTo, std::vector <DistTo>, std::greater<DistTo>> E; // empty ques;
	std::priority_queue <DistTo, std::vector <DistTo>, std::greater<DistTo>> ER;
	H.push({ s,0 });
	HR.push({ t,0 });
	changedf.push_back(s);
	changedb.push_back(t);
	std::unordered_set <int> proc{ s };
	std::unordered_set <int> procr{ t };
	while (!H.empty() || !HR.empty())	{
		if (!H.empty())
		{
			if (H.top().Dist_ > estimate) H.swap(E);
		}		
		if (!H.empty())
		{
			auto u = H.top();
			H.pop();
			DistTo current{ u.vertex_,forward[u.vertex_].d };
			if (u < current) continue;
			proc.insert(u.vertex_);
			for (auto &v : forward[u.vertex_].AdjList) {			
				if (forward[v.first].d > forward[u.vertex_].d + v.second) {				
					forward[v.first].d = forward[u.vertex_].d + v.second;
					changedf.push_back(v.first);
					if (forward[v.first].d<estimate)
					H.push({ v.first,forward[v.first].d });		
				}
			}
			if (procr.count(u.vertex_))	{		
				if (forward[u.vertex_].d + backward[u.vertex_].d < estimate) estimate = forward[u.vertex_].d + backward[u.vertex_].d;
			}
		}
		if (!HR.empty()){
			if (HR.top().Dist_ > estimate) HR.swap(ER);
		}
		// backward
		if (!HR.empty()){
			auto u = HR.top();
			HR.pop();
			DistTo current{ u.vertex_,backward[u.vertex_].d };
			if (u < current) continue;
			procr.insert(u.vertex_);
			for (auto &v : backward[u.vertex_].AdjList){
				if (backward[v.first].d > backward[u.vertex_].d + v.second){
					backward[v.first].d = backward[u.vertex_].d + v.second;
					changedb.push_back(v.first);
					if (backward[v.first].d < estimate)
					HR.push({ v.first,backward[v.first].d });
			
				}
			}
			if (proc.count(u.vertex_)){
				if (forward[u.vertex_].d + backward[u.vertex_].d <= estimate) estimate = forward[u.vertex_].d + backward[u.vertex_].d;

			}
		}
	}
	VipeForward(changedf);
	VipeBackward(changedb);
	if (estimate == Infinity) return -1;
	return estimate;
}



