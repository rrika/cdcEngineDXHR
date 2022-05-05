#pragma once
#include "PCDX11AsyncCreateResource.h"
#include "PCDX11InternalResource.h"
#include "CommonShader.h"

namespace cdc {

class PCDX11VertexShader :
	public PCDX11AsyncCreateResource,
	public PCDX11InternalResource
{
public:
	ID3D11VertexShader *m_d3dShader;
	ShaderSub m_sub;

	PCDX11VertexShader() {}
	PCDX11VertexShader(ID3D11VertexShader *shader) {
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
