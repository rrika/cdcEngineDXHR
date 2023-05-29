#include "PCDX11ComputeShader.h"
#include "../PCDX11DeviceManager.h"
#include <d3d11.h>

namespace cdc {

void PCDX11ComputeShader::createD3DShader() {
	deviceManager->getD3DDevice()->CreateComputeShader(
		m_sub.shaderBinary, m_sub.shaderBinarySize, 0, &m_d3dShader);
}

bool PCDX11ComputeShader::internalCreate() {
	return true;
}

void PCDX11ComputeShader::internalRelease() {
	if (m_d3dShader)
		m_d3dShader->Release();
	m_d3dShader = nullptr;
}

}
