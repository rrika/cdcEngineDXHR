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

	ID3D11Device *getD3DDevice() { return device; }
	IDXGIFactory *getDxgiFactory() { return dxgiFactory; }
	ID3D11DeviceContext *getD3DDeviceContext() { return deviceContext; }
	PCDX11StateManager *getStateManager() { return stateManager; }
	PCDX11ShaderManager *getShaderManager() { return &shaderManager; }
};

extern PCDX11DeviceManager *deviceManager;
PCDX11DeviceManager *createPCDX11DeviceManager();

}
