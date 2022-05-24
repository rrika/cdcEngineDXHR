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

	uint32_t flags34; // 34

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
		flags34 = (tab0[0].triangleCount << 8);
	}

	inline bool isUnlit() const { return (flags34 >> 0) & 1; }
	inline bool getCullMode() const { return (flags34 >> 1) & 1; }
	inline uint32_t getTriangleCount() const { return (flags34 >> 8); }

	static bool draw1(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool draw2(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool draw7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool draw4(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool draw56(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool drawA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);

	static bool compare128(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool compare7(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool compare46(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);
	static bool compareA(uint32_t funcSetIndex, IRenderDrawable *drawable, IRenderDrawable *prevDrawable);

	void draw(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) override;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *prevDrawable) override;

	void draw(
		PCDX11RenderDevice *renderDev,
		PCDX11StateManager *stateMgr,
		PCDX11StreamDecl *streamDecl,
		bool renderTwice);
};

}