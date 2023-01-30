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

	IDirect3D9 *getD3D() { return d3d9; }
	IDirect3DDevice9 *getD3DDevice() { return d3d9Device; }
	// PCStateManager *getStateManager() { return stateManager; }
	PCShaderManager *getShaderManager() { return shaderManager; }
};

extern PCDeviceManager *deviceManager9;
PCDeviceManager *createPCDeviceManager();

}
