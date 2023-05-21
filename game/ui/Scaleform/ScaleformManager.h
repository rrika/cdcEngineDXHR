#pragma once
#include <cstdint>
#include <string>
#include <map>

class ScaleformManager {
public:
	static void Initialize();
};

extern std::map<std::string, uint32_t> scaleformStringMap;
