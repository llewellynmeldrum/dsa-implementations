#include "ansi_colors.hpp"
#include <cassert>
#include <iostream>
#include <ostream>
#include <queue>
#include <sstream>
#include <string>
#include <strstream>
#include <cmath>
#include <source_location>
#include <fstream>
#include <variant>
#include <vector>
#include <unordered_set>

#include "TestGroup.hpp"
#include "LMUtils.hpp"
#include <fmt/color.h>

#include "LinkedList.hpp"
#include "BinaryTree.hpp"

size_t G::TOTAL_FAIL_COUNT = 0;
size_t G::TOTAL_PASS_COUNT = 0;
size_t G::FAILING_GROUPS = 0;
size_t G::SUCCESSFUL_GROUPS = 0;

void print_summary_info();
void test_LinkedList();
void test_BinaryTree();

int main() {
	test_LinkedList();
	test_BinaryTree();
	print_summary_info();
}


void test_BinaryTree() {
TEST_BT_LEETCODE_STYLE_CTOR: {
		// cases from LC144, 94 and 145
		// (https://leetcode.com/problems/binary-tree-preorder-traversal/description/)
		// (https://leetcode.com/problems/binary-tree-inorder-traversal/description/)
		// (https://leetcode.com/problems/binary-tree-postorder-traversal/description/)

		const std::string s1 = "[1,null,2,3]";
		const std::string s2 = "[1,2,3,4,5,null,8,null,null,6,7,9]";
		auto tree = BinaryTree<int>(s1);
		auto tree2 = BinaryTree<int>(s2);
		{
			TestGroup<std::string> test("BINARY TREE: Leetcode style CTOR (1)", true);
			test.run_test("BFS equality:", 			"[1,null,2,3]", tree.BFS_ToString());
			test.run_test("DFS (preorder) equality:", 	"[1,2,3]", 	tree.DFS_PreOrderToString());
			test.run_test("DFS (inorder) equality:", 	"[1,3,2]", 	tree.DFS_InOrderToString());
			test.run_test("DFS (postorder) equality:", 	"[3,2,1]", 	tree.DFS_PostOrderToString());
		}

		{
			TestGroup<std::string> test2("BINARY TREE: Leetcode style CTOR (2)", true);
			test2.run_test("BFS equality:", 		"[1,2,3,4,5,null,8,null,null,6,7,9]", tree2.BFS_ToString());
			test2.run_test("DFS (preorder) equality:", 	"[1,2,4,5,6,7,3,8,9]", 	tree2.DFS_PreOrderToString());
			test2.run_test("DFS (inorder) equality:", 	"[4,2,6,5,7,1,3,9,8]", 	tree2.DFS_InOrderToString());
			test2.run_test("DFS (postorder) equality:", 	"[4,6,7,5,2,9,8,3,1]", 	tree2.DFS_PostOrderToString());
		}



	}
}




void test_LinkedList() {
	LinkedList<int> list;

TEST_LL_ADD_REMOVE: {
		TestGroup<std::string> test("LINKED LIST: insert/delete", true);
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
		TestGroup<bool> utility("LINKED LIST: utility", false);

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

using fmt::print;
using fmt::color;
using fmt::emphasis;
using fmt::text_style;

#define printst(format, ...) fmt::print(style, format, ##__VA_ARGS__)

void print_summary_info() {
	text_style style;
	size_t n_test_groups = G::SUCCESSFUL_GROUPS + G::FAILING_GROUPS;
	size_t n_tests = G::TOTAL_PASS_COUNT + G::TOTAL_FAIL_COUNT;

	fmt::color head_col = G::FAILING_GROUPS ? color::red : color::green;
	style = fg(head_col) | emphasis::bold | emphasis::reverse;
	printst("TESTS SUMMARY:\n");


	style = {};
	if (G::FAILING_GROUPS) style = fg(color::red);
	print("     test groups: ");
	printst("{}/{}",
	        G::SUCCESSFUL_GROUPS, n_test_groups);
	print(" completed without failure.\n");

	style = {};
	if (G::TOTAL_FAIL_COUNT)  style = fg(color::red);
	print("individual tests: ");
	printst("{}/{}",
	        G::TOTAL_PASS_COUNT, n_tests);
	print(" completed without failure.\n");

}
