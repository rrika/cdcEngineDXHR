#pragma once
#include "PCDX11AsyncCreateResource.h"
#include "PCDX11InternalResource.h"
#include "PCDX11Shader.h"

class ID3D11PixelShader;

namespace cdc {

class PCDX11PixelShader :
	public PCDX11AsyncCreateResource,
	public PCDX11InternalResource
{
public:
	ID3D11PixelShader *m_d3dShader;
	PCDX11ShaderSub m_sub;

	PCDX11PixelShader() {}
	PCDX11PixelShader(ID3D11PixelShader *shader) {
		m_requested = true;
		m_keepWaiting = false;
		m_d3dShader = shader;
	}

	void requestShader() {
		m_d3dShader = 0;
		request(0);
	}

	void asyncCreate() override {};

	void internalResource04() override {};
	void internalResource08() override {};
};

}
