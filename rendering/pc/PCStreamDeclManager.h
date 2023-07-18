#pragma once
#include <unordered_map>

namespace cdc {

struct StreamDeclCacheKey9 {
	uint32_t hashB;
	uint32_t hashA;
	bool operator==(StreamDeclCacheKey9 const& other) const {
		return hashB == other.hashB && hashA == other.hashA;
	}
};

}

template<>
struct std::hash<cdc::StreamDeclCacheKey9> {
	std::size_t operator()(cdc::StreamDeclCacheKey9 const& k) const noexcept {
		return k.hashB;
	}
};

namespace cdc {

class PCRenderDevice;
class PCStreamDecl;
struct PCShaderBinary;
struct VertexDecl;

class PCStreamDeclManager {
	PCRenderDevice *renderDevice; // 0
	std::unordered_map<StreamDeclCacheKey9, PCStreamDecl *> cache;

public:
	PCStreamDeclManager(PCRenderDevice *renderDevice) :
		renderDevice(renderDevice)
	{}

	PCStreamDecl *FindOrCreate(VertexDecl *vertexDecl);
};

}
