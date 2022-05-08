#include "PCDX11VertexShader.h"
#include "PCDX11DeviceManager.h"
#include <d3d11.h>

namespace cdc {

void PCDX11VertexShader::asyncCreate() {
	deviceManager->getD3DDevice()->CreateVertexShader(
		m_sub.shaderBinary, m_sub.shaderBinarySize, 0, &m_d3dShader);
}

void PCDX11VertexShader::internalResource04() {
	requestShader();
}

void PCDX11VertexShader::internalResource08() {
	if (m_d3dShader)
		m_d3dShader->Release();
	m_d3dShader = nullptr;
}

}
