#pragma once
#include <cstdint>
#include <functional>

struct GFxValue {
	void *objectInterface = nullptr;
	enum ValueType {
		VT_Undefined        = 0x00,
		VT_Null             = 0x01,
		VT_Boolean          = 0x02,
		VT_Number           = 0x03,
		VT_String           = 0x04,
		VT_StringW          = 0x05,
		VT_Object           = 0x06,
		VT_Array            = 0x07,
		VT_DisplayObject    = 0x08,
	} type;
	union {
		double number;
		bool boolean;
		const char *string;
	};

	GFxValue() : type(VT_Undefined) {}
	GFxValue(bool b) : type(VT_Boolean), boolean(b) {}
};

class GFxState {
public:
	enum StateType {
		State_ExternalInterface = 9
	};
};

class GFxMovieView {
public:
	// GFxMovieView method
	virtual void SetExternalInterfaceRetVal(const GFxValue&) = 0; // C8

	// GFxStateBag method
	virtual void SetState(GFxState::StateType state, GFxState* pstate) = 0; // 8

	// custom additions
	virtual void init() = 0;
	virtual void buildUI() = 0;
};

class GFxMovieDef {
public:
	struct MemoryParams {
		// don't care
	};

	virtual GFxMovieView *CreateInstance(MemoryParams const& memParams, bool initFirstFrame) = 0;
};

class GFxExternalInterface : public GFxState {
public:
	virtual ~GFxExternalInterface() = default; // 0
	virtual void Callback(GFxMovieView *pMovieView, const char *methodName, const GFxValue *args, uint32_t numArgs) = 0; // 4
};

class GFxMovieRoot : public GFxMovieView {
protected:
	GFxExternalInterface *externalInterface = nullptr;
	GFxValue lastReturnValue;
public:
	void SetExternalInterfaceRetVal(const GFxValue&) override;
	void SetState(GFxState::StateType state, GFxState* pstate) override;
};

class GFxMovieDefImpl : public GFxMovieDef {
	std::function<GFxMovieView*()> createInstance;

public:
	GFxMovieDefImpl(std::function<GFxMovieView*()> createInstance) : createInstance(createInstance) {}
	GFxMovieView *CreateInstance(MemoryParams const& memParams, bool initFirstFrame) override;
};

class ScaleformMovie;
extern ScaleformMovie mainMenuMovie;
