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

static bool auto_verbose = false;

int main(int argc, char** argv) {
	if (argc == 2) {
		if (strcmp(argv[1], "-v") == 0) {
			printf("%sAuto verbose mode enabled.%s\n", ansi::fg_green.data, ansi::reset.data);
			auto_verbose = true;
		} else {
			printf("%sError! Unrecognized input arguments. Usage:%s %s [-v]\n", ansi::fg_red.data, ansi::reset.data, argv[0]);
			exit(EXIT_FAILURE);
		}
	} else if (argc > 2) {
		printf("%sError! Too many input arguments (%d). Usage:%s %s [-v]\n", ansi::fg_red.data, argc, ansi::reset.data, argv[0]);
		exit(EXIT_FAILURE);
	} else if (argc < 1) {
		printf("%sError! Too few input arguments (%d). Usage:%s %s [-v]\n", ansi::fg_red.data, argc, ansi::reset.data, argv[0]);
		exit(EXIT_FAILURE);
	}

	test_LinkedList();
	test_BinaryTree();
	test_Graphs();
	print_summary_info();
}


void test_LinkedList() {
	LinkedList<int> list;
	{
		TestGroup<std::string> test("LINKED LIST: insert/delete", {.verbose = auto_verbose});
		list.push_back(4)->push_back(5)->push_back(6)->push_back(7);
		test.run("push_back()", "[4,5,6,7]", list.to_str());

		list.push_front(3)->push_front(2)->push_front(1);
		test.run("push_front()", "[1,2,3,4,5,6,7]", list.to_str());

		list.remove_tail();
		test.run("remove_tail()", "[1,2,3,4,5,6]", list.to_str());

		list.remove_head();
		test.run("remove_head()", "[2,3,4,5,6]", list.to_str());

		list.clear();
		test.run("clear()", "[]", list.to_str());
	}
	{
		TestGroup<bool> test("LINKED LIST: utility", {.verbose = auto_verbose});

		list = {};
		test.run(".empty() 1", true, list.empty());
		test.run(".contains() 1", false, list.contains(1));
		test.run(".size() 1", 0, list.size());

		list.push_back(1);
		test.run(".empty() 2", false, list.empty());
		test.run(".contains() 2", true, list.contains(1));
		test.run(".size() 2", 1, list.size());

		list.remove_head();
		list.push_back(1)->push_back(2)->push_back(3);
		test.run(".size() 2", 3, list.size());
		test.run(".has_cycle() 1", false, list.has_cycle());

		list.push_back(list.head);
		test.run(".has_cycle() 2", true, list.has_cycle());
	}
}

void test_BinaryTree() {
	// Test cases from LC144, 94 and 145
	BinaryTree<int> tree1("[1,null,2,3]");
	BinaryTree<int> tree2("[1,2,3,4,5,null,8,null,null,6,7,9]");
	{
		TestGroup<std::string> test("BINARY TREE: Leetcode style CTOR (1)", {.verbose = auto_verbose});
		test.run("BFS equality:", 			"[1,null,2,3]", tree1.BFS_ToString());
		test.run("DFS (preorder) equality:", 	"[1,2,3]", 	tree1.DFS_PreOrderToString());
		test.run("DFS (inorder) equality:", 	"[1,3,2]", 	tree1.DFS_InOrderToString());
		test.run("DFS (postorder) equality:", 	"[3,2,1]", 	tree1.DFS_PostOrderToString());
	}
	{
		TestGroup<std::string> test("BINARY TREE: Leetcode style CTOR (2)");
		test.run("BFS equality:", "[1,2,3,4,5,null,8,null,null,6,7,9]", tree2.BFS_ToString());
		test.run("DFS (preorder) equality:", 	"[1,2,4,5,6,7,3,8,9]", 	tree2.DFS_PreOrderToString());
		test.run("DFS (inorder) equality:", 	"[4,2,6,5,7,1,3,9,8]", 	tree2.DFS_InOrderToString());
		test.run("DFS (postorder) equality:", 	"[4,6,7,5,2,9,8,3,1]", 	tree2.DFS_PostOrderToString());
	}
}

void test_Graphs() {
	// test cases manually verified on https://graphonline.top/en/
	DirectedGraph<int> g0("g0", "[]");
	DirectedGraph<int> g1("g1", "[[1,2],[1,3],[1,4],[3,1]]");
	DirectedGraph<int> g2("g2", "[[1,2],[1,3],[2,4],[2,5],[3,6],[5,6]]");
	DirectedGraph<int> g3("g3", "[[1,2],[1,3],[2,4],[3,4],[4,1],[4,5],[5,3]]");
	DirectedGraph<int> g4("g4",
	                      "[[1,2],[1,3],[1,6],[1,7],[1,8],"
	                      "[2,4],"
	                      "[3,4],[3,11],[3,12],"
	                      "[4,1],[4,5],"
	                      "[5,3],"
	                      "[7,2],"
	                      "[8,3],[8,6],"
	                      "[10,4],"
	                      "[11,2]]");
	{
		TestGroup<std::string> test("DIRECTED GRAPH: Adjacency list equality", {.verbose = auto_verbose});
		test.run("g0", "", g0.adjacencyListToStr());
		test.run("g1", "1->[2,3,4], 2->[], 3->[1], 4->[]", g1.adjacencyListToStr());
		test.run("g2", "1->[2,3], 2->[4,5], 3->[6], 4->[], 5->[6], 6->[]", g2.adjacencyListToStr());
		test.run("g3", "1->[2,3], 2->[4], 3->[4], 4->[1,5], 5->[3]", g3.adjacencyListToStr());
		test.run("g4",
		         "1->[2,3,6,7,8], "
		         "2->[4], "
		         "3->[4,11,12], "
		         "4->[1,5], "
		         "5->[3], "
		         "6->[], "
		         "7->[2], "
		         "8->[3,6], "
		         "9->[], "
		         "10->[4], "
		         "11->[2], "
		         "12->[]", g4.adjacencyListToStr());
	}
	{
		TestGroup<std::string> test("DIRECTED GRAPH: Breadth first traversal (level order)", {.verbose = auto_verbose});
		test.run("g0", "[]", g0.BFStoString());
		test.run("g1", "[1,2,3,4]", g1.BFStoString());
		test.run("g2", "[1,2,3,4,5,6]", g2.BFStoString());
		test.run("g3", "[1,2,3,4,5]", g3.BFStoString());
		test.run("g4", "[1,2,3,6,7,8,4,11,12,5]", g4.BFStoString());

	}
	{
		TestGroup<std::string> test("DIRECTED GRAPH: Depth first traversal", {.verbose = auto_verbose});
		test.run("g0", "[]", g0.DFStoString());
		test.run("g1", "[1,2,3,4]", g1.DFStoString());
		test.run("g2", "[1,2,4,5,6,3]", g2.DFStoString());
		test.run("g3", "[1,2,4,5,3]", g3.DFStoString());
		test.run("g4", "[1,2,4,5,3,11,12,6,7,8]", g4.DFStoString());

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

