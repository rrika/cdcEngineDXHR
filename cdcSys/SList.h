#pragma once

namespace cdc {

template <typename T>
struct Link {
	Link *next;
	T item;
};

template <typename T>
struct LinkIterator {
	Link<T> *link;
	inline T& operator*() { return link->item; }
	inline void operator++() { link = link->next; }
	inline bool operator!=(LinkIterator const& other) { return link != other.link; }
};

template <typename T>
struct SList {
	Link<T> *head = nullptr;

	inline void insert(T item) {
		head = new /*(alloc8)*/ Link<T>{head, item};
	}

	inline LinkIterator<T> begin() { return {head}; }
	inline LinkIterator<T> end() { return {nullptr}; }
};

}
