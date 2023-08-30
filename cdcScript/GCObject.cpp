#include "GCObject.h"

namespace cdc {

GCObject::GCObject() {
	GarbageCollector::s_instance->AllocNode(this, /*markNode=*/ true);
}

GCObject::~GCObject() = default;
void GCObject::MarkChildren() { /* empty */ }
void GCObject::Finalize() { /* empty */ }
const char* GCObject::GetGCName() { return "EmptySymbol"; }

}