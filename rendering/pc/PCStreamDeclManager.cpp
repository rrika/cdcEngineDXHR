#include <d3d9.h>
#include "rendering/pc/PCStreamDecl.h"
#include "rendering/pc/PCStreamDeclManager.h"
#include "rendering/VertexDeclaration.h"

namespace cdc {

PCStreamDecl *PCStreamDeclManager::FindOrCreate(VertexDecl *layout)
{
	StreamDeclCacheKey9 key {0, layout->hash0};
	auto it = cache.insert({key, nullptr});
	PCStreamDecl *&streamDecl = it.first->second;

	if (it.second) {
		D3DVERTEXELEMENT9 *elements = new D3DVERTEXELEMENT9[layout->numAttr];
		memset(elements, 0, sizeof(D3DVERTEXELEMENT9) * layout->numAttr);
		MakeD3DVertexElements(elements, layout->attrib, layout->numAttr);
		streamDecl = new PCStreamDecl(elements, layout->vertStrideA);
	}

	return streamDecl;
}

}
