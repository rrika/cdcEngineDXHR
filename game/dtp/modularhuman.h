#pragma once
#include <cstdint>

namespace cdc {
class IMaterial;
class RenderMesh;
}

namespace dtp {

struct BaseMeshInfo;

struct ModularHuman {
	uint32_t dword0;
	uint32_t dword4;
	cdc::IMaterial *material8;
	cdc::IMaterial *materialC;
	cdc::IMaterial *material10;
	cdc::RenderMesh *upperBody;
	cdc::IMaterial *material18;
	cdc::IMaterial *material1C;
	cdc::RenderMesh *hands;
	cdc::IMaterial *material24;
	cdc::RenderMesh *lowerBody;
	cdc::IMaterial *material2C;
	cdc::IMaterial *material30;
	cdc::IMaterial *material34;
	BaseMeshInfo *baseMeshInfo38;
	uint32_t count3C;
};

}
