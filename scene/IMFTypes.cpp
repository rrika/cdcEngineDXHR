#include "IMFTypes.h"
#include "cdcResource/DTPDataSection.h"

namespace cdc {

dtp::IntermediateMesh *GetIMFPointerFromId(uint32_t id) {
	return (dtp::IntermediateMesh *)DTPDataSection::getPointer(id);
}

}
