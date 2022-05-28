#pragma once
#include <unordered_map>

namespace cdc {

struct StreamDeclCacheKey {
	uint32_t hashB;
	uint32_t hashA;
	bool operator==(StreamDeclCacheKey const& other) const {
		return hashB == other.hashB && hashA == other.hashB;
	}
};

}

template<>
struct std::hash<cdc::StreamDeclCacheKey> {
	std::size_t operator()(cdc::StreamDeclCacheKey const& k) const noexcept {
		return k.hashB;
	}
};

namespace cdc {

class PCDX11RenderDevice;
class PCDX11ShaderSub;
class PCDX11StreamDecl;
struct VertexAttributeLayoutA;
struct VertexAttributeLayoutB;

struct PCDX11StreamDeclCache {

	PCDX11StreamDeclCache(PCDX11RenderDevice *renderDevice) :
		renderDevice(renderDevice)
	{}

	PCDX11RenderDevice *renderDevice;
	std::unordered_map<StreamDeclCacheKey, PCDX11StreamDecl *> cache;
	uint32_t dword18;

	PCDX11StreamDecl *buildStreamDecl(
		VertexAttributeLayoutA *layoutA,
		PCDX11ShaderSub *shaderSub);

	PCDX11StreamDecl *buildStreamDecl(
		VertexAttributeLayoutA *layoutA,
		VertexAttributeLayoutB *layoutB,
		bool a4,
		PCDX11ShaderSub *shaderSub);
};

}
