#pragma once
#include <windows.h>
#include <vector>
#include <dxgiformat.h>
#include "IPCDeviceManager.h"
#include "shaders/PCDX11ShaderManager.h"

struct ID3D11Device;
struct IDXGIFactory;
struct ID3D11DeviceContext;
struct ID3DUserDefinedAnnotation;

namespace cdc {

class PCDX11StateManager;

class PCDX11DeviceManager :
	public IPCDeviceManager
{
public:
	struct AdapterInfo {
		// uint32_t ordinal; // 0
		// wchar_t name[512]; // 4
		// IDXGIAdapter *pD3DAdapter; // 404
		// DXGI_FORMAT displayFormat; // 408
		DXGI_FORMAT backBufferFormat; // 40C
		DXGI_FORMAT depthTextureFormat; // 410
		// std::vector<PCDX11Output> outputs; // 414
		// std::vector<PCDisplayMode> stereoDisplayModes; // 420
		// uint32_t dword424;
		// DXGI_ADAPTER_DESC *adapterDesc;
		// uint32_t featureLevel;
	};

private:
	HMODULE d3d11; // 1C
	HMODULE dxgi; // 20
	ID3D11Device *device; // 24
	IDXGIFactory *dxgiFactory; // 28
	ID3D11DeviceContext *deviceContext; // 2C
	DisplayConfig config1; // 34
	DisplayConfig config2; // BC
	PCDX11ShaderManager shaderManager; // 14C, actually a pointer
	ID3DUserDefinedAnnotation *annotation = nullptr; // HACK

public:
	PCDX11StateManager *stateManager; // 150
	std::vector<AdapterInfo> adapters; // 154
	AdapterInfo *currentAdapter; // 160

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

	void StartMarker(const char *) override;
	void StartMarker(const wchar_t *) override;
	void EndMarker() override;

	ID3D11Device *getD3DDevice() { return device; }
	IDXGIFactory *getDxgiFactory() { return dxgiFactory; }
	ID3D11DeviceContext *getD3DDeviceContext() { return deviceContext; }
	PCDX11StateManager *getStateManager() { return stateManager; }
	PCDX11ShaderManager *getShaderManager() { return &shaderManager; }
};

extern PCDX11DeviceManager *deviceManager;
PCDX11DeviceManager *createPCDX11DeviceManager();

}
