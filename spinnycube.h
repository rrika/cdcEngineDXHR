#pragma once
#define ENABLE_IMGUI 1

class ID3D11Device;
class ID3D11DeviceContext;

int spinnyCube(
	HWND hwnd,
	ID3D11Device *device,
	ID3D11DeviceContext *deviceContext);
