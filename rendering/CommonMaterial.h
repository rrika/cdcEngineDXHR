#pragma once
#include <cstdint>
#include "IMaterial.h"

namespace cdc {

class TextureMap;
struct MaterialBlob;
struct MaterialInstanceData;

class CommonMaterial : public IMaterial {
	uint32_t m_renderPassesMask; // 4
	uint32_t m_renderPassesMaskFading; // 8
public:
	MaterialBlob *materialBlob = nullptr;
public:
	uint32_t GetId() override;
	void Release() override = 0;
	void SetRenderPassMask(uint32_t mask, bool fading) override;
	uint32_t GetRenderPassMask(bool fading) override;
	~CommonMaterial() = default;
	virtual void method_18() = 0;

	uint32_t GetBlendMode(); // 32
	uint32_t GetFadeBlendMode(); // 52
	void InitInstanceData(MaterialInstanceData*, TextureMap**); // 116
	void SetRenderPasses(); // 199
};

}
