#pragma once
#include "cdcMath/Math.h" // for Matrix
#include "buffers/PCDX11UberConstantBuffer.h"
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

class PCDX11DeferredLight { // line 531
	struct LightParams {
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
	};

	PCDX11RenderLight *renderLight0;
	uint32_t scene4;
	uint32_t dword8;
	uint8_t gapC[4];
	LightParams m_params; // 10
	uint32_t dword70;
	uint8_t byte74;
	uint8_t byte75;
	uint8_t gap76[2];
	float float78;
	float float7C;
	float float80;
	uint8_t gap84[12];
	Matrix m_worldToLight; // 90
	Matrix m_lightToWorld; // D0
	Matrix m_modulationTransform; // 110
	Matrix m_shadowMapTransform; // 150
	DrawableList drawableList190;
	DrawableList drawableList1A0;
	DrawableList drawableList1B0;
	DrawableList drawableList1C0;
	uint32_t dword1D0;
	uint8_t gap1D4[268];
	Vector m128_2E0;
	Vector m128_2F0;
	uint32_t dword300;
	uint8_t byte304;

	void RenderShadowMap(const uint32_t *pOffset, uint32_t size);
	void RenderShadowMapSpot(const uint32_t *pOffset, uint32_t size);
	void RenderShadowMapDirectional(const uint32_t *pOffset, uint32_t size);
};

struct PCDX11LightSet { // light receiver
	PCDX11LightSet *nextMaybe;
	uint16_t word4;
	uint16_t word6;
	PCDX11DeferredLight *dword8[16];
	PCDX11ConstantBuffer *cb48;
	PCDX11BitmapTexture *GlobalModulationMapTextures[3];
};

struct u32pair {
	uint32_t dword0;
	uint32_t dword4;
};

struct LightManagerSubA {
	std::vector<PCDX11DeferredLight*> lightData;
	std::vector<PCDX11RenderLight*> lights;
};

struct PCDX11LightManagerData {
	PCDX11RenderLight *lights[1024];
	uint32_t count;
};

class PCDX11LightManager :
	public PCDX11InternalResource,
	public CommonLightManager
{
	struct ShadowMapPool {
		PCDX11DeferredLight *m_pLights[12];
		uint32_t array30[12];
		u32pair array60[6];
		uint32_t array90[12];
		uint32_t arrayC0[12];
		uint32_t dwordF0;
		uint32_t dwordF4;
		uint32_t m_numLights; // F8
		uint32_t dwordFC;
		uint32_t dword100;

		void Render(PCDX11LightManager*);
	};

	PCDX11RenderDevice *renderDevice; // 10
	// uint32_t dword14;
	// uint32_t dword18;
	// uint32_t dword1C;
	// uint8_t f20[768];
	// uint32_t dword320;
	// uint8_t f324[260];
	// uint32_t dword428;
	// uint8_t f42C[4];
	PCDX11BitmapTexture *attenuationSampler430;
	// uint32_t dword434;
	// PCDX11StaticConstantBuffer *staticConstantBuffer438;
	PCDX11UberConstantBuffer staticConstantBuffer438 {69}; // HACK
	// LightManagerSubA subA[16]; // 43C
	// uint32_t dword5FC;
	// char char600;
	// uint8_t f601[2047];
	// uint32_t dwordE00;
	// uint32_t dwordE04;
	// PCDX11LightManagerData *subB;
	// PCDX11LightSet *receiver_E0C;
	// PCDX11LightSet *receiver_E10;
	// uint32_t *mostRecentAssignmentToCommonCB5; // E14
	// PCDX11DepthBuffer *depthBuffer; // E18
	uint32_t pixelShaderIndexBaseE1C;
	uint32_t pixelShaderIndexBaseE20;
	// ShadowMapPool m_shadowMapPool;

public:
	PCDX11LightManager(CommonRenderDevice *renderDevice);
	~PCDX11LightManager();

	PCDX11LightManagerData *BeginSubFrame();
	void ApplySinglePassLightsInternal(PCDX11LightSet *receiverData);
	PCDX11LightSet *BuildLightState();
	void BeginFlush(PCDX11LightManagerData *subB);
	void ApplyIrradianceStateInternal(char *src);
	void UpdateCombinedLightTexture();

	bool internalCreate() override;
	void internalRelease() override;
	void internalOnSettingsChanged() override;

	void lightManager4() override { /*TODO*/ }
	void lightManager8() override { /*TODO*/ }
};

}
