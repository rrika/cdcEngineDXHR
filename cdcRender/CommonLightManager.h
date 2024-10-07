#pragma once

namespace cdc {

class CommonLightManager {
public:
	~CommonLightManager() = default;
	virtual void lightManager4() = 0;
	virtual void lightManager8() = 0;
};

}
