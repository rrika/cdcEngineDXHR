#pragma once

namespace cdc {

struct MaterialBlob;

class IMaterial {
public:
	virtual void load(MaterialBlob*) = 0;
	virtual MaterialBlob *GetMaterialData() = 0;
	virtual void Release() = 0;
	virtual void SetRenderPassMask(uint32_t mask, bool fading) = 0;
	virtual uint32_t GetRenderPassMask(bool fading) = 0;
	virtual ~IMaterial() = default;
};

}
