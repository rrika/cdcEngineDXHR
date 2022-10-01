#pragma once
#include <cstdint>

namespace cdc {

class IRenderDrawable {
protected:
	friend class DrawableList;
	uint16_t typeID = 0; // kDrawableTypeIDDefault
	uint16_t order = 0;
	float sortZ = 0.0f;
public:
	virtual void draw(uint32_t funcSetIndex, IRenderDrawable *other) = 0;
	virtual uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other) = 0;
	uint32_t getVtable() { return *(uint32_t*)this; }
};

}
