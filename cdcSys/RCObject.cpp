#include "RCObject.h"

namespace cdc {

HandleData *HandleData::s_handles = nullptr;
HandleData *HandleData::s_free = nullptr;

void InitHandlePool(uint32_t count) { // 13
	if (HandleData::s_handles)
		return;
	HandleData::s_handles = new HandleData[count];
	HandleData::s_free = HandleData::s_handles + 1;
	// index 0 is reserved
	// index 1 is free, m_object points to next free (2)
	// index 2 is free, m_object points to next free (3)
	// etc.
	for (uint32_t i=0; i<count-1; i++)
		HandleData::s_handles[i].m_object = &HandleData::s_handles[i+1];
}

void FreeHandlePool() { // 33
	// TODO
}

bool IsHandlePoolValid() { // 50
	// guess
	return HandleData::s_handles != nullptr;
}

uint32_t GetHandleCount() { // 55
	// TODO
	return 0;
}

uint16_t HandleData::Create(void *object) { // 69
	if (!IsHandlePoolValid())
		InitHandlePool(0x2000);
	uint16_t x = HandleData::s_free - HandleData::s_handles;
	HandleData::s_free = (HandleData*)HandleData::s_free->m_object;
	HandleData::s_handles[x].m_object = object;
	return x;
}

RCObject::~RCObject() { // 88
	// this will call dtor of m_handleOwner
}

void RCObject::Remove() { // 93
	delete this;
}

}
