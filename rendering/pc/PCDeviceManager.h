#pragma once
#include <windows.h>
#include "../IPCDeviceManager.h"

struct IDirect3D9;
struct IDirect3D9Device;

namespace cdc {

class PCDeviceManager :
	public IPCDeviceManager
{
	HMODULE d3d9lib; // 18
	IDirect3D9 *d3d9;
	IDirect3D9Device *d3d9Device;
	DisplayConfig config1; // 15C
	DisplayConfig config2; // 1E4
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
	void method_20() override;
	void method_24() override;

	IDirect3D9 *getD3D() { return d3d9; }
	IDirect3D9Device *getD3DDevice() { return d3d9Device; }
	// PCStateManager *getStateManager() { return stateManager; }
	// PCShaderManager *getShaderManager() { return &shaderManager; }
};

extern PCDeviceManager *deviceManager9;
PCDeviceManager *createPCDeviceManager();

}
