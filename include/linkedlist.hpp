
#include <unordered_set>
#include <string>
#include <iostream>
#include <sstream>
#include "ansi_colors.hpp"
template <typename T>
struct ListNode {
	T val;
	ListNode *next;
	ListNode(T val, ListNode* next = nullptr)
		: val(val), next(next) {};
};

template <typename T>
struct LinkedList {
	ListNode<T> *head = nullptr;
	ListNode<T> *tail = nullptr;


	~LinkedList () {
		if (has_cycle()) {
			clear_beware_cycle();
		}
		clear();
	}

	// insertion
	void push_after();
	void push_before();

	LinkedList<T> *push_front(T val) {
		auto new_head = new ListNode<T>(val);
		if (!head || !tail) {
			head = new_head;
			tail = new_head;
			return this;
		}

		new_head->next = head;
		head = new_head;
		return this;
	}

	LinkedList<T> *push_front(ListNode<T> *new_head) {
		if (!head || !tail) {
			head = new_head;
			tail = new_head;
			return this;
		}

		new_head->next = head;
		head = new_head;
		return this;
	}

	LinkedList<T> *push_back(ListNode<T> *new_tail) {
		if (!head || !tail) {
			head = new_tail;
			tail = new_tail;
			return this;
		}

		tail->next = new_tail;
		tail = new_tail;
		return this;
	}

	LinkedList<T> *push_back(T val) {
		auto new_tail = new ListNode<T>(val);
		if (!head || !tail) {
			head = new_tail;
			tail = new_tail;
			return this;
		}

		tail->next = new_tail;
		tail = new_tail;
		return this;
	}

	bool remove_head() {
		if (!head) return false;
		if (!head->next) {
			delete head;
			head = nullptr;
			return true;
		}
		auto new_head = head->next;
		delete head;
		head = new_head;
		return true;
	}

	bool remove_tail() {
		if (!tail || !head ) return false;
		auto cur = head;
		auto prev = cur;
		while (cur->next) {
			prev = cur;
			cur = cur->next;
		}
		prev->next = nullptr;
		delete tail;
		tail = prev;
		return true;
	}

	void clear() {
		while (head) remove_head();
	}

	void clear_beware_cycle() {
		if (!head) return;
		while (has_cycle()) {
			std::unordered_set<ListNode<T>*> seen;
			auto current = head;
			int i = 0;
			while (current) {
				if (current->next && seen.contains(current->next)) {
					current->next = nullptr;
					break;
				}
				seen.insert(current);
				current = current->next;
			}
		}

		clear();
	}
	// utility
	bool contains(T val) {
		if (!head) return false;
		auto curr = head;
		while (curr) {
			if (curr->val == val) return true;
			curr = curr->next;
		}
		return false;
	}

	bool empty() {
		return !head;
	}

	size_t size() {
		if (!head) return 0;
		auto curr = head;
		size_t i = 0;
		while (curr) {
			i++;
			curr = curr->next;
		}
		return i;
	}


	bool has_cycle() {
		if (!head) return false;
		ListNode<T> *tortoise = head;
		ListNode<T> *hare = head;
		while (tortoise->next && hare->next && hare->next->next) {
			tortoise = tortoise->next;
			hare = hare->next->next;
			if (tortoise == hare) return true;
		}
		return false;
	}

	std::string to_str() {
		if (!head) return "[]";
		std::unordered_set<ListNode<T>*> seen;
		// unordered map would be faster for larger N

		ListNode<T> *current = head;

		std::ostringstream oss;
		bool contains_cycle = has_cycle();
		if (contains_cycle) oss << ansi::bold << ansi::underline;
		oss << "[";
		int i = 0;
		while (current) {
			if (seen.contains(current)) {
				// locate the cycle
				oss << ansi::fg_red;
				break;
			}
			seen.insert(current);
			oss << current->val;
			if (current->next && !(seen.contains(current->next))) oss << ",";
			current = current->next;
			i++;
		}

		if (contains_cycle) {
			std::string copied = oss.str().erase(0, 8);
			oss << "-->" << copied.substr(1) << "] (CYCLE DETECTED)";
		} else {
			oss << "]";
		}
		return oss.str();
	}

	void print() {
		std::cout << to_str() << std::endl;
	}

};
