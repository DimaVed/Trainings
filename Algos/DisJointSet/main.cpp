#include <vector>
#include <algorithm>


namespace DJS {
	struct  vertex {
		size_t parent =0u;
		size_t rank = 0u;
	};
	class DJS final {
	public:
		DJS(size_t size_) {
			v_.resize(size_);
			for (size_t i = 0u; i < size_; ++i) {
				v_[i].parent = i;
				v_[i].rank = 0u;
			}
		};
		size_t Find(size_t i) { // path compression applied
			while (i != v_[i].parent) {
				i = v_[i].parent;
			}
			return i;
		}
		void Union(size_t i, size_t j) {// rank heristic applied
			auto i_id = Find(i);
			auto j_id = Find(j);
			if (i_id == j_id) return;
			if (v_[i_id].rank > v_[j_id].rank) {
				v_[j_id].parent = i_id;
			}
			else {
				v_[i_id].parent = j_id;
				if (v_[i_id].rank == v_[j_id].rank) {
					v_[j_id].rank++;
				}
			}
		}
		~DJS() = default;
	private:
		std::vector <vertex> v_;
	};
}

// DJS used in Kruskal algorithm
namespace Kruskal {
	using Vertex = std::vector <std::pair<int, double>>;
		
	struct  Edge{
		int from = 0;
		int to = 0;
		double w;

	};
	std::vector < Edge > KruskalAlgorithm(const std::vector <Vertex>& adj){
		DJS::DJS S(adj.size());
		std::vector <Edge > res;
		std::vector <Edge> lightestribs;
		for (int i = 0; i < adj.size(); ++i) {
			for (auto vw : adj[i]) {
				lightestribs.push_back({ i,vw.first,vw.second });
			}
		}
		std::sort(lightestribs.begin(), lightestribs.end(), 
			[](auto lhs, auto rhs) { return lhs.w < rhs.w; });
		for (auto &item : lightestribs){
			if (S.Find(item.from) != S.Find(item.to)){
				res.push_back(item);
				S.Union(item.from, item.to);
			}
		}
		return res;
	}
}


int main() {


	return 0;
}


