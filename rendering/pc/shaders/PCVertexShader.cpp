#include <d3d9.h>
#include "../PCDeviceManager.h"
#include "PCVertexShader.h"

namespace cdc {

void PCVertexShader::requestShader() {
	deviceManager9->getD3DDevice()->CreateVertexShader(
		(DWORD*)m_sub.shaderBinary, &m_d3dShader);
	// if (!m_d3dShader) tableflip();
}

bool PCVertexShader::internalCreate() {
	return true;
}

void PCVertexShader::internalRelease() {
	if (m_d3dShader)
		m_d3dShader->Release();
	m_d3dShader = nullptr;
}

bool PCVertexShader::internalResource0C() {
	return true;
}

void PCVertexShader::internalResource10() {
	// empty
}

}
