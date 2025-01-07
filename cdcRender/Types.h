#pragma once
#include <cstdint>

namespace cdc {

enum TextureClass {
	kTextureClassUnknown = 0,
	kTextureClass2D = 1,
	kTextureClass3D = 2,
	kTextureClassCube = 3,
	kTextureClassNormalMap = 4
};

enum PassIndex {
	kPassIndexDepth = 0,
	kPassIndexComposite = 1,
	kPassIndexOpaque = 2,
	kPassIndexTranslucent = 3,
	kPassIndexFullScreenFX = 4,
	kPassIndexPostFSX = 5,
	kPassIndexAlphaBloomFSX = 6,
	kPassIndexPredator = 7,
	kPassIndex8 = 8,
	kPassIndexShadow = 9,
	kPassIndexDepthDependent = 10,
	// 
	kPassIndexNormal = 12,
	kPassIndexDeferredShading = 13,
	kPassIndexNonNormalDepth = 14,
	kMaxRenderPasses = 32
};

enum RenderFunction {
	kRenderFunctionDefault = 0,
	kRenderFunctionDepth = 1,
	kRenderFunctionShadow = 2,
	kRenderFunctionOpaque = 3,
	kRenderFunctionComposite = 4,
	kRenderFunctionTranslucent = 5,
	kRenderFunctionPredator = 6,
	kRenderFunctionAlphaBloomFSX = 7,
	kRenderFunction8 = 8,
	// 9
	kRenderFunctionNormal = 10,
	kRenderFunctionFirstCustom = 11,
	kRenderFunctionXRay = 11,
	kMaxRenderFunctions = 20,
	kRenderFunctionInvalid = 21
};

enum DrawableTypeID { // 184
	kDrawableTypeIDDefault = 0,
	kDrawableTypeIDModel = 1,
	kDrawableTypeIDTerrain = 2,
	kDrawableTypeIDNGAPrimitves = 3,
	kDrawableTypeFirstCustom = 4,
	kMaxDrawableTypes = 16,
};

enum : uint32_t {
	POLYFLAG_BLENDMODE   =        0x7,

	POLYFLAG_DOUBLESIDED =       0x40,
	POLYFLAG_NOCOLOR     =      0x100,
	POLYFLAG_FADING      =      0x400, // no depth test either
	POLYFLAG_SORTKEY     =     0x1000, // use the provided sort key

	POLYFLAG_TRILIST     =    0x00000,
	POLYFLAG_QUADLIST    =    0x10000,
	POLYFLAG_TRISTRIP    =    0x20000,
	POLYFLAG_LINELIST    =    0x30000,
	POLYFLAG_PRIMTYPE    =    0x30000,

	POLYFLAG_POSTFSX     =    0x40000, // force the respective pass mask
	POLYFLAG_BGRSWAP     =   0x200000, // RGB <-> BGR swap
	POLYFLAG_BRIGHTER    =   0x400000,
	POLYFLAG_FSFX        =  0x1000000, // force the respective pass mask
	POLYFLAG_2D          = 0x10000000,
	POLYFLAG_DEPTHBIAS   = 0x20000000  // see PCDX11Material::setupDepthBias
};

}
