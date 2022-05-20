#pragma once
#include "IRenderDrawable.h"
#include "RenderMesh.h" // for MeshSub and MeshTab0

namespace cdc {

class PCDX11RenderModel;
class PCDX11RenderDevice;
class PCDX11StateManager;
class PCDX11StreamDecl;

class PCDX11ModelDrawable : public IRenderDrawable {

	PCDX11RenderModel *renderModel; // 10
	MeshSub *meshSub; // 14
	MeshTab0 *tab0; // 18

	uint32_t flags34_unknown_0 : 1; // 34
	uint32_t cullMode : 1;
	uint32_t flags34_unknown_2 : 6;
	uint32_t triangleCount : 24;

public:
	PCDX11ModelDrawable(
		PCDX11RenderModel *renderModel,
		MeshSub *meshSub,
		MeshTab0 *tab0)
	:
		renderModel(renderModel),
		meshSub(meshSub),
		tab0(tab0)
	{ // hack
		triangleCount = tab0[0].triangleCount;
	}

	void draw(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) override;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) override;

	void draw(
		PCDX11RenderDevice *renderDev,
		PCDX11StateManager *stateMgr,
		PCDX11StreamDecl *streamDecl,
		bool renderTwice);
};

}