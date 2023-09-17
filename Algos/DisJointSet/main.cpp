#include <vector>
#include <algorithm>


namespace djs {
	struct  Vertex {
		size_t parent =0u;
		size_t rank = 0u;
	};
	class DJS final {
	public:
		explicit DJS(size_t size) {
			v_.resize(size);
			for (size_t i = 0u; i < size; ++i) {
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
			if (i_id == j_id) { return;
}
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
		std::vector <Vertex> v_;
	};
}

// DJS used in Kruskal algorithm
namespace kruskal {
	using Vertex = std::vector <std::pair<int, double>>;
		
	struct  Edge{
		int from = 0;
		int to = 0;
		double w;

	};
	std::vector < Edge > KruskalAlgorithm(const std::vector <Vertex>& adj){
		djs::DJS s(adj.size());
		std::vector <Edge > res;
		std::vector <Edge> lightestribs;
		for (size_t i = 0; i < adj.size(); ++i) {
			for (auto vw : adj[i]) {
				lightestribs.push_back({ static_cast<int>(i),vw.first,vw.second });
			}
		}
		std::sort(lightestribs.begin(), lightestribs.end(), 
			[](auto lhs, auto rhs) { return lhs.w < rhs.w; });
		for (auto &item : lightestribs){
			if (s.Find(item.from) != s.Find(item.to)){
				res.push_back(item);
				s.Union(item.from, item.to);
			}
		}
		return res;
	}
}


int main() {


	return 0;
}


