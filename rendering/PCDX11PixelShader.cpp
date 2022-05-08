#include "PCDX11PixelShader.h"
#include "PCDX11DeviceManager.h"
#include <d3d11.h>

// copied from PCDX11VertexShader

namespace cdc {

void PCDX11PixelShader::asyncCreate() {
	deviceManager->getD3DDevice()->CreatePixelShader(
		m_sub.shaderBinary, m_sub.shaderBinarySize, 0, &m_d3dShader);
}

void PCDX11PixelShader::internalResource04() {
	requestShader();
}

void PCDX11PixelShader::internalResource08() {
	if (m_d3dShader)
		m_d3dShader->Release();
	m_d3dShader = nullptr;
}

}
