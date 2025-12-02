#include <iostream>
#include <queue>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <print>
#include <algorithm>
#include "LMUtils.hpp"

template <typename T>
struct DirectedGraph {

	struct Node {
		T val = -1;
		double weight = 1.0;
	};
	Node *root = nullptr;
	std::vector<Node> nodes{};

	using NodeID = T;
	using child_set_t = std::unordered_set<size_t>;
	std::unordered_map<NodeID, child_set_t> adjacencyList;


	void addAdjacency(NodeID parent_index, NodeID child_index) {
		if ( !adjacencyList.contains(parent_index) ) {
			adjacencyList[parent_index] = {};
			adjacencyList[parent_index].insert(child_index);
			if (!root) {
				root = new Node{parent_index};
				nodes.push_back(*root);
			} else {
				nodes.push_back(Node{parent_index});
			}

		} else  {
			auto& children = adjacencyList[parent_index];
			children.insert(child_index);
		}
	}

	std::string BFStoString();
	std::string DFStoString();

	DirectedGraph() {}
	DirectedGraph(std::string name, std::string s);
	DirectedGraph(std::string s): DirectedGraph("unnamed", s) {};

	~DirectedGraph() {
		delete root;
	};


	// utility/testing
	void logNotice(std::string s, std::string err);
	inline std::string adjacencyListToStr();
  private:
	static constexpr bool noticesEnabled = true;

	enum NodeState {
		UNDISCOVERED,
		DISCOVERED,
		PROCESSED,
	};
	std::string *traversal_buffer;
	std::string debug_name;
	std::string DFS_rec(int parent_id, LM::hash_map<int, NodeState> &state_of);
};


template<typename T>
std::string DirectedGraph<T>::DFStoString() {
	if (!root) return "[]";

	LM::hash_map<int, NodeState> state_of{};
	for (auto&[node_id, _] : adjacencyList) {
		state_of.insert({node_id, UNDISCOVERED});
	}


	if (adjacencyList.contains(root->val) == false) {
		LM::log("Error! root is invalid. id=(", root->val, "), does not exist in adjacencyList.");
	}


	traversal_buffer = new std::string{};
	DFS_rec(root->val, state_of);
	traversal_buffer->erase(traversal_buffer->size() - 1, 1);

	auto res = "[" + *traversal_buffer + "]";
	delete traversal_buffer;

	return res;
}

template<typename T>
std::string DirectedGraph<T>::DFS_rec(int parent_id, LM::hash_map<int, NodeState> &state_of) {
	state_of[parent_id] = DISCOVERED;

	if (state_of[parent_id] != PROCESSED) {
		traversal_buffer->append(std::format("{},", parent_id));
	} else {
		return "";
	}

	LM::min_heap<NodeID> minHeap{};
	for (NodeID child_id : adjacencyList.at(parent_id)) {
		// test expects ascending order starting from root nodes
		minHeap.push(child_id);
	}

	while (!minHeap.empty()) {
		int child_id = LM::pop_top(minHeap);
		if (state_of[child_id] == UNDISCOVERED) {
			state_of[child_id] = DISCOVERED;
			DFS_rec(child_id, state_of);
		}
	}

	state_of[parent_id] = PROCESSED;
	return "";
}

template<typename T>
std::string DirectedGraph<T>::BFStoString() {
	if (!root) return "[]";

	std::ostringstream oss;
	std::queue<int> q{};

	LM::hash_map<int, NodeState> state_of{};

	for (auto&[node_id, _] : adjacencyList) {
		state_of.insert({node_id, UNDISCOVERED});
	}


	q.push(root->val);
	state_of[root->val] = DISCOVERED;
	if (adjacencyList.contains(root->val) == false) {
		LM::log("Error! root is invalid. id=(", root->val, "), does not exist in adjacencyList.");
	}
	while (q.empty() == false) {
		int N = q.size();
		LM::min_heap<int> minHeap{};
		for (int i = 0; i < N; i++) {
			auto parent_id = LM::pop_front(q);
			if (state_of[parent_id] != PROCESSED) {
				minHeap.push(parent_id);
			}

			for (NodeID child_id : adjacencyList.at(parent_id)) {
				if (state_of[child_id] == UNDISCOVERED) {
					state_of[child_id] = DISCOVERED;
					q.push(child_id);
				}

			}
			state_of[parent_id] = PROCESSED;
		}
		while (!minHeap.empty()) {
			// test cases expect ascending order per level.
			oss << LM::pop_top(minHeap) << ",";
		}
	}



	auto res = oss.str();
	res.erase(res.size() - 1, 1); // trim the final comma
	return "[" + res + "]";
}



// Constructs a directed graph from a leetcode style edge list.
// - i.e `"[[[1,2]],[[2,3]],[[3,1]]]"`:
// - ╭──▶① ────▶② ──╮
// - ╰──────③ ◀─────╯
// - Any whitespace is ignored.
// ^ no it doesnt you fucking liar
template <typename T>
DirectedGraph<T>::DirectedGraph(std::string name, std::string s) {
	this->debug_name = name;
	bool error;
	if (s.size() <= 7) {
		// smallest possible is [[a,b]] (7 chars)
		DirectedGraph();
	}
	std::vector<std::pair<int, int>> pairs{};
	if (s[0] != '[' || s[s.size() - 1] != ']') {
		logNotice(s, "Missing opening/closing square braces.\n");
		DirectedGraph();
	}

	bool FOUND_ROOT = false;
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
			logNotice(s, "Adjacency pair includes !=2 elements:\n");
			DirectedGraph();
			break;
		}
		for (std::string& a : tokens) {
//			std::cout << "'" << a << "', ";
		}
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
void DirectedGraph<T>::logNotice(std::string s, std::string err) {
	if (noticesEnabled)
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

