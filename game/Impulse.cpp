#include "Impulse.h"
#include <forward_list>
#include <unordered_map>

// named after the generic console command in half-life

struct ImpulseEntry {
	uint32_t index;
	void *key;
	void *userdata;
	ImpulseFunc *funcptr;
	const char *comment;
};

static std::unordered_map<void*, std::forward_list<ImpulseEntry>> impulse_table[467];

void impulse_register(uint32_t index, void *key, void *userdata, ImpulseFunc *funcptr, const char *comment) {
	impulse_table[index][key].push_front(ImpulseEntry {
		index,
		key,
		userdata,
		funcptr,
		comment
	});
}

void impulse_call(uint32_t index, void *arg, void *key) {
	// TODO: increment recursion counter
	auto& map = impulse_table[index];
	if (key) {
		// for this specific key
		if (auto it = map.find(key); it != map.end()) {
			for (auto& entry : it->second) {
				entry.funcptr(entry.userdata, arg);
			}
		}
	} else {
		// for all keys
		for (auto& list : map) {
			for (auto& entry : list.second) {
				entry.funcptr(entry.userdata, arg);
			}
		}
	}
	// TODO: decrement recursion counter
	// TODO: deferred calls
}
