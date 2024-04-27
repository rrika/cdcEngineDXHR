#pragma once

class Instance;

namespace cdc {

class PlayerAPI {
public:
	virtual ~PlayerAPI() = default;
	virtual float getHealth() = 0; // 4
	// method_8
	// ...
	// method_48
};

class CdcPlayer {
public:
	class IDelegate {
	public:
		virtual bool init(CdcPlayer*) = 0;
		virtual bool method_4(CdcPlayer*) = 0;
		virtual bool uninit_a(CdcPlayer*) = 0;
		virtual bool uninit_b(CdcPlayer*) = 0;
	};

	inline static IDelegate *s_delegate = nullptr;

	PlayerAPI *playerAPI = nullptr; // 0
	Instance *instance = nullptr; // 4
};

}
