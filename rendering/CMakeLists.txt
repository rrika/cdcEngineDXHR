target_sources(dxhr PRIVATE
	Culling/BasicPrimitives.cpp
	Culling/Primitives.cpp

	buffers/PCDX11DynamicIndexBuffer.cpp
	buffers/PCDX11DynamicVertexBuffer.cpp
	buffers/PCDX11StaticIndexBuffer.cpp
	buffers/PCDX11StaticVertexBuffer.cpp

	drawables/PCDX11ClearDrawable.cpp
	drawables/PCDX11FastBlurDrawable.cpp
	drawables/PCDX11FXAADrawable.cpp
	drawables/PCDX11MLAADrawable.cpp
	drawables/PCDX11SetRTDrawable.cpp

	shaders/PCDX11ComputeShader.cpp
	shaders/PCDX11PixelShader.cpp
	shaders/PCDX11ShaderLib.cpp
	shaders/PCDX11VertexShader.cpp

	surfaces/PCDX11BitmapTexture.cpp
	surfaces/PCDX11DefaultRenderTarget.cpp
	surfaces/PCDX11DepthBuffer.cpp
	surfaces/PCDX11RenderTexture.cpp
	surfaces/PCDX11Texture.cpp

	BuiltinResources.cpp
	CommonMaterial.cpp
	CommonRenderDevice.cpp
	CommonRenderTerrainInstance.cpp
	CommonScene.cpp
	LinearAllocator.cpp
	PCDX11DeviceManager.cpp
	PCDX11ImmediateDraw.cpp
	PCDX11InternalResource.cpp
	PCDX11LightManager.cpp
	PCDX11Material.cpp
	PCDX11MatrixState.cpp
	PCDX11ModelDrawable.cpp
	PCDX11Pool.cpp
	PCDX11RenderContext.cpp
	PCDX11RenderDevice.cpp
	PCDX11RenderLight.cpp
	PCDX11RenderModel.cpp
	PCDX11RenderModelInstance.cpp
	PCDX11RenderPassCallbacks.cpp
	PCDX11RenderTerrain.cpp
	PCDX11Scene.cpp
	PCDX11StateManager.cpp
	PCDX11StaticPool.cpp
	PCDX11StreamDecl.cpp
	PCDX11StreamDeclCache.cpp
	PCDX11TerrainDrawable.cpp
	PIXTracker.cpp
	PrimitiveContext.cpp
	Projection.cpp
	RangeAllocator.cpp
	RenderPasses.cpp
	RenderResource.cpp
	VertexDeclaration.cpp)

if (ENABLE_IMGUI)
	target_sources(dxhr PRIVATE
		Inspector.cpp)
endif()
