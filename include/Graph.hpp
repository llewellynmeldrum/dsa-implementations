#include <unordered_map>
#include <unordered_set>
#include "LMUtils.hpp"

template <typename T>
struct DirectedGraph {

	using index_t = size_t;
	using child_t = std::unordered_set<size_t>;
	std::unordered_map<index_t, child_t> adjacencyList;

	// *INDENT-OFF*
	void addAdjacency(index_t parent_index, index_t child_index) {
		if ( !adjacencyList.contains(parent_index) ) {
			adjacencyList[parent_index] = {};
			adjacencyList[parent_index].insert(child_index);
		} else  {
			auto& children = adjacencyList[parent_index];
			children.insert(child_index);
		}
	}
	void printAdjList(){
		for (const auto& pair : adjacencyList) {
			std::cout << "[" << pair.first << "]:\n";
			for (const auto& child : pair.second){
				std::cout << "    " << child << "\n";
			}
		}
	}

	// goal: parse following form into adjacency list
	// [[2,4],[1,3],[2,4],[1,3]]
	void logError(std::string s, std::string err) {
		fprintf(stderr, "Failed to parse input to Graph ctor (%s): %s Initializing as empty.\n", s.c_str(), err.c_str());
	}

	DirectedGraph() {}
	DirectedGraph(std::string s) {
		if (s.size() <= 7) {
			// smallest possible is [[n,m]]
			DirectedGraph();
		}
		std::vector<std::pair<int, int>> pairs{};
		if (s[0] != '[' || s[s.size() - 1] != ']') {
			logError(s, "Missing opening/closing square braces.\n");
			DirectedGraph();
		}

		int left_edge = 1;
		std::cout << "Parsing begun for '" << s << "'\n"; 
		for (int iter = 1;iter<=9; iter++) {
			std::cout << "[" << iter << "] ";
			int right_edge = s.find(']', left_edge);
			if (right_edge==std::string::npos) break;
			auto string_pair = LM::string::between(s, left_edge+1, right_edge);

			std::cout << "| Pair: {'" << string_pair << "'}";
			std::vector<std::string>& tokens = LM::string::split(string_pair, ',');
			if (tokens.size() != 2) {
				logError(s, "Adjacency pair includes !=2 elements:\n");
				DirectedGraph();
			}
			std::cout << "| Tokens:";
			std::cout << '{';
			for (std::string& a: tokens){
				std::cout << "'"<< a <<"', ";
			}
			std::cout << "}\n";
			auto parent_idx = atol(tokens[0].c_str());
			auto child_idx = atol(tokens[1].c_str());
			addAdjacency(parent_idx, child_idx);
			left_edge = right_edge+2;
		}

	}
	~DirectedGraph(){};
};
