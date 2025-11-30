#include "ansi_colors.hpp"
#include "TestGroup.hpp"

#include "LinkedList.hpp"
#include "BinaryTree.hpp"
#include "Graph.hpp"
#include "LMUtils.hpp"


void print_summary_info();
void test_LinkedList();
void test_BinaryTree();
void test_Graphs();
void test_LM_UTILS();

int main() {
	test_LinkedList();
	test_BinaryTree();
	test_Graphs();
//	test_LM_UTILS();
	print_summary_info();
}



void test_Graphs() {
	CFG::instance().testgroup_verbose_default = true;
	DirectedGraph<int> basic("[[1,2],[1,3],[1,4],[3,1]]");
	DirectedGraph<int> empty("[]");
	DirectedGraph<int> one_node("[[1,null]]");
	{
		TestGroup<std::string> test("DIRECTED GRAPH: Leetcode style CTOR, test adjacency list equality (1)");
		test.run_test("Empty graph", "", empty.adjacencyListToStr());
		test.run_test("Lonely node", "1->[0]", one_node.adjacencyListToStr());
		test.run_test("Basic case", "1->[2,3,4], 2->[], 3->[1], 4->[]", basic.adjacencyListToStr());
	}
	CFG::instance().testgroup_verbose_default = false;
}

void test_BinaryTree() {
	// cases from LC144, 94 and 145
	// (https://leetcode.com/problems/binary-tree-preorder-traversal/description/)
	// (https://leetcode.com/problems/binary-tree-inorder-traversal/description/)
	// (https://leetcode.com/problems/binary-tree-postorder-traversal/description/)

	const std::string s1 = "[1,null,2,3]";
	const std::string s2 = "[1,2,3,4,5,null,8,null,null,6,7,9]";
	auto tree = BinaryTree<int>(s1);
	auto tree2 = BinaryTree<int>(s2);
	{
		TestGroup<std::string> test("BINARY TREE: Leetcode style CTOR (1)");
		test.run_test("BFS equality:", 			"[1,null,2,3]", tree.BFS_ToString());
		test.run_test("DFS (preorder) equality:", 	"[1,2,3]", 	tree.DFS_PreOrderToString());
		test.run_test("DFS (inorder) equality:", 	"[1,3,2]", 	tree.DFS_InOrderToString());
		test.run_test("DFS (postorder) equality:", 	"[3,2,1]", 	tree.DFS_PostOrderToString());
	}

	{
		TestGroup<std::string> test2("BINARY TREE: Leetcode style CTOR (2)");
		test2.run_test("BFS equality:", 		"[1,2,3,4,5,null,8,null,null,6,7,9]", tree2.BFS_ToString());
		test2.run_test("DFS (preorder) equality:", 	"[1,2,4,5,6,7,3,8,9]", 	tree2.DFS_PreOrderToString());
		test2.run_test("DFS (inorder) equality:", 	"[4,2,6,5,7,1,3,9,8]", 	tree2.DFS_InOrderToString());
		test2.run_test("DFS (postorder) equality:", 	"[4,6,7,5,2,9,8,3,1]", 	tree2.DFS_PostOrderToString());
	}



}




void test_LinkedList() {
	LinkedList<int> list;

TEST_LL_ADD_REMOVE: {
		TestGroup<std::string> test("LINKED LIST: insert/delete");
		list.push_back(4)->push_back(5)->push_back(6)->push_back(7);
		test.run_test("push_back()", "[4,5,6,7]", list.to_str());

		list.push_front(3)->push_front(2)->push_front(1);
		test.run_test("push_front()", "[1,2,3,4,5,6,7]", list.to_str());

		list.remove_tail();
		test.run_test("remove_tail()", "[1,2,3,4,5,6]", list.to_str());

		list.remove_head();
		test.run_test("remove_head()", "[2,3,4,5,6]", list.to_str());

		list.clear();
		test.run_test("clear()", "[]", list.to_str());
	}

TEST_LL_UTILITY: {
		TestGroup<bool> utility("LINKED LIST: utility", {.verbose = false});

		list = {};
		utility.run_test("empty() 1", true, list.empty());



		utility.run_test("contains() 1", false, list.contains(1));
		utility.run_test("size() 1", 0, list.size());

		list.push_back(1);
		utility.run_test("empty() 2", false, list.empty());
		utility.run_test("contains() 2", true, list.contains(1));
		utility.run_test("size() 2", 1, list.size());


		list.remove_head();
		list.push_back(1)->push_back(2)->push_back(3);
		utility.run_test("size() 2", 3, list.size());


		utility.run_test("has_cycle() 1", false, list.has_cycle());

		list.push_back(list.head); // make cyclic
		utility.run_test("has_cycle() 2", true, list.has_cycle());
	}
}
void print_summary_info() {
	size_t N_TEST_GROUPS = N_PASSED_GROUPS + N_FAILED_GROUPS;
	size_t N_TESTS = N_PASSED_TESTS + N_FAILED_TESTS;

	ansi::fmt title_color = ansi::fg_green;
	if (N_FAILED_GROUPS) title_color = ansi::fg_red;

	ansi::fmt body_color = ansi::reset;
	if (N_FAILED_GROUPS) body_color = ansi::fg_red;




	std::cout << title_color;
	std::cout << ansi::bold << ansi::inverse
	          << "TESTS SUMMARY:\n" << ansi::reset;
	std::cout << body_color << ansi::bold;
	printf("    test groups: %zu/%zu completed successfully.\n", N_PASSED_GROUPS,  N_TEST_GROUPS);
	printf("          tests: %zu/%zu completed successfully.\n", N_PASSED_TESTS,  N_TESTS);
	std::cout << ansi::reset;

}

void test_LM_UTILS() {
	std::cout << "\n";

	{
		TestGroup<std::string> string_split("LM::String functions");
		std::string str = "1,2,3";
		auto& vec = LM::string::split(str, ',');
		string_split.run_test("split[0]", "1", vec[0]);
		string_split.run_test("split[1]", "2", vec[1]);
		string_split.run_test("split[2]", "3", vec[2]);
		string_split.run_test("split[3]", "", "");
		TestGroup<int> string_split_len_test("LM::String Split Length Check");
		string_split_len_test.run_test("split len", 3, vec.size());
	}
}

