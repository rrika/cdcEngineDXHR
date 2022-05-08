#include "PCDX11PixelShader.h"
#include "PCDX11DeviceManager.h"
#include <d3d11.h>

namespace cdc {

void PCDX11PixelShader::asyncCreate() {
	deviceManager->getD3DDevice()->CreatePixelShader(
		m_sub.shaderBinary, m_sub.shaderBinarySize, 0, &m_d3dShader);
}

bool PCDX11PixelShader::internalCreate() {
	// why? vertex shader does this different
	return true;
}

void PCDX11PixelShader::internalRelease() {
	if (m_d3dShader)
		m_d3dShader->Release();
	m_d3dShader = nullptr;
}

}
