#include <iterator>
#include <windows.h>
#include <d3d9.h>
#include "PCDeviceManager.h"
#include "shaders/PCShaderManager.h"

namespace cdc {

PCDeviceManager::PCDeviceManager(
	HMODULE d3d9lib,
	IDirect3D9 *d3d9)
:
	d3d9lib(d3d9lib),
	d3d9(d3d9)
{
	shaderManager = new PCShaderManager();
	// stateManager = new PCStateManager();
/*
#if _WIN32
	auto createDXGIFactory = (decltype(&CreateDXGIFactory))(GetProcAddress(d3d9, "CreateDXGIFactory"));
	createDXGIFactory(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
#else
	IDXGIDevice *dxgiDevice;
	IDXGIAdapter *adapter;
	device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	dxgiDevice->GetAdapter(&adapter);
	adapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
#endif
*/
}

void PCDeviceManager::method_00() {
	// TODO
}

DisplayConfig *PCDeviceManager::getDisplayConfig() {
	return &config1;
}

void PCDeviceManager::setBrightness(float brightness) {
	config1.brightness = brightness;
}

bool PCDeviceManager::isConfigAcceptable(DisplayConfig*) {
	// TODO
	return true;
}

void PCDeviceManager::method_10() {
	// TODO
}

void PCDeviceManager::method_14() {
	// TODO
}

void PCDeviceManager::method_18() {
	// TODO
}

void PCDeviceManager::method_1C() {
	// TODO
}

void PCDeviceManager::method_20() {
	// TODO
}

void PCDeviceManager::method_24() {
	// TODO
}

PCDeviceManager *deviceManager9 = nullptr;

PCDeviceManager *createPCDeviceManager() {
	#if _WIN32
	HMODULE d3d9lib = LoadLibraryA("d3d9.dll");
	if (!d3d9lib)
		return 0;

	auto create9 = (decltype(&Direct3DCreate9))(GetProcAddress(d3d9lib, "Direct3DCreate9"));
	#else
	void *d3d9lib = nullptr;
	#endif

	IDirect3D9 *d3d9 = Direct3DCreate9(D3D_SDK_VERSION); // D3D_SDK_VERSION = 32 in DXHRDC release build
	if (!d3d9) {
		#if _WIN32
		FreeLibrary(d3d9lib);
		#endif
		return nullptr;
	}

	deviceManager9 = new PCDeviceManager(d3d9lib, d3d9);
	return deviceManager9;
}

}
