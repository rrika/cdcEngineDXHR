#pragma once
#include "../matrix.h"
#include "CommonLightManager.h"
#include "PCDX11InternalResource.h"
#include "RenderPasses.h" // for DrawableList
#include <vector>

namespace cdc {

class PCDX11BitmapTexture;
class PCDX11ConstantBuffer;
class PCDX11LightManager;
class PCDX11RenderDevice;
class PCDX11RenderLight;

struct RenderLightData {
	PCDX11RenderLight *renderLight0;
	uint32_t scene4;
	uint32_t dword8;
	uint8_t gapC[4];
	float float10;
	uint32_t dword14;
	float float18;
	float float1C;
	uint32_t dword20;
	float float24;
	float float28;
	float float2C;
	uint32_t dword30;
	float float34;
	float float38;
	float float3C;
	uint32_t dword40;
	uint32_t dword44;
	float float48;
	uint32_t dword4C;
	float float50;
	float float54;
	float float58;
	float float5C;
	uint32_t dword60;
	uint32_t dword64;
	uint32_t dword68;
	uint32_t dword6C;
	uint32_t dword70;
	uint8_t byte74;
	uint8_t byte75;
	uint8_t gap76[2];
	float float78;
	float float7C;
	float float80;
	uint8_t gap84[12];
	float4x4 matrix4x490;
	float4x4 matrix4x4D0;
	uint8_t gap110[32];
	uint32_t dword130;
	uint32_t dword134;
	uint32_t dword138;
	uint32_t dword13C;
	uint32_t dword140;
	uint32_t dword144;
	uint32_t dword148;
	uint32_t dword14C;
	uint8_t gap150[64];
	DrawableList drawableList190;
	DrawableList drawableList1A0;
	DrawableList drawableList1B0;
	DrawableList drawableList1C0;
	uint32_t dword1D0;
	uint8_t gap1D4[268];
	float m128_2E0[4];
	float m128_2F0[4];
	uint32_t dword300;
	uint8_t byte304;

	void render(void*, void*);
	void render1(void*, void*);
	void render2(void*, void*);
};

struct LightReceiverData {
	uint8_t byte0;
	uint8_t pad[3];
	uint16_t word4;
	uint16_t word6;
	RenderLightData *dword8[16];
	PCDX11ConstantBuffer *cb48;
	PCDX11BitmapTexture *GlobalModulationMapTextures[3];
};

struct u32pair {
	uint32_t dword0;
	uint32_t dword4;
};

struct LightManagerSubA {
	std::vector<RenderLightData*> lightData;
	std::vector<PCDX11RenderLight*> lights;
};

struct LightManagerSubB {
	PCDX11RenderLight *lights[1024];
	uint32_t count;
};

struct LightManagerSubC {
	RenderLightData *array0[12];
	uint32_t array30[12];
	u32pair array60[6];
	uint32_t array90[12];
	uint32_t arrayC0[12];
	uint32_t dwordF0;
	uint32_t dwordF4;
	uint32_t dwordF8;
	uint32_t dwordFC;
	uint32_t dword100;

	void renderLights(PCDX11LightManager*);
};

class PCDX11LightManager :
	public PCDX11InternalResource,
	public CommonLightManager
{
	PCDX11RenderDevice *renderDevice; // 10
	// uint32_t dword14;
	// uint32_t dword18;
	// uint32_t dword1C;
	// uint8_t f20[768];
	// uint32_t dword320;
	// uint8_t f324[260];
	// uint32_t dword428;
	// uint8_t f42C[4];
	// PCDX11BitmapTexture *texture430;
	// uint32_t dword434;
	// PCDX11StaticConstantBuffer *staticConstantBuffer438;
	// LightManagerSubA subA[16]; // 43C
	// uint32_t dword5FC;
	// char char600;
	// uint8_t f601[2047];
	// uint32_t dwordE00;
	// uint32_t dwordE04;
	// LightManagerSubB *subB;
	// LightReceiverData *receiver_E0C;
	// LightReceiverData *receiver_E10;
	// uint32_t *mostRecentAssignmentToCommonCB5; // E14
	// PCDX11DepthBuffer *depthBuffer; // E18
	// uint32_t dwordE1C;
	// uint32_t dwordE20;
	// LightManagerSubC subC;

public:
	LightManagerSubB *allocateSubB();
	void fillLightBuffer(LightReceiverData *receiverData);
	LightReceiverData *makeReceiver();
	void renderLights(LightManagerSubB *subB);
	void assignCommonCB5(char *src);

	bool internalCreate() override { /*TODO*/ return true; }
	void internalRelease() override { /*TODO*/ }

	void lightManager4() override { /*TODO*/ }
	void lightManager8() override { /*TODO*/ }
};

}
