#pragma once
#include <cstdint>
#include "../math/Math.h" // for float4x4
#include "IRenderScene.h"
#include "IRenderDrawable.h"
#include "surfaces/CommonRenderTarget.h" // for CommonRenderTarget to IRenderTarget cast
#include "surfaces/CommonDepthBuffer.h" // for CommonDepthBuffer to IDepthBuffer cast

namespace cdc {

class CommonRenderDevice;
class CommonScene;
class DrawableListsAndMasks;
class RenderPasses;

struct CommonSceneSub10 {
	float cameraDirection[4]; // 50
	float cameraPosition[4]; // 60
	uint32_t dwordC0; // C0, for PCDX11DepthPassCallbacks::pre
	uint32_t mask; // E0
};

struct CommonSceneSub114 {
	TextureMap *tex14[13] = {0}; // 14
	// 14 [0]
	// 18 [1]
	// 1C [2]
	// 20 [3]
	// 24 [4]
	// 28 [5]
	// 2C [6]
	// 30 [7]
	// 34 [8]
	// 38 [9]
	// 3C [10] depthRenderTexture
	// 40 [11]
	// 44 [12]
};

class CommonScene :
	public IRenderScene,
	public IRenderDrawable
{
public:
	CommonSceneSub10 sub10;
	float fogColor[4];
	// float float110;
	CommonSceneSub114 sub114;
	uint8_t byte25C; // reset by PCDX11CompositePassCallbacks::post
	// uint8_t f25D[2];
	// Matrix4x4 mat260;
	// char field_29F;
	// CommonRenderDevice *commonRenderDevice2A0;
	// uint32_t dword2A4;
	// uint32_t dword2A8;
	// uint32_t dword2AC;
	float4x4 viewMatrix;
	float4x4 projectMatrix;
	// Matrix4x4 mat330;
	// Matrix4x4 mat370;
	// Matrix4x4 mat3B0_maybe;
	CommonRenderTarget *renderTarget; //3F0
	CommonDepthBuffer *depthBuffer; // 3F4
	// uint32_t dword3F8;
	// uint32_t dword3FC;
	// uint32_t dword400;
	CommonScene *parentScene; // 404
	CommonScene *nextScene; // 408
	CommonScene *prevScene; // 40C
	float heightFogParams[4];
	float fogScaleOffset[4];
	// uint8_t f430[4];
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
	uint32_t numSubScenes;
  	CommonScene *subScenes[18];

public:
	CommonScene(
		CommonRenderDevice *renderDevice,
		/* ... */
		CommonScene *parentScene,
		CommonSceneSub10 *sub10,
		CommonRenderTarget *renderTarget,
		CommonDepthBuffer *depthBuffer,
		/* ... */
		CommonSceneSub114 *sub114,
		/* ... */
		RenderPasses *renderPasses)
	:
		renderTarget(renderTarget),
		parentScene(parentScene),
		nextScene(nullptr),
		prevScene(nullptr),
		depthBuffer(depthBuffer),
		numSubScenes(0)
	{
		this->sub10 = *sub10;
		this->sub114 = *sub114;
	}

	float4x4& getViewMatrix() override { return viewMatrix; }
	void scene4() override {}
	float4x4& getProjectMatrix() override { return projectMatrix; }
	void sceneC() override {}
	void getCameraPosition(float *pos) override {
		pos[0] = sub10.cameraPosition[0];
		pos[1] = sub10.cameraPosition[1];
		pos[2] = sub10.cameraPosition[2];
		pos[3] = sub10.cameraPosition[3];		
	}
	void getCameraDirection(float *dir) override {
		dir[0] = sub10.cameraDirection[0];
		dir[1] = sub10.cameraDirection[1];
		dir[2] = sub10.cameraDirection[2];
		dir[3] = sub10.cameraDirection[3];
	}
	CommonSceneSub10& getSceneSub10() override { return sub10; }
	IRenderTarget *getRenderTarget() override { return renderTarget; }
	IDepthBuffer *getDepthBuffer() override { return depthBuffer; }
	void scene24() override {}
	void scene28() override {}

	// void draw(uint32_t funcSetIndex, IRenderDrawable *other) = 0;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other) override { return true; }

	void setSharedTextureToRenderTarget(CommonRenderTarget *rt, uint32_t slot, uint32_t) {
		if (rt) {
			sub114.tex14[slot + 5] = rt->getRenderTexture();
			// TODO
		}
	}
	void setSharedTextureToDepthBuffer(CommonDepthBuffer *db, uint32_t slot) {
		if (db) {
			sub114.tex14[slot + 5] = db->getRenderTexture();
			// TODO
		}
	}
};

}
