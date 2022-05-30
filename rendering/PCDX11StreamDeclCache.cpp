#include "buffers/PCDX11UberConstantBuffer.h"
#include "PCDX11DeviceManager.h"
#include "PCDX11StreamDecl.h"
#include "PCDX11StreamDeclCache.h"
#include "shaders/PCDX11Shader.h"
#include "VertexAttribute.h"

namespace cdc {

PCDX11StreamDecl *PCDX11StreamDeclCache::buildStreamDecl(
	VertexAttributeLayoutA *layout,
	PCDX11ShaderSub *shaderSub)
{
	StreamDeclCacheKey key {0, layout->hash0};
	auto it = cache.insert({key, nullptr});
	PCDX11StreamDecl *&streamDecl = it.first->second;

	if (it.second) {
		D3D11_INPUT_ELEMENT_DESC *inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[layout->numAttr];
		memset(inputElementDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC[layout->numAttr]));
		decodeVertexAttribA(inputElementDesc, layout->attrib, layout->numAttr, shaderSub->wineWorkaround);
		streamDecl = new PCDX11StreamDecl(renderDevice, inputElementDesc, layout->numAttr, shaderSub);

		// HACK
		auto buffer = new PCDX11UberConstantBuffer(2);
		float rows[] = {
			2.0f, -1.0f, 0.0f, 0.0f, // NormalScaleOffset
			1.0f,  0.0f, 0.0f, 0.0f  // TexcoordScales
		};
		buffer->assignRow(0, rows, 2);
		buffer->syncBuffer(deviceManager->getD3DDeviceContext());
		streamDecl->streamDeclBuffer = buffer;
	}

	return streamDecl;
}

PCDX11StreamDecl *PCDX11StreamDeclCache::buildStreamDecl(
	VertexAttributeLayoutA *layoutA,
	VertexAttributeLayoutB *layoutB,
	bool a4,
	PCDX11ShaderSub *shaderSub)
{
	return buildStreamDecl(layoutA, shaderSub); // HACK
}

}
