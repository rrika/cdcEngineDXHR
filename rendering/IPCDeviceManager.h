#pragma once
#include <cstdint>

namespace cdc {

struct DisplayConfig {
	uint16_t fullscreenWidth;
	uint16_t fullscreenHeight;
	uint8_t fullscreenRefreshRate;
	uint8_t fullscreen;
	uint8_t showBorder;
	uint8_t lockWindowResolution;
	uint8_t lockWindowAspect;
	uint8_t lockWindowSize;
	uint8_t pad_A;
	uint8_t pad_B;
	uint32_t lockedWindowWidth;
	uint32_t lockedWindowHeight;
	uint32_t aspectRatio;
	uint8_t enableVsync;
	uint8_t enableTripleBuffer;
	uint8_t disableHardwareVP;
	uint8_t disablePureDevice;
	uint8_t useRefDevice;
	uint8_t useD3DFPUPreserve;
	uint8_t disableDriverManagement;
	uint8_t disableDynamicTextures;
	uint8_t disableNonPow2Textures;
	uint8_t disableHardwareShadowMaps;
	uint8_t atiForceFetch4;
	uint8_t disableNullRenderTargets;
	uint8_t dontDeferShaderCreation;
	uint8_t useShaderUsageData;
	uint8_t createGameFourCC;
	float brightness;
	float contrast;
	uint8_t presentationIntervalTwo;
	uint32_t bestTextureFilter;
	uint8_t enablePostProcess;
	uint8_t enableGDICursor;
	uint32_t antiAliasing;
	uint32_t shadowMode;
	uint32_t ssaoMode;
	uint32_t dofMode;
	uint32_t adapterId;
	uint32_t adapterIdentifier50;
	uint32_t adapterIdentifier54;
	uint32_t adapterIdentifier58;
	uint32_t adapterIdentifier5C;
	uint32_t dword60;
	uint32_t dx11AdapterId;
	uint32_t dword68;
	uint32_t dword6C;
	uint32_t dx11OutputId;
	uint8_t sampleCount;
	uint32_t sampleQuality;
	uint8_t enableTesselation;
	uint32_t stereoMode;
	uint8_t enableDirectX11;
};

struct MonitorInfo {
	uint32_t left;
	uint32_t right;
	uint32_t top;
	uint32_t bottom;
};

class IPCDeviceManager {
	virtual void method_00() = 0;
	virtual DisplayConfig *getDisplayConfig() = 0;
	virtual void setBrightness(float) = 0;
	virtual void method_0C() = 0;
	virtual void method_10() = 0;
	virtual void method_14() = 0;
	virtual void method_18() = 0;
	virtual void method_1C() = 0;
	virtual void method_20() = 0;
	virtual void method_24() = 0;
};

}
