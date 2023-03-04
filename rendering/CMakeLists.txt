target_sources(dxhr PRIVATE
	drawables/PCDX11ClearDrawable.cpp
	drawables/PCDX11FastBlurDrawable.cpp
	drawables/PCDX11FXAADrawable.cpp
	drawables/PCDX11MLAADrawable.cpp
	drawables/PCDX11SetRTDrawable.cpp

	pc/shaders/PCPixelShader.cpp
	pc/shaders/PCShaderLib.cpp
	pc/shaders/PCVertexShader.cpp

	shaders/PCDX11PixelShader.cpp
	shaders/PCDX11ShaderLib.cpp
	shaders/PCDX11VertexShader.cpp

	pc/surfaces/PCDeviceBaseTexture.cpp
	pc/surfaces/PCDeviceTexture.cpp
	pc/surfaces/PCTexture.cpp

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

	pc/PCDeviceManager.cpp
	pc/PCInternalResource.cpp
	pc/PCMaterial.cpp
	pc/PCRenderContext.cpp
	pc/PCRenderDevice.cpp
	pc/PCStateManager.cpp

	PCDX11DeviceManager.cpp
	PCDX11InternalResource.cpp
	PCDX11LightManager.cpp
	PCDX11Material.cpp
	PCDX11MatrixState.cpp
	PCDX11ModelDrawable.cpp
	PCDX11RenderContext.cpp
	PCDX11RenderDevice.cpp
	PCDX11RenderLight.cpp
	PCDX11RenderModel.cpp
	PCDX11RenderModelInstance.cpp
	PCDX11RenderPassCallbacks.cpp
	PCDX11RenderTerrain.cpp
	PCDX11Scene.cpp
	PCDX11StateManager.cpp
	PCDX11StreamDecl.cpp
	PCDX11StreamDeclCache.cpp
	PCDX11TerrainDrawable.cpp

	Projection.cpp
	RenderPasses.cpp
	RenderResource.cpp
	VertexDeclaration.cpp)

if (ENABLE_IMGUI)
	target_sources(dxhr PRIVATE
		Inspector.cpp)
endif()
