#pragma once
#include <windows.h>
#include "IPCDeviceManager.h"
#include "shaders/PCDX11ShaderManager.h"

struct ID3D11Device;
struct IDXGIFactory;
struct ID3D11DeviceContext;

namespace cdc {

class PCDX11StateManager;

class PCDX11DeviceManager :
	public IPCDeviceManager
{
	HMODULE d3d11;
	HMODULE dxgi;
	ID3D11Device *device;
	IDXGIFactory *dxgiFactory;
	ID3D11DeviceContext *deviceContext;
	DisplayConfig config1;
	DisplayConfig config2;
	PCDX11ShaderManager shaderManager; // actually a pointer
public:
	PCDX11StateManager *stateManager;

	PCDX11DeviceManager(HMODULE d3d11, HMODULE dxgi);
	PCDX11DeviceManager(HMODULE d3d11, HMODULE dxgi, ID3D11Device*, ID3D11DeviceContext*);
	void method_00();
	DisplayConfig *getDisplayConfig();
	void setBrightness(float brightness);
	bool isConfigAcceptable(DisplayConfig*);
	void method_10();
	void method_14();
	void method_18();
	void method_1C();
	void method_20();
	void method_24();

	ID3D11Device *getD3DDevice() { return device; }
	IDXGIFactory *getDxgiFactory() { return dxgiFactory; }
	ID3D11DeviceContext *getD3DDeviceContext() { return deviceContext; }
	PCDX11StateManager *getStateManager() { return stateManager; }
	PCDX11ShaderManager *getShaderManager() { return &shaderManager; }
};

extern PCDX11DeviceManager *deviceManager;
PCDX11DeviceManager *createPCDX11DeviceManager();

}
