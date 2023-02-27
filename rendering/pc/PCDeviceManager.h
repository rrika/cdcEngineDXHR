#pragma once
#include <windows.h>
#include <d3d9.h>
#include "../IPCDeviceManager.h"

namespace cdc {

class PCRenderContext;
class PCShaderManager;

class PCDeviceManager :
	public IPCDeviceManager
{
	HWND hwnd; // 14
	HMODULE d3d9lib; // 18
	IDirect3D9 *d3d9; // 1C
	IDirect3DDevice9 *d3d9Device = nullptr;
	DisplayConfig config1; // 15C
	DisplayConfig config2; // 1E4
	D3DPRESENT_PARAMETERS presentParams; // 270
	PCShaderManager *shaderManager; // 2B4
	// PCStateManager *stateManager; // 2B8

	friend class PCRenderContext;

public:
	enum PCCaps { // line 50
		PC_CAPS_TEXTURE_NONPOW2 = 1,
		PC_CAPS_TEXTURE_MIPMAPS = 2,
		PC_CAPS_TEXTURE_DYNAMIC = 4,
		PC_CAPS_FULLSCREENEFFECTS = 8
	};

	PCDeviceManager(HMODULE d3d9, IDirect3D9*);
	void method_00() override;
	DisplayConfig *getDisplayConfig() override;
	void setBrightness(float brightness) override;
	bool isConfigAcceptable(DisplayConfig*) override;
	void method_10() override;
	void method_14() override;
	void method_18() override;
	void method_1C() override;
	void Init(HWND, DisplayConfig*) override;
	void method_24() override;

	bool InitializePresentParams(DisplayConfig*);
	void CreateDevice(DisplayConfig*);
	void ReleaseDevice(DeviceStatus status);
	bool CheckFormat(D3DFORMAT format, D3DRESOURCETYPE type, uint32_t usage);
	void OnCreateResourceFailed();

	IDirect3D9 *getD3D() { return d3d9; }
	IDirect3DDevice9 *getD3DDevice() { return d3d9Device; }
	// PCStateManager *getStateManager() { return stateManager; }
	PCShaderManager *getShaderManager() { return shaderManager; }

	uint32_t GetCaps() {
		uint32_t caps = // HACK: assume all available
			PC_CAPS_TEXTURE_NONPOW2 |
			PC_CAPS_TEXTURE_MIPMAPS |
			PC_CAPS_TEXTURE_DYNAMIC |
			PC_CAPS_FULLSCREENEFFECTS;

		DisplayConfig *dc = getDisplayConfig();
		if (dc->disableNonPow2Textures)
			caps &= ~PC_CAPS_TEXTURE_NONPOW2;
		if (dc->disableDynamicTextures)
			caps &= ~PC_CAPS_TEXTURE_DYNAMIC;
		return caps;
	}
};

extern PCDeviceManager *deviceManager9;
PCDeviceManager *createPCDeviceManager();

}
