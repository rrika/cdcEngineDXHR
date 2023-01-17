#pragma once
#include "../PCInternalResource.h"
#include "PCShaderBinary.h"

struct IDirect3DVertexShader9;

namespace cdc {

// unlike the DX11 equivalent, this is not an AsyncCreateResource
class PCVertexShader :
	public PCInternalResource
{
public:
	IDirect3DVertexShader9 *m_d3dShader;
	PCShaderBinary m_sub;
	uint32_t dword1C;
	// uint32_t dword20;

	static PCVertexShader *create(
		char *blob, bool takeCopy, /*shaderManagerData,*/
		bool *isWrappedPtr, bool isWrapped)
	{
		PCShaderBinary sub(blob, /*takeCopy=*/false, isWrapped);
		auto *shader = new PCVertexShader(blob, takeCopy, isWrapped);
		return shader;
	}

	PCVertexShader(char *blob, bool takeCopy, bool isWrapped) :
		PCInternalResource(),
		m_d3dShader(nullptr),
		m_sub(blob, takeCopy, isWrapped),
		dword1C(1)
	{
		internalCreateIfDeviceManagerAgrees();
	}

	void requestShader();

	bool internalCreate() override;
	void internalRelease() override;
	bool internalResource0C() override;
	void internalResource10() override;
};

}
