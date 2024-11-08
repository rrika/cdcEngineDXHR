#pragma once
#include <unordered_set>

namespace cdc {

template <typename T>
using HashSet = std::unordered_set<T>;

}

/*
#include <cstdint>
#include "Allocator.h"
#include "Algorithm.h"

namespace cdc {

template <typename T>
struct HashSet {
	using value_type = T;
	using iterator = int;
	// TODO
	Pair<iterator, bool> insert(value_type const&);
};

}
*/
