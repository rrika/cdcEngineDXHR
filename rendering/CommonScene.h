#pragma once
#include <cstdint>
#include "IRenderScene.h"
#include "IRenderDrawable.h"

namespace cdc {

class CommonRenderDevice;
class CommonScene;
class CommonRenderTarget;
class CommonDepthBuffer;
class DrawableListsAndMasks;
class RenderPasses;

struct CommonSceneSub10 {
	uint32_t mask; // E0
};

struct CommonSceneSub114 {

};

class CommonScene :
	public IRenderScene,
	public IRenderDrawable
{
public:
	CommonSceneSub10 sub10;
	// float fogColor[4];
	// float float110;
	CommonSceneSub114 sub114;
	// uint8_t byte25C;
	// uint8_t f25D[2];
	// Matrix4x4 mat260;
	// char field_29F;
	// CommonRenderDevice *commonRenderDevice2A0;
	// uint32_t dword2A4;
	// uint32_t dword2A8;
	// uint32_t dword2AC;
	// Matrix4x4 mat2B0;
	// Matrix4x4 mat2F0;
	// Matrix4x4 mat330;
	// Matrix4x4 mat370;
	// Matrix4x4 mat3B0_maybe;
	CommonRenderTarget *renderTarget; //3F0
	CommonDepthBuffer *depthBuffer; // 3F4
	// uint32_t dword3F8;
	// uint32_t dword3FC;
	// uint32_t dword400;
	// CommonScene *otherScene404;
	// CommonScene *scene408;
	// uint32_t dword40C;
	// uint8_t f410[36];
	// float float434;
	// uint8_t byte438;
	// uint8_t f439[3];
	// uint32_t dword43C;
	// float float440;
	// float float444;
	// char char448[256];
	// int field_548;
	// int field_54C;
	// char field_550[256];
	// uint32_t dword650;
	// uint32_t dword654;
	// uint32_t dword658;
	// uint32_t dword65C;
	// uint32_t dword660;
	// uint32_t dword664;
	// float float668;
	// float float66C;
	// float float670;
	// float float674;
	// float float678;

	// uint32_t dword67C;
	// uint8_t f680[64];
	// uint32_t dword6C0;
	// uint32_t figureThisOut6C4;
	DrawableListsAndMasks *drawableListsAndMasks; // 6C8
	// int forwardToLightManagerPassMask6CC;
	// uint32_t dword6D0;
	// uint32_t dword6D4;
	// char field_6D4[120];
	// uint32_t dword750;
	// uint32_t dword754;

public:
	CommonScene(
		CommonRenderDevice *renderDevice,
		/* ... */
		CommonScene *otherScene,
		CommonSceneSub10 *sub10,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer,
		/* ... */
		CommonSceneSub114 *sub114,
		/* ... */
		RenderPasses *renderPasses)
	:
		renderTarget(renderTarget),
		depthBuffer(depthBuffer)
	{
		this->sub10 = *sub10;
		this->sub114 = *sub114;
	}

	void scene0() override {}
	void scene1() override {}
	void scene2() override {}
	void scene3() override {}
	void scene4() override {}
	void scene5() override {}
	void scene6() override {}
	void scene7() override {} // getRenderTarget
	void scene8() override {}
	void scene9() override {}
	void sceneA() override {}

	// void draw(uint32_t funcSetIndex, IRenderDrawable *other) = 0;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other) override { return true; }
};

}
