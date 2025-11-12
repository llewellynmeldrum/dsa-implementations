
#include <functional>
#include <iostream>
#include <queue>
#include "LMUtils.hpp"
template<typename T>
struct BinaryTree {
	template<typename U = T>
	struct Node {
		U val{};
		Node<U> *left = nullptr;
		Node<U> *right = nullptr;

		Node(U v, Node<U> *l, Node<U> *r) : val(v), left(l), right(r) {};
		Node(U v) : val(v), left(nullptr), right(nullptr) {};
		~Node() { }

		inline bool isLeaf() {
			std::cout << 'g';
			return !(left && right);
		}
	};
	Node<T> *root = nullptr;
	std::vector<T *> traversalBuffer{};




	template <typename V>
	void printVector (const std::vector<V> &vec) {
		int x = 0;
		std::cout << "{";
		for (auto e : vec) {
			std::cout <<  (x++ == 0  ? "" : ", ");
			std::cout << "\"" << e << "\"";
		}
		std::cout << "}\n";
	}
	BinaryTree() { };

	~BinaryTree() {
		std::function<void(Node<T>*)>
		postorderDeletion = [&](Node<T> *node) {
			if (!node) return;
			postorderDeletion(node->left);
			postorderDeletion(node->right);
			delete node;
		};
		postorderDeletion(root);
	}

	BinaryTree(std::string s) {
		auto logFailure = [](const std::string& msg = "") {
			std::cerr << "Failed to construct binary tree"
			             "from breadth first comma separated string: ";
			std::cerr << msg << std::endl;
		};

		if (s.size() < 2) {
			logFailure("String " + s + " is too small.");
			BinaryTree();
		}
		if (s.size() == 2) BinaryTree();


		size_t first = 0;
		size_t last = s.size() - 1;
		if (!(	s[first] == '[' &&
		        s[last] == ']')) {
			logFailure("String " + s + " is missing closing or opening square braces.");
			BinaryTree();
		}

		std::cout << s << std::endl;
		s = LM::string::trim(s, " \r\n\t");
		std::cout << s << std::endl;
		auto tokens = LM::string::split(s, ',');

		auto remainingStringsAreNull = [](const std::vector<std::string> &arr, size_t start) {
			for (int i = start; i < arr.size(); i++) {
				if (arr[i] != "null") {
					return false;
				}
			}
			return true;
		};

		auto parse = [this](const std::string & valueString) mutable -> Node<T> *  {
			if (valueString == "null") return nullptr;

			std::istringstream ss(valueString);
			T value;
			ss >> value;
			// assign val with the implicit cast from istringstream  (i think this only works for numeric)
			Node<T> *node = newNode(value);
			return node;
		};


		auto leftChildIndex = [](int i) {
			return (i * 2) + 1;
		};
		auto rightChildIndex = [](int i) {
			return (i * 2) + 2;
		};


		std::queue<Node<T>*> q;
		root = parse(tokens[0]);
		q.push(root);
		size_t i = 1;
		while (!q.empty() && i < tokens.size()) {
			auto* node = q.front();
			q.pop();
			if (tokens[i] != "null") {
				node->left = parse(tokens[i]);
				q.push(node->left);
			}
			i++;
			if (i < tokens.size() && tokens[i] != "null") {
				node->right = parse(tokens[i]);
				q.push(node->right);
			}
			i++;
		}


	};

	/* -------- ADD/REMOVAL -------- */

	inline Node<T> *newNode(T val) {
		return new Node<T>(val);
	}


	void inorderTraverse(Node<T> *root) {
		if (!root) return;
		inorderTraverse(root->left);
		traversalBuffer.push_back(&root->val);
		inorderTraverse(root->right);
	}

	void preorderTraverse(Node<T> *root) {
		if (!root) return;
		traversalBuffer.push_back(&root->val);
		preorderTraverse(root->left);
		preorderTraverse(root->right);
	}
	void postorderTraverse(Node<T> *root) {
		if (!root) return;
		postorderTraverse(root->left);
		postorderTraverse(root->right);
		traversalBuffer.push_back(&root->val);
	}


	std::string DFS_InOrderToString() {
		if (!root) return "[]";
		traversalBuffer.clear();
		std::ostringstream oss;

		inorderTraverse(root);
		for (int i = 0; i < traversalBuffer.size(); i++) {
			if (i != 0) oss << ",";
			oss << *traversalBuffer[i];

		}
		return "[" + oss.str() + "]";
	}

	std::string DFS_PreOrderToString() {
		if (!root) return "[]";
		traversalBuffer.clear();
		std::ostringstream oss;

		preorderTraverse(root);
		for (int i = 0; i < traversalBuffer.size(); i++) {
			if (i != 0) oss << ",";
			oss << *traversalBuffer[i];

		}
		return "[" + oss.str() + "]";
	}

	std::string DFS_PostOrderToString() {
		if (!root) return "[]";
		traversalBuffer.clear();
		std::ostringstream oss;

		postorderTraverse(root);
		for (int i = 0; i < traversalBuffer.size(); i++) {
			if (i != 0) oss << ",";
			oss << *traversalBuffer[i];

		}
		return "[" + oss.str() + "]";
	}

	std::string BFS_ToString() {
		if (!root) return "[]";

		std::vector<std::string> out;
		std::queue<Node<T>*> q;
		q.push(root);

		size_t lastNonNull = 0;

		auto to_str = [](T val) {
			std::ostringstream os;
			os << val;
			return os.str();
		};

		while (!q.empty()) {
			Node<T> *n = q.front();
			q.pop();
			if (n) {
				out.push_back(to_str(n->val));
				lastNonNull = out.size() - 1;
				q.push(n->left);
				q.push(n->right);
			} else {
				out.push_back("null");
			}
		}

		out.resize(lastNonNull + 1);

		std::ostringstream oss;
		for (size_t i = 0; i < out.size(); ++i) {
			if (i) oss << ",";
			oss << out[i];
		}
		return "[" + oss.str() + "]";
	}

};
