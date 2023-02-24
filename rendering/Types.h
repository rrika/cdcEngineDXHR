#pragma once

namespace cdc {

enum TextureFilter { // line 59
	kTextureFilterPoint = 0,
	kTextureFilterBilinear = 1,
	kTextureFilterTrilinear = 2,
	kTextureFilterAnisotropic1X = 3,
	kTextureFilterAnisotropic2X = 4,
	kTextureFilterAnisotropic4X = 6,
	kTextureFilterAnisotropic8X = 10,
	kTextureFilterAnisotropic16X = 18,
	kTextureFilterBest = 256,
	kTextureFilterDefault = 18,
	kTextureFilterInvalid = 512
};

enum TextureClass { // line 80
	kTextureClassUnknown = 0,
	kTextureClass2D = 1,
	kTextureClass3D = 2,
	kTextureClassCube = 3,
	kTextureClassNormalMap = 4,
	kTextureClassVertex = 5
};

enum PassIndex { // line 91
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

enum DrawableTypeID { // line 184
	kDrawableTypeIDDefault = 0,
	kDrawableTypeIDModel = 1,
	kDrawableTypeIDTerrain = 2,
	kDrawableTypeIDNGAPrimitves = 3,
	kDrawableTypeFirstCustom = 4,
	kMaxDrawableTypes = 16,
};

enum RenderFunctions { // line 210
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

}
