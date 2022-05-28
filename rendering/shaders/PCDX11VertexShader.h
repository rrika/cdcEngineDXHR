#pragma once
#include "../PCDX11AsyncCreateResource.h"
#include "../PCDX11InternalResource.h"
#include "PCDX11Shader.h"

class ID3D11VertexShader;

namespace cdc {

class PCDX11VertexShader :
	public PCDX11AsyncCreateResource,
	public PCDX11InternalResource
{
public:
	ID3D11VertexShader *m_d3dShader;
	PCDX11ShaderSub m_sub;

	static PCDX11VertexShader *create(
		char *blob, bool takeCopy, /*shaderManagerData,*/
		bool *isWrappedPtr, bool isWrapped)
	{
		PCDX11ShaderSub sub(blob, /*takeCopy=*/false, isWrapped);
		// TODO
		auto *shader = new PCDX11VertexShader(blob, takeCopy, isWrapped);
		// TODO
		return shader;
	}

	PCDX11VertexShader() {}
	PCDX11VertexShader(ID3D11VertexShader *shader) { // hack
		m_requested = true;
		m_keepWaiting = false;
		m_d3dShader = shader;
	}
	PCDX11VertexShader(char *blob, bool takeCopy, bool isWrapped) :
		PCDX11AsyncCreateResource(),
		PCDX11InternalResource(),
		m_d3dShader(nullptr),
		m_sub(blob, takeCopy, isWrapped)
	{
		// dword28 = true;
		internalCreateIfDeviceManagerAgrees();
	}

	void requestShader() {
		m_d3dShader = 0;
		request(0);
	}

	void asyncCreate() override;

	bool internalCreate() override;
	void internalRelease() override;
};

}
