#pragma once

namespace cdc {

template <typename T>
struct SList {

	struct Node {
		Node *next;
		T item;
	};

	struct iterator {
		Node **link;
		inline T& operator*() { return (*link)->item; }
		inline T& operator->() { return (*link)->item; }
		inline void operator++() { link = &(*link)->next; }
		inline bool operator==(iterator const& other) { return *link == *other.link; }
		inline bool operator!=(iterator const& other) { return *link != *other.link; }
	};

	Node *head = nullptr;
	inline static Node *endPtr = nullptr;

	~SList() {
		Node *node = head;
		while (node) {
			Node *next = node->next;
			delete node;
			node = next;
		}
	}

	inline void push_front(T item) {
		head = new /*(alloc8)*/ Node{head, item};
	}

	inline iterator begin() { return {&head}; }
	inline iterator end() { return {&endPtr}; }
	inline bool empty() { return head != nullptr; }

	inline void erase(iterator it) {
		if (auto link = it.link) {
			Node *erasedLink = *link;
			if (erasedLink) {
				*link = erasedLink->next;
				delete erasedLink;
			} else {
				*link = nullptr;
			}
		}
	}
};

}
