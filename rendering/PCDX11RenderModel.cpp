#include "PCDX11RenderModel.h"
#include <cstring>
#include <cstdio>

namespace cdc {

PCDX11RenderModel::PCDX11RenderModel(PCDX11RenderDevice *renderDevice, uint32_t size) :
	RenderMesh(),
	// RenderExternalResource(renderDevice),
	size(size)
{
	meshHeader = (MeshHeader*)new char[size];
}

void PCDX11RenderModel::resFill(void* src, size_t size, size_t offset) {
	memcpy((char*)meshHeader + offset, src, size);
}

char *PCDX11RenderModel::resGetBuffer() {
	return (char*)meshHeader;
}

void PCDX11RenderModel::resConstruct() {
	mesh = meshHeader->mesh;
	char *magic = (char*)&mesh->magic;
	printf("reading mesh with magic \"%c%c%c%c\" ", magic[0], magic[1], magic[2], magic[3]);
	if (mesh->magic == 0x6873654D)
		printf("OK\n");
	else
		printf("NG\n");
}

}
