#include "PCDX11VertexShader.h"
#include "PCDX11DeviceManager.h"
#include <d3d11.h>

namespace cdc {

void PCDX11VertexShader::asyncCreate() {
	deviceManager->getD3DDevice()->CreateVertexShader(
		m_sub.shaderBinary, m_sub.shaderBinarySize, 0, &m_d3dShader);
}

bool PCDX11VertexShader::internalCreate() {
	requestShader();
	return true;
}

void PCDX11VertexShader::internalRelease() {
	if (m_d3dShader)
		m_d3dShader->Release();
	m_d3dShader = nullptr;
}

}
