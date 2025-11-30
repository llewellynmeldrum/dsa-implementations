#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include "LMUtils.hpp"

template <typename T>
struct DirectedGraph {
	struct Node {
		Node *child;
		Node(Node* child = nullptr): child(child) {};
	};


	using index_t = size_t;
	using child_set_t = std::unordered_set<size_t>;
	std::unordered_map<index_t, child_set_t> adjacencyList;

	void addAdjacency(index_t parent_index, index_t child_index) {
		if ( !adjacencyList.contains(parent_index) ) {
			adjacencyList[parent_index] = {};
			adjacencyList[parent_index].insert(child_index);
		} else  {
			auto& children = adjacencyList[parent_index];
			children.insert(child_index);
		}
	}
	inline std::string adjacencyListToStr();

	// goal: parse following form into adjacency list
	// [[2,4],[1,3],[2,4],[1,3]]
	void logError(std::string s, std::string err);

	DirectedGraph() {}
	DirectedGraph(std::string s);
	~DirectedGraph() {};
};


// Constructs a directed graph from a leetcode style edge list.
// - i.e `"[[[1,2]],[[2,3]],[[3,1]]]"`:
// - ╭──▶① ────▶② ──╮
// - ╰──────③ ◀─────╯
// - Any whitespace is ignored.
template <typename T>
DirectedGraph<T>::DirectedGraph(std::string s) {
	bool error;
	if (s.size() <= 7) {
		// smallest possible is [[a,b]] (7 chars)
		DirectedGraph();
	}
	std::vector<std::pair<int, int>> pairs{};
	if (s[0] != '[' || s[s.size() - 1] != ']') {
		logError(s, "Missing opening/closing square braces.\n");
		DirectedGraph();
	}

	int left_edge = 1;
//		std::cout << "Parsing begun for '" << s << "'\n";
	for (int iter = 1; iter <= 100; iter++) {
//			std::cout << "[" << iter << "] ";
		int right_edge = s.find(']', left_edge);
		if (right_edge == std::string::npos) break;
		auto string_pair = LM::string::between(s, left_edge + 1, right_edge);

//			std::cout << "| Pair: {'" << string_pair << "'}";
		std::vector<std::string> &tokens = LM::string::split(string_pair, ',');
		if (tokens.size() != 2) {
			if (tokens.size() == 0) {
				DirectedGraph();
				break;
			}
			logError(s, "Adjacency pair includes !=2 elements:\n");
			DirectedGraph();
			break;
		}
//			std::cout << "| Tokens:";
//			std::cout << '{';
		for (std::string& a : tokens) {
//				std::cout << "'"<< a <<"', ";
		}
//			std::cout << "}\n";
		size_t parent_idx = atol(tokens[0].c_str());
		size_t child_idx = atol(tokens[1].c_str());
		// add an empty one for any mentioned children
		if (!adjacencyList.contains(child_idx)) {
			adjacencyList.insert({child_idx, {}});
		}
		addAdjacency(parent_idx, child_idx);
		left_edge = right_edge + 2;
	}

}
template <typename T>
void DirectedGraph<T>::logError(std::string s, std::string err) {
	fprintf(stderr, "Failed to parse input to Graph ctor (%s): %s Initializing as empty.\n", s.c_str(), err.c_str());
}
template <typename T>
inline std::string DirectedGraph<T>::adjacencyListToStr() {
	std::vector<std::string> orderedAdjacencyList({});

	auto addAndResize = [](std::vector<std::string> &list, const std::string & ins, size_t idx) {
		while (idx >= list.size()) {
			list.resize(idx + 1);
		}
		list[idx] = ins;
	};

	for (const auto&[parent_idx, children_indexes] : adjacencyList) {
		std::vector<size_t> children{};
		for (const auto& child_idx : children_indexes) {
			children.push_back(child_idx);
		}

		// children are sorted ascending
		std::sort(children.begin(), children.end());
		std::ostringstream oss;
		for (int i = 0; i < children.size(); i++) {
			oss << children[i];
			if (i != children.size() - 1) {
				oss << ",";
			}
		}
		// parents edge lists are inserted in ascending order
		addAndResize(orderedAdjacencyList, oss.str(), parent_idx);
	}

	std::ostringstream oss;
	for (int i = 1; i < orderedAdjacencyList.size(); i++) {
		const auto& s = orderedAdjacencyList[i];
		oss << i << "->[";
		oss << s << "]";
		if (i != orderedAdjacencyList.size() - 1) {
			oss << ", ";
		}
	}
	return oss.str();
}

