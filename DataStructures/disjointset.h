#include <vector>

namespace DJS{
	struct  vertex{
		size_t parent;
		size_t rank = 0;
	};
	class DJS{
	public:
		DJS(size_t size_){
			v_.resize(size_);
			for (size_t i = 0; i < size_; i++){
				v_[i].parent = i;
				v_[i].rank = 0;
			}
		};
		size_t Find(size_t i){
			while ( i!=v_[i].parent){
				i = v_[i].parent;
			}
			return i;
		}
		void Union(size_t i, size_t j){
			auto i_id = Find(i);
			auto j_id = Find(j);
			if (i_id == j_id) return;
			if (v_[i_id].rank > v_[j_id].rank)
			{
				v_[j_id].parent = i_id;
			}
			else
			{
				v_[i_id].parent = j_id;
				if (v_[i_id].rank == v_[j_id].rank)
				{
					v_[j_id].rank++;
				}
			}
		}
		~DJS()= default;
	private :
		std::vector <vertex> v_;
	};


}

