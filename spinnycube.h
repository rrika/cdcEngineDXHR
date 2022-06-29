#pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;

int spinnyCube(
	HWND hwnd,
	ID3D11Device *device,
	ID3D11DeviceContext *deviceContext);
