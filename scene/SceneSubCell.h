#pragma once
#include <vector>

namespace cdc {

class SceneEntity;

class SceneSubCell {
public:
	std::vector<SceneEntity*> entities; // static ones followed by dynamic ones
};

}
