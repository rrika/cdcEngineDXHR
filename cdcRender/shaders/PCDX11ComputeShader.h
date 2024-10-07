#pragma once
#include "../PCDX11AsyncCreateResource.h"
#include "../PCDX11InternalResource.h"
#include "PCDX11Shader.h"

struct ID3D11ComputeShader;

namespace cdc {

class PCDX11ComputeShader :
	public PCDX11InternalResource
{
public:
	ID3D11ComputeShader *m_d3dShader;
	PCDX11ShaderBinary m_sub;

	static PCDX11ComputeShader *create(
		char *blob, bool takeCopy, /*shaderManagerData,*/
		bool *isWrappedPtr, bool isWrapped)
	{
		PCDX11ShaderBinary sub(blob, /*takeCopy=*/false, isWrapped);
		// TODO
		auto *shader = new PCDX11ComputeShader(blob, takeCopy, isWrapped);
		// TODO
		return shader;
	}

	PCDX11ComputeShader() {}
	PCDX11ComputeShader(ID3D11ComputeShader *shader) {
		m_d3dShader = shader;
	}
	PCDX11ComputeShader(char *blob, bool takeCopy, bool isWrapped) :
		PCDX11InternalResource(),
		m_d3dShader(nullptr),
		m_sub(blob, takeCopy, isWrapped)
	{ // copied from PCDX11VertexShader
		internalCreateIfDeviceManagerAgrees();
	}

	void createD3DShader();

	bool internalCreate() override;
	void internalRelease() override;
};

}
