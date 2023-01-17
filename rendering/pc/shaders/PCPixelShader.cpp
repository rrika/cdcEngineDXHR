#include <d3d9.h>
#include "../PCDeviceManager.h"
#include "PCPixelShader.h"

namespace cdc {

void PCPixelShader::requestShader() {
	deviceManager9->getD3DDevice()->CreatePixelShader(
		(DWORD*)m_sub.shaderBinary, &m_d3dShader);
	// if (!m_d3dShader) tableflip();
}

bool PCPixelShader::internalCreate() {
	return true;
}

void PCPixelShader::internalRelease() {
	if (m_d3dShader)
		m_d3dShader->Release();
	m_d3dShader = nullptr;
}

bool PCPixelShader::internalResource0C() {
	return true;
}

void PCPixelShader::internalResource10() {
	// empty
}

}
