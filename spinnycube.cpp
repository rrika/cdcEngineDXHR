#include <cmath>
#include <cstdio>
#include <functional>
#include <iterator>
#include <memory>
#include "config.h" // for ENABLE_IMGUI and ENABLE_D3DCOMPILER

#include <windows.h>
#include <d3d11_1.h>
#if ENABLE_D3DCOMPILER
#include <d3dcompiler.h>
#endif

#include "spinnycube.h"
#include "camera/CameraManager.h"
#include "camera/GenericCamera.h"
#include "cdc/dtp/objectproperties/imfref.h"
#include "cdc/dtp/objectproperties/intermediatemesh.h"
#include "cdc/dtp/objectproperties/sfxmarker.h"
#include "cdc/dtp/soundplex.h"
#include "cdcFile/ArchiveFileSystem.h"
#include "cdcFile/FileHelpers.h" // for archiveFileSystem_default
#include "cdcFile/FileSystem.h" // for enum cdc::FileRequest::Priority
#include "cdcFile/FileUserBufferReceiver.h"
#include "game/dtp/objecttypes/globaldatabase.h"
#include "game/DeferredRenderingObject.h"
#include "game/Gameloop.h"
#include "game/script/game/NsMainMenuMovieController.h"
#include "game/ui/FakeScaleform/fakescaleform.h"
#include "game/ui/Scaleform/ScaleformMovieInstance.h"
#include "cdcGameShell/cdcGameShell.h" // for LOAD_IMFFileName
#ifdef _WIN32
#include "cdcGameShell/win32/MainVM.h" // for yellowCursor
#endif
#include "input/PCMouseKeyboard.h"
#include "cdcLocale/localstr.h"
#include "cdcMath/Math.h" // for cdc::Matrix
#include "cdcObjects/Object.h"
#include "cdcObjects/ObjectManager.h" // for buildObjectsUI
#include "rendering/buffers/PCDX11ConstantBufferPool.h"
#include "rendering/buffers/PCDX11IndexBuffer.h"
#include "rendering/buffers/PCDX11UberConstantBuffer.h"
#include "rendering/Culling/Primitives.h"
#include "rendering/Culling/Primitives_inlines.h"
#include "rendering/Culling/BasicPrimitives_inlines.h"
#include "rendering/drawables/PCDX11FXAADrawable.h"
#include "rendering/CommonRenderTerrainInstance.h"
#include "rendering/IPCDeviceManager.h"
#include "rendering/IRenderPassCallback.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/PCDX11Material.h" // for CommonMaterial to PCDX11Material cast
#include "rendering/PCDX11MatrixState.h"
#include "rendering/PCDX11RenderContext.h"
#include "rendering/PCDX11RenderDevice.h"
#include "rendering/PCDX11RenderModel.h"
#include "rendering/PCDX11RenderModelInstance.h"
#include "rendering/PCDX11RenderTerrain.h"
#include "rendering/PCDX11Scene.h"
#include "rendering/PCDX11StateManager.h"
#include "rendering/PCDX11StreamDecl.h"
#include "rendering/Projection.h"
#include "rendering/RenderModelInstance.h"
#include "rendering/RenderPasses.h"
#include "rendering/shaders/PCDX11PixelShader.h"
#include "rendering/shaders/PCDX11VertexShader.h"
#include "rendering/surfaces/PCDX11DefaultRenderTarget.h"
#include "rendering/surfaces/PCDX11DepthBuffer.h"
#include "rendering/surfaces/PCDX11Texture.h"
#include "rendering/VertexDeclaration.h"
#include "cdcResource/DRMIndex.h"
#include "cdcResource/ResolveObject.h"
#include "cdcResource/ResolveReceiver.h"
#include "cdcResource/ResolveSection.h"
#include "cdcResource/WaveSection.h"
#include "cdcScript/ScriptType.h"
#include "cdcSound/MultiplexStream.h"
#include "cdcScene/IMFTypes.h"
#include "cdcScene/IScene.h"
#include "cdcScene/SceneCellGroup.h" // for SceneCellGroup to ISceneCellGroup cast
#include "cdcScene/SceneEntity.h"
#include "cdcSound/SoundPlex.h"
#include "cdcWorld/RMIDrawableBase.h"
#include "cdcWorld/stream.h" // for buildUnitsUI
#include "cdcWorld/StreamUnit.h"
#include "cdcWorld/SceneLayer.h" // for g_scene

#if ENABLE_IMGUI
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx11.h"
#ifdef _WIN32
#include "imgui/backends/imgui_impl_win32.h"
#else
#include "imgui/backends/imgui_impl_sdl.h"
#endif
#include "rendering/Inspector.h"
#endif

#ifdef __linux__
#include <SDL2/SDL.h>
#endif

cdc::MultiplexStream *neverAskedForThis = nullptr;
uint32_t subtitleIndex = 0;

void howDoYouHandleAllOfThis() {
	if (!neverAskedForThis)
		neverAskedForThis = cdc::MultiplexStream::CreateSoundStream("vo\\eng\\det1\\adam_jensen\\sq02\\det1_sq02_dia_adam_006b", 0);

	((cdc::MultiplexStreamImpl*)neverAskedForThis)->hackSample->Play();
	subtitleIndex = 7497;
}

class ImGuiDrawable : public cdc::IRenderDrawable {
public:
	std::function<void()> lastMinuteAdditions;

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other) override { /*TODO*/ return 0; };
};

class SpinnyCubePass : public cdc::IRenderPassCallback {
public:
	D3D11_VIEWPORT *viewport;
	ID3D11BlendState1 *keepAlphaBlend;

	bool pre(
		cdc::CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		cdc::CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

DRMIndex drmIndex;

#if ENABLE_IMGUI
struct DRMExplorer {

	void draw(bool *showWindow) {
		ImGui::Begin("DRM Explorer", showWindow);

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {

			if (ImGui::BeginTabItem("Loaded DRMs")) {
				for (auto& entry : drmIndex.sectionHeaders) {
					if (ImGui::TreeNode(entry.first.c_str())) {
						uint32_t i=0;
						for (auto& section : entry.second) {
							const char *names[] = {
								"Generic",
								"Empty",
								"Animation",
								"",
								"",
								"RenderResource",
								"FMODSoundBank",
								"DTPData",
								"Script",
								"ShaderLib",
								"Material",
								"Object",
								"RenderMesh",
								"CollisionMesh",
								"StreamGroupList",
								"AnyType",
							};
							ImGui::Text("%3d: %04x %s allocFlags:%d unk6:%x (%d bytes)",
								i++, section.id, names[section.type], section.allocFlags, section.unknown06, section.payloadSize);
							if (section.type == 5) { // RenderResource
								ImGui::Text("    ");
								ImGui::SameLine();
								auto *resource = (cdc::RenderResource*)cdc::g_resolveSections[5]->getWrapped(section.id);
								if (auto tex = dynamic_cast<cdc::PCDX11Texture*>(resource)) {
									ImGui::Image(
										tex->createShaderResourceView(), ImVec2(256, 256));
								}
							}
							if (section.type == 6) { // FMOD
								ImGui::PushID(section.id);
								ImGui::SameLine();
								if (ImGui::SmallButton("Play sound")) {
									((cdc::WaveSection*)cdc::g_resolveSections[6])->playSound(section.id);
								}
								ImGui::PopID();
							}
							if (section.type == 7 && section.allocFlags == 0xD) { // DTP (SoundPlex)
								ImGui::PushID(section.id);
								ImGui::SameLine();
								auto *plex = (dtp::SoundPlex*)cdc::g_resolveSections[7]->getWrapped(section.id);
								if (plex) {
									if (ImGui::SmallButton("Play soundplex")) {
										cdc::SOUND_StartPaused(plex, /*delay=*/ 0.0f);
									}
									buildUI(plex, /*indent=*/ "    ");
								}
								ImGui::PopID();

							}
							if (section.type == 8) { // Script
								if (auto *ty = (cdc::ScriptType*)cdc::g_resolveSections[8]->getWrapped(section.id)) {
									ImGui::SameLine();
									ImGui::Text(" %s", ty->blob->name);
								}
							}
						}
						ImGui::TreePop();
					}
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}
} drmexplorer;
#endif

int spinnyCube(HWND window,
	ID3D11Device *baseDevice,
	ID3D11DeviceContext *baseDeviceContext) {

	localstr_set_language(language_english, language_default);

	std::unique_ptr<cdc::PCMouseKeyboard> mouseKeyboard(cdc::PCMouseKeyboard::create(window));
	auto renderDevice = static_cast<cdc::PCDX11RenderDevice*>(cdc::g_renderDevice);

	///////////////////////////////////////////////////////////////////////////////////////////////

	ID3D11Device1* device;

	baseDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&device));

	ID3D11DeviceContext1* deviceContext;

	baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&deviceContext));

	///////////////////////////////////////////////////////////////////////////////////////////////

	cdc::PCDX11RenderContext *renderContext = renderDevice->getRenderContextAny();
	renderContext->internalCreate();

	///////////////////////////////////////////////////////////////////////////////////////////////

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	renderContext->frameBuffer->GetDesc(&depthBufferDesc); // base on framebuffer properties

	{
		cdc::PCDX11RenderTarget& cdcRenderTarget = *renderContext->renderTarget2C;
		cdc::PCDX11DepthBuffer& cdcDepthBuffer = *renderContext->depthBuffer;
		cdcDepthBuffer.renderTexture.sampleCount = depthBufferDesc.SampleDesc.Count;
		cdcDepthBuffer.renderTexture.sampleQuality = depthBufferDesc.SampleDesc.Quality;

		cdcRenderTarget.getRenderTexture11()->createRenderTargetView();
		cdcDepthBuffer.renderTexture.createDepthStencilView();

		// do not keep the cdcRenderTarget and cdcDepthBuffer references around
		// these objects get destroyed on window resize
	}

	///////////////////////////////////////////////////////////////////////////////////////////////

#if ENABLE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
#ifdef _WIN32
	ImGui_ImplWin32_Init(window);
#else
	ImGui_ImplSDL2_InitForSDLRenderer((SDL_Window*)window, nullptr);
#endif
	ImGui_ImplDX11_Init(baseDevice, baseDeviceContext);
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////

	ID3D11BlendState1* keepAlphaBlend = NULL;

	D3D11_BLEND_DESC1 BlendState;
	memset(&BlendState, 0, sizeof(D3D11_BLEND_DESC1));
	BlendState.AlphaToCoverageEnable = false;
	BlendState.IndependentBlendEnable = false;
	BlendState.RenderTarget[0].BlendEnable = true;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; // ignore shader
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; // keep RT value
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState1(&BlendState, &keepAlphaBlend);

	///////////////////////////////////////////////////////////////////////////////////////////////

	cdc::PCDX11StateManager stateManager(deviceContext, device);
	stateManager.internalCreate();
	cdc::deviceManager->stateManager = &stateManager; // hack

	auto bottleIndex = cdc::objectIdByName("alc_beer_bottle_a");
	cdc::requestObjectNormal(bottleIndex);

	auto lightIndex = cdc::objectIdByName("deferred_fast_omni_diffuse");
	cdc::requestObjectNormal(lightIndex);

	// see below for why
	auto obj3 = cdc::ResolveObject::create(
		"pc-w\\shaderlibs\\deferred_fast_omni_diffusemt_79464a5e59514c7f_dx11.drm",
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		0,
		cdc::FileRequest::NORMAL
	);
	auto obj4 = cdc::ResolveObject::create(
		"pc-w\\s_scn_det_sarifhq_rail_tutorial_barrettintro_det_sarifhq_rail_tutorial.drm",
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		0,
		cdc::FileRequest::NORMAL
	);

	auto obj5 = cdc::ResolveObject::create(
		"pc-w\\globaloutershell.drm",
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		0,
		cdc::FileRequest::NORMAL
	);

	cdc::archiveFileSystem_default->processAll();

	cdc::ResolveSection *objectSection = cdc::g_resolveSections[11];
	cdc::Object *bottleObject = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(0x04a8));
	printf("have bottle object: %p\n", bottleObject);

	// unrelated: get the name of the first map in the game
	uint32_t globalDatabaseId = cdc::objectIdByName("GlobalDatabase");
	cdc::Object *globalDatabaseObject = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(globalDatabaseId));
	auto *globalDatabase = (GlobalDatabase*)globalDatabaseObject->data;

	printf("first map is: %s\n", globalDatabase->newGameMap);

	auto bottleTexture = (cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x0396);
	printf("have bottle cdc texture: %p\n", bottleTexture);
	bottleTexture->asyncCreate();
	renderDevice->missingTexture = bottleTexture;
	printf("have bottle d3d texture: %p\n", bottleTexture->d3dTexture128);

	// create the other four textures
	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x0395))->asyncCreate();
	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x005b))->asyncCreate();
	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x0061))->asyncCreate();

	auto bottleRenderModel_direct = (cdc::PCDX11RenderModel*)cdc::g_resolveSections[12]->getWrapped(0xA301);
	auto bottleRenderModel = (cdc::PCDX11RenderModel*)bottleObject->models[0]->renderMesh;


	printf("have bottle cdc render model: %p (directly)\n", bottleRenderModel_direct);
	printf("have bottle cdc render model: %p (via object)\n", bottleRenderModel);
	printf("have bottle cdc mesh blob: %p\n", bottleRenderModel->getMesh());

	for (uint32_t i = 0; i < bottleRenderModel->numPrimGroups; i++)
		printf("  bottle->tab0Ext128Byte[i].material = %p\n", bottleRenderModel->tab0Ext128Byte[i].material);

	RMIDrawableBase rmiDrawable(bottleRenderModel);

	auto sceneCube = g_scene->CreateEntity();
	sceneCube->setDrawable(&rmiDrawable);
	sceneCube->setCellGroup(g_scene->GetCellGroup(0));

	// cdc::RenderModelInstance *bottleRenderModelInstance =
	// 	renderDevice->createRenderModelInstance(bottleRenderModel);

	///////////////////////////////////////////////////////////////////////////////////////////////

	// for some reason deferred_fast_omni_diffuse.drm depends on shaderlibs/decal_853189b19db6f909_dx11.drm
	// when it should be using a shader from shaderlibs/decal_853189b19db6f909_dx11.drm
	// namely section 5: ShaderLib 0 cd2 unk6:2ce3 DX11 (4f38 bytes)
	//
	// one of the few materials referencing this shaderlib is
	// s_scn_det_sarifhq_rail_tutorial_barrettintro_det_sarifhq_rail_tutorial.drm/7: Material 12 a4 unk6:7a84 DX11 (7e0 bytes)


	cdc::Object *lightObject = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(lightIndex));
	printf("have light object: %p\n", lightObject);

	auto lightRenderModel = (cdc::PCDX11RenderModel*)lightObject->models[0]->renderMesh;
	printf("have light cdc render model: %p (via object)\n", lightRenderModel);

	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x0061))->asyncCreate();
	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x014c))->asyncCreate();

	auto lightMaterial = (cdc::PCDX11Material*)cdc::g_resolveSections[10]->getWrapped(0x00a4);
	printf("have light material (from scenario drm): %p\n", lightMaterial);

	// patch light material
	for (uint32_t i = 0; i < lightRenderModel->numPrimGroups; i++)
		lightRenderModel->tab0Ext128Byte[i].material = lightMaterial;

	cdc::RenderModelInstance *lightRenderModelInstance =
		renderDevice->createRenderModelInstance(lightRenderModel);

	cdc::Vector4 *lightInstanceParams = static_cast<cdc::CommonRenderModelInstance*>(lightRenderModelInstance)->ext->instanceParams;
	// taken from a random light draw in renderdoc
	float lightRadius = 2.0f;
	float invLightRadius = 1.0f / lightRadius;
	lightInstanceParams[0] = cdc::Vector4{1.0, 1.0, 2.0, 1.0}; // .xyz = viewspace light position perhaps
	lightInstanceParams[1] = cdc::Vector4{0.88235, 0.7098, 0.3451, 0.00}; // .xyz = light color
	lightInstanceParams[2] = cdc::Vector4{invLightRadius, invLightRadius, invLightRadius, 1.00}; // .x = 1/radius
	lightInstanceParams[3] = cdc::Vector4{0.80, 0.00, 0.00, 0.00}; // .x= light scale

	///////////////////////////////////////////////////////////////////////////////////////////////

	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(depthBufferDesc.Width), static_cast<float>(depthBufferDesc.Height), 0.0f, 1.0f };

	///////////////////////////////////////////////////////////////////////////////////////////////

	float w = viewport.Width / viewport.Height; // width (aspect ratio)
	float h = 1.0f;                             // height
	float n = 1.0f;                             // near
	float f = 100000.0f;                        // far

	float scale = 1.f;
	cdc::Vector modelRotation    = { 0.0f, 0.0f, 0.0f };
	cdc::Vector modelScale       = { scale, scale, scale };
	cdc::Vector modelTranslation = { 50.0f, 0.0f, 0.0f };

	bool mouseLook = false;
	bool useFrustumCulling = true;
	bool drawCellBoxes = false;
	bool applyFXAA = false;
	bool pointlessCopy = false;
	int showTempBuffer = -1;
	cdc::Vector cameraPos{0, 0, 0};

	///////////////////////////////////////////////////////////////////////////////////////////////

	GenericCamera camera;
	CameraManager cameraManager;

	cameraManager.activeCamera = &camera;

	///////////////////////////////////////////////////////////////////////////////////////////////

	cdc::RenderViewport renderViewport;
	renderViewport.nearz = n;
	renderViewport.farz = f;
	renderViewport.fov = 0.925f;
	renderViewport.aspect = w;
	renderViewport.mask = 0x310B; // pass 0, 12. 13, 1, and 8
	// pass 12 normals (function set 10, draw bottle normals)
	// pass 13 deferred shading (just contains a cleardrawable)
	// pass 1 composite (draw bottle textures)
	// pass 3 translucent
	// pass 8 runs last and is where I put imgui since it messes with the render state

	SpinnyCubePass cubePass;
	cubePass.viewport = &viewport;
	cubePass.keepAlphaBlend = keepAlphaBlend;
	renderDevice->setPassCallback(0, &cubePass);

	ImGuiDrawable imGuiDrawable;

	///////////////////////////////////////////////////////////////////////////////////////////////

#if ENABLE_IMGUI
	bool loadedSarifHQ = false;
	bool showDrawablesWindow = false;
	bool showFilesystemWindow = false;
	bool showObjectsWindow = false;
	bool showDRMWindow = false;
	bool showUnitsWindow = false;
	bool showLoadedUnitsWindow = false;
	bool showStringsWindow = false;
	std::vector<std::pair<void*, cdc::CommonScene*>> captures { { nullptr, nullptr } };
	uint32_t selectedCapture = 0;

	cdc::ScriptType *mainMenuScriptType =
		(cdc::ScriptType*)cdc::g_resolveSections[8]->getWrapped(0x154a7); // pc-w/globaloutershell.drm section 0xb7
	ScaleformMovieInstance mainMenuInstance(&mainMenuMovie);
	NsMainMenuMovieController mainMenuMovieController(mainMenuScriptType);

	// unsure how this link is established in the game
	mainMenuInstance.m_controllerArray.push_back(&mainMenuMovieController);
	mainMenuMovieController.movieInstance = &mainMenuInstance;

	mainMenuInstance.init();
#else
	mouseLook = true;
	mouseKeyboard->setCursorGrab(true);
#endif

	std::unordered_map<cdc::IRenderTerrain*, cdc::CommonRenderTerrainInstance *> renderTerrainInstances;

	cdc::GameShell::UpdateLoop(0.5);

	while (true)
	{
#ifdef _WIN32
		MSG msg;

		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			if (msg.message == WM_DESTROY) {
				PostQuitMessage(0);
				goto end;
			}
			if (msg.message == WM_QUIT) {
				goto end;
			}

			mouseKeyboard->processWndProc(msg.message, msg.wParam, msg.lParam);

			//if (msg.message == WM_KEYDOWN) return 0;
			DispatchMessageA(&msg);
		}
#else
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
#if ENABLE_IMGUI
			ImGui_ImplSDL2_ProcessEvent(&event);
#endif
			mouseKeyboard->processSDLEvent(&event);

			switch (event.type) {
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
						renderDevice->handleResize(event.window.data1, event.window.data2);
					break;
				case SDL_QUIT:
					goto end;
				default:
					break;
			}
		}
#endif

		mouseKeyboard->update();

		float forward = 0;
		float sideways = 0;
		float speedModifier = 100.0f;

#if ENABLE_IMGUI
		ImGui_ImplDX11_NewFrame();
#ifdef _WIN32
		ImGui_ImplWin32_NewFrame(); // this will reset our pretty cursor
#else
		ImGui_ImplSDL2_NewFrame();
#endif
		ImGui::NewFrame();

		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			mouseLook = false;
			mouseKeyboard->setCursorGrab(false);
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_Tab)) {
			mouseLook = !mouseLook;
			mouseKeyboard->setCursorGrab(mouseLook);
		}
		if (ImGui::IsKeyDown(ImGuiKey_W))
			forward += 1.0f;
		if (ImGui::IsKeyDown(ImGuiKey_S))
			forward -= 1.0f;
		if (ImGui::IsKeyDown(ImGuiKey_A))
			sideways -= 1.0f;
		if (ImGui::IsKeyDown(ImGuiKey_D))
			sideways += 1.0f;
		if (ImGui::IsKeyDown(ImGuiKey_ModShift))
			speedModifier *= 4.0f;

#ifdef _WIN32
		if (mouseLook) {
			// ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			SetCursor((HCURSOR)0);
			io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
		} else {
			// ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
			SetCursor((HCURSOR)yellowCursor); // ahh, much better
			io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
#endif
#endif

		// cubePass.viewport ->
		viewport = {
			0.0f, 0.0f,
			static_cast<float>(renderContext->width),
			static_cast<float>(renderContext->height),
			0.0f, 1.0f };
		float w = viewport.Width / viewport.Height; // width (aspect ratio)
		renderViewport.aspect = w;

		///////////////////////////////////////////////////////////////////////////////////////////

		cdc::Matrix rotateX   = { 1, 0, 0, 0, 0, static_cast<float>(cos(modelRotation.x)), -static_cast<float>(sin(modelRotation.x)), 0, 0, static_cast<float>(sin(modelRotation.x)), static_cast<float>(cos(modelRotation.x)), 0, 0, 0, 0, 1 };
		cdc::Matrix rotateY   = { static_cast<float>(cos(modelRotation.y)), 0, static_cast<float>(sin(modelRotation.y)), 0, 0, 1, 0, 0, -static_cast<float>(sin(modelRotation.y)), 0, static_cast<float>(cos(modelRotation.y)), 0, 0, 0, 0, 1 };
		cdc::Matrix rotateZ   = { static_cast<float>(cos(modelRotation.z)), -static_cast<float>(sin(modelRotation.z)), 0, 0, static_cast<float>(sin(modelRotation.z)), static_cast<float>(cos(modelRotation.z)), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		cdc::Matrix bottleScale = { modelScale.x, 0, 0, 0, 0, modelScale.y, 0, 0, 0, 0, modelScale.z, 0, 0, 0, 0, 1 };
		cdc::Matrix bottleTranslate = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, modelTranslation.x, modelTranslation.y, modelTranslation.z, 1 };
		cdc::Matrix cameraTranslate = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -cameraPos.x, -cameraPos.y, -cameraPos.z, 1 };
		cdc::Matrix lightScaleTranslate = { lightRadius * 2.0f, 0, 0, 0, 0, lightRadius * 2.0f, 0, 0, 0, 0, lightRadius * 2.0f, 0, 1, 1, 1, 1 };
		cdc::Matrix zUpWorld = {0, 0, 1, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};

		if (mouseLook) {
			modelRotation.x += mouseKeyboard->state.deltaY;
			modelRotation.y += mouseKeyboard->state.deltaX;
		}

		cdc::Matrix cameraRotate = rotateX * rotateY * zUpWorld;

		cameraPos += cdc::Vector{
			cameraRotate.m[0][2],
			cameraRotate.m[1][2],
			cameraRotate.m[2][2]} * (speedModifier * forward);
		cameraPos += cdc::Vector{
			cameraRotate.m[0][0],
			cameraRotate.m[1][0],
			cameraRotate.m[2][0]} * (speedModifier * sideways);

		///////////////////////////////////////////////////////////////////////////////////////////

		for (auto &unit : StreamTracker) {
			if (!unit.used)
				continue;
			cdc::Level *level = unit.level;
			if (!level)
				continue;

			cdc::CellGroupData *cellGroupData = level->sub50;
			uint32_t numCells = cellGroupData->header->numTotalCells;
			for (uint32_t i=0; i < numCells; i++) {
				auto *cell = cellGroupData->cells[i];

				if (cell->pHeader) {
					cdc::CellStreamGroupData *streamgroup = cell->pHeader->streamGroup50;
					if (streamgroup && streamgroup->resolveObject && isLoaded(streamgroup->resolveObject)) {
						dtp::IntermediateMesh *im = cdc::GetIMFPointerFromId(cell->pHeader->streamGroupDtp54);
						if (im && im->m_type == cdc::IMFType_TerrainInstance) {
							auto *rt = (cdc::IRenderTerrain *)im->pRenderModel;
							if (rt) {
								if (renderTerrainInstances.find(rt) != renderTerrainInstances.end())
									continue;
								auto *rti = (cdc::CommonRenderTerrainInstance*)renderDevice->createRenderTerrainInstance(rt);
								renderTerrainInstances[rt] = rti;
								printf("created RenderTerrainInstance %p for RenderTerrain %p\n", rti, rt);
							}
						}
					}
				}

				if (cell->pTerrainData) {
					cdc::IRenderTerrain *rt = cell->pTerrainData->pTerrain;
					if (rt) {
						if (renderTerrainInstances.find(rt) != renderTerrainInstances.end())
							continue;
						auto *rti = (cdc::CommonRenderTerrainInstance*)renderDevice->createRenderTerrainInstance(rt);
						renderTerrainInstances[rt] = rti;
						printf("created RenderTerrainInstance %p for RenderTerrain %p\n", rti, rt);
					}
				}
			}

			uint32_t numStreamGroups = cellGroupData->header->numStreamGroups;
			for (uint32_t i=0; i < numStreamGroups; i++) {
				cdc::CellStreamGroupData *streamgroup = &cellGroupData->streamgroups[i];
				if (!streamgroup->resolveObject && streamgroup->streamFileName) {
					char path[256];
					sprintf(path, "pc-w\\streamgroups\\%s.drm", streamgroup->streamFileName);
					printf("requesting %s\n", path);
					streamgroup->resolveObject = cdc::ResolveObject::create(
						path,
						nullptr,
						nullptr,
						nullptr,
						nullptr,
						nullptr,
						nullptr,
						0,
						cdc::FileRequest::NORMAL
					);
				}
			}
			cdc::archiveFileSystem_default->processAll();
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		cdc::Matrix viewMatrix = cameraRotate * cameraTranslate;
		camera.matrix = viewMatrix;
		cameraManager.update();
		viewMatrix = *cameraManager.getMatrix(); // wow, it's nothing

		cdc::PCDX11RenderTarget *tempRenderTarget = nullptr;

		if (applyFXAA && !pointlessCopy)
			tempRenderTarget = static_cast<cdc::PCDX11RenderTarget*>(renderDevice->dx11_createRenderTarget(
				100, 100, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, 0x18, cdc::kTextureClass2D));

		renderDevice->resetRenderLists();
		renderDevice->beginRenderList(nullptr);
		auto *scene = renderDevice->createSubScene(
			&renderViewport,
			tempRenderTarget ? tempRenderTarget : renderContext->renderTarget2C,
			renderContext->depthBuffer,
			nullptr,
			nullptr);
		scene->viewMatrix = viewMatrix;
		cdc::OrthonormalInverse3x4(&renderViewport.viewMatrix, viewMatrix);

		cdc::CullingFrustum cullingFrustum;
		cullingFrustum.Set(renderViewport);

		cdc::Matrix bottleWorldMatrix = bottleTranslate * bottleScale;

		cdc::PCDX11MatrixState lightMatrixState(renderDevice);
		lightMatrixState.resize(1);
		auto *lightWorldMatrix = reinterpret_cast<cdc::Matrix*>(lightMatrixState.poseData->getMatrix(0));
		*lightWorldMatrix = lightScaleTranslate;

		// add drawables to the scene
		float backgroundColor[4] = {0.025f, 0.025f, 0.025f, 1.0f};
		// float lightAccumulation[4] = {0.9f, 0.9f, 0.9f, 1.0f};
		float lightAccumulation[4] = {0.0f, 0.0f, 0.0f, 1.0f};

		renderDevice->clearRenderTarget(10, /*mask=*/ 1, 0.0f, backgroundColor, 1.0f, 0);
		renderDevice->clearRenderTarget(2, /*mask=*/ 0x2000, 0.0f, lightAccumulation, 1.0f, 0); // deferred shading buffer
		static_cast<cdc::PCDX11RenderModelInstance*>(lightRenderModelInstance)->baseMask = 0x2000; // deferred lighting
		lightRenderModelInstance->recordDrawables(&lightMatrixState);
		// static_cast<cdc::PCDX11RenderModelInstance*>(bottleRenderModelInstance)->baseMask = 0x1002; // normals & composite
		// bottleRenderModelInstance->recordDrawables(&bottleMatrixState);

		static_cast<cdc::PCDX11RenderModelInstance*>(rmiDrawable.rmi)->baseMask = 0x1002; // normals & composite

		std::vector<std::unique_ptr<RMIDrawableBase>> recycleRMI;

		auto putObject = [&](cdc::PCDX11RenderModel *renderModel, cdc::Matrix& instanceMatrix, DeferredRenderingExtraData *extraData) {
			RMIDrawableBase *selectedRmiDrawable = &rmiDrawable;
			if (renderModel) {
				// printf("%p %s\n", renderModel, typeid(*(cdc::RenderMesh*)renderModel).name());
				// printf("%p\n", renderModel->getMesh());

				// printf("%f %f %f\n", instanceMatrix.m[3][0], instanceMatrix.m[3][1], instanceMatrix.m[3][2]);

				if (renderModel && extraData) {
					// patch all materials (even though this render model is shared between instances)
					for (uint32_t i = 0; i < renderModel->numPrimGroups; i++)
						renderModel->tab0Ext128Byte[i].material = static_cast<cdc::PCDX11Material*>(extraData->material);
				}

				selectedRmiDrawable = new RMIDrawableBase(renderModel);
				auto *rmi = static_cast<cdc::CommonRenderModelInstance*>(selectedRmiDrawable->rmi);
				if (extraData) {
					rmi->baseMask = 0x2000; // deferred lighting
					hackCalcInstanceParams(extraData, &instanceMatrix, rmi->ext->instanceParams);
				} else {
					rmi->baseMask = 0x100A; // normals, composite, translucent. this is further narrowed down by CommonMaterial::SetRenderPasses
				}
				recycleRMI.emplace_back(selectedRmiDrawable);
			}

			bool visible = true;
			if (useFrustumCulling) {
				cdc::BasicCullingVolume cullingVolume;
				selectedRmiDrawable->GetBoundingVolume(&cullingVolume);
				cullingVolume.Transform(instanceMatrix);
				visible = cdc::Intersects(cullingVolume, cullingFrustum);
			}

			if (visible)
				selectedRmiDrawable->draw(&instanceMatrix, 0.0f);
		};

		// all the other objects
		for (auto& unit : StreamTracker) {
			if (!unit.used)
				continue;
			cdc::Level *level = unit.level;
			if (!level)
				continue;

			uint32_t numIntros = level->admdData->numObjects;
			dtp::Intro *intros = level->admdData->objects;
			uint32_t numIMFRefs = level->admdData->numIMFRefs;
			dtp::IMFRef *imfrefs = level->admdData->imfrefs;

			for (uint32_t i=unit.introShowRange[0]; i<numIntros && i<unit.introShowRange[1]; i++) {
				dtp::Intro &intro = intros[i];
				float s = 1.f;
				cdc::Matrix instanceMatrix = {
					s*intro.scale[0], 0, 0, 0,
					0, s*intro.scale[1], 0, 0,
					0, 0, s*intro.scale[2], 0,
					intro.position[0], intro.position[1], intro.position[2], 1
				};
				cdc::Matrix rotationMatrix; rotationMatrix.Build_XYZOrder(intro.rotation);
				instanceMatrix = instanceMatrix * rotationMatrix;
				if (intro.objectListIndex == 0)
					continue;
				cdc::Object *object = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(intro.objectListIndex));
				if (!object)
					continue;
				if (object->numModels == 0)
					continue;

				struct ObjProp {
					uint16_t version;
					uint16_t family;
					uint16_t id;
					uint16_t type;
				};
				auto *objProp = (ObjProp*) object->data;
				uint32_t objFamily = 0;
				if (objProp && objProp->id == 0xb00b)
					objFamily = objProp->family;
				DeferredRenderingExtraData *extraData = nullptr;
				if (objFamily == 0x50)
					extraData = (DeferredRenderingExtraData*) intro.field44_extraData1;

				auto *renderModel = (cdc::PCDX11RenderModel*)object->models[0]->renderMesh;
				putObject(renderModel, instanceMatrix, extraData);
			}

			for (uint32_t i=unit.imfShowRange[0]; i<numIMFRefs && i<unit.imfShowRange[1]; i++) {
				dtp::IMFRef &ref = imfrefs[i];
				if (ref.m_imfDRMName && ref.m_pResolveObject == nullptr) {
					char path[256];
					cdc::GameShell::LOAD_IMFFileName(path, ref.m_imfDRMName);
					ref.m_pResolveObject = cdc::ResolveObject::create(
						path,
						nullptr, nullptr, nullptr,
						nullptr,
						nullptr,
						nullptr,
						0,
						cdc::FileRequest::NORMAL);
					cdc::archiveFileSystem_default->processAll();
				}
				if (isLoaded(ref.m_pResolveObject) || ref.m_imfDRMName == nullptr) {
					//printf("%d %04x %s ", i, ref.dtpID, ref.m_imfDRMName);
					dtp::IntermediateMesh *im = cdc::GetIMFPointerFromId(ref.dtpID);
					if (im) {
						//printf("%p ", im);
						cdc::RenderMesh *model = im->pRenderModel;
						//printf("%p\n", model);
						putObject(static_cast<cdc::PCDX11RenderModel*>(model), ref.m_transform, nullptr);
					}
				}
			}
		}

		// single bottle at origin
		// rmiDrawable.draw(&bottleWorldMatrix, 0.0f);
		sceneCube->setMatrix(bottleWorldMatrix);
		g_scene->RenderWithoutCellTracing(renderViewport);

		auto putTerrain = [&](cdc::IRenderTerrain *renderTerrain, cdc::Matrix& instanceMatrix) {

			auto rtiIt = renderTerrainInstances.find(renderTerrain);
			if (rtiIt == renderTerrainInstances.end())
				return;
			cdc::CommonRenderTerrainInstance *rti = rtiIt->second;

			bool visible = true;
			if (useFrustumCulling) {
				cdc::IRenderTerrain::Node *nodes = renderTerrain->GetNodes();
				cdc::Vector3 center{nodes->center[0], nodes->center[1], nodes->center[2]};
				cdc::Vector3 extents{nodes->extents[0], nodes->extents[1], nodes->extents[2]};

				cdc::CullingBox box;
				box.SetFromCenterAndExtents(center, extents);

				cdc::BasicCullingVolume cullingVolume;
				cullingVolume.m_data.box = box;
				cullingVolume.m_type = cdc::kVolumeBox;

				cullingVolume.Transform(instanceMatrix);
				visible = cdc::Intersects(cullingVolume, cullingFrustum);
			}

			if (visible)
				static_cast<cdc::PCDX11RenderTerrain*>(renderTerrain)->hackDraw(rti, &instanceMatrix);
		};

		// draw cells
		for (auto& unit : StreamTracker) {
			if (!unit.used)
				continue;
			cdc::Level *level = unit.level;
			if (!level)
				continue;

			cdc::CellGroupData *cellGroupData = level->sub50;
			uint32_t numCells = cellGroupData->header->numTotalCells;
			for (uint32_t i=0; i < numCells; i++) {
				cdc::CellData *cell = cellGroupData->cells[i];

				// draw streamgroup meshes
				if (cell->pHeader) {
					cdc::CellStreamGroupData *streamgroup = cell->pHeader->streamGroup50;
					if (streamgroup && streamgroup->resolveObject && isLoaded(streamgroup->resolveObject)) {
						dtp::IntermediateMesh *im = cdc::GetIMFPointerFromId(cell->pHeader->streamGroupDtp54);
						if (im && im->m_type == cdc::IMFType_TerrainInstance) {
							auto *rt = (cdc::IRenderTerrain *)im->pRenderModel;
							putTerrain(rt, cdc::identity4x4);
						}
					}
				}

				// draw renderterrain
				if (cell->pTerrainData) {
					cdc::IRenderTerrain *rt = cell->pTerrainData->pTerrain;
					putTerrain(rt, cdc::identity4x4);
				}

				// draw rendermesh (these are for occlusion culling only, presumably)
				if (cell->renderMesh && drawCellBoxes) {
					auto *cellRMIDrawable = new RMIDrawableBase(cell->renderMesh);
					recycleRMI.emplace_back(cellRMIDrawable);
					static_cast<cdc::PCDX11RenderModelInstance*>(cellRMIDrawable->rmi)->baseMask = 0x1002; // normals & composite
					cellRMIDrawable->draw(&cdc::identity4x4, 0.0f);
				}
			}
		}

		static_cast<cdc::PCDX11Scene*>(scene)->debugShowTempBuffer = showTempBuffer;

		if (applyFXAA) {
			if (pointlessCopy) {
				tempRenderTarget = static_cast<cdc::PCDX11RenderTarget*>(renderDevice->dx11_createRenderTarget(
					100, 100, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, 0x18, cdc::kTextureClass2D));
				tempRenderTarget->getRenderTexture11()->createRenderTargetView();

				auto *fxaaDrawable = new (renderDevice, 0) cdc::PCDX11FXAADrawable(
					renderDevice,
					/*quality*/ 0,
					/*texture*/ renderContext->renderTarget2C->getRenderTexture(),
					/*renderTarget*/ tempRenderTarget,
					/*flags*/ 0,
					/*sortZ*/ 0.0f);

				renderDevice->recordDrawable(fxaaDrawable, /*mask=*/ 0x100, 0);
				renderDevice->finishScene();
				
				// restart the scene to cause a (pointless) copy
				renderDevice->createSubScene(
					&renderViewport,
					renderContext->renderTarget2C,
					renderContext->depthBuffer,
					tempRenderTarget,
					nullptr);
			} else {
				renderDevice->finishScene();
				tempRenderTarget->getRenderTexture11()->createRenderTargetView();

				auto *fxaaDrawable = new (renderDevice, 0) cdc::PCDX11FXAADrawable(
					renderDevice,
					/*quality*/ 0,
					/*texture*/ tempRenderTarget->getRenderTexture(),
					/*renderTarget*/ renderContext->renderTarget2C,
					/*flags*/ 0,
					/*sortZ*/ 0.0f);
				
				// now draw to the real buffer
				renderDevice->createSubScene(
					&renderViewport,
					renderContext->renderTarget2C,
					renderContext->depthBuffer,
					nullptr,
					nullptr);
				renderDevice->recordDrawable(fxaaDrawable, /*mask=*/ 0x100, 0);
			}
		} else if (showTempBuffer != -1) {
			// restart the scene so that the showTempBuffer feature doesn't draw over the UI
			renderDevice->finishScene();
			renderDevice->createSubScene(
				&renderViewport,
				renderContext->renderTarget2C,
				renderContext->depthBuffer,
				nullptr,
				nullptr);
		}

		renderDevice->recordDrawable(&imGuiDrawable, /*mask=*/ 0x100, /*addToParent=*/ 0);
		renderDevice->finishScene();

		renderDevice->endRenderList();

#if ENABLE_IMGUI
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Windows")) {
				if (!loadedSarifHQ && ImGui::MenuItem("Load det_sarifhq_rail_tutorial")) {
					loadedSarifHQ = true;
					Gameloop::InitiateLevelLoad("det_sarifhq_rail_tutorial", nullptr);
					cdc::getDefaultFileSystem()->processAll();
				}
				if (ImGui::MenuItem("Show drawables")) { showDrawablesWindow = true; }
				// if (ImGui::MenuItem("Show filesystem")) { showFilesystemWindow = true; }
				if (ImGui::MenuItem("Show objects")) { showObjectsWindow = true; }
				if (ImGui::MenuItem("Show DRMs")) { showDRMWindow = true; }
				if (ImGui::MenuItem("Show units")) { showUnitsWindow = true; }
				if (ImGui::MenuItem("Show loaded units")) { showLoadedUnitsWindow = true; }
				if (ImGui::MenuItem("Show strings")) { showStringsWindow = true; }
				if (ImGui::MenuItem("I never asked for this")) { howDoYouHandleAllOfThis(); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Language")) {
				language_t vo = localstr_get_voice_language();
				if (ImGui::MenuItem("English", nullptr, vo == language_english))  { localstr_set_language(language_english,   language_default); }
				if (ImGui::MenuItem("French",  nullptr, vo == language_french))   { localstr_set_language(language_french,    language_default); }
				if (ImGui::MenuItem("Italian", nullptr, vo == language_italian))  { localstr_set_language(language_italian,   language_default); }
				if (ImGui::MenuItem("German",  nullptr, vo == language_german))   { localstr_set_language(language_german,    language_default); }
				if (ImGui::MenuItem("Spanish", nullptr, vo == language_spanish))  { localstr_set_language(language_spanish,   language_default); }
				if (ImGui::MenuItem("Polish",  nullptr, vo == language_polish))   { localstr_set_language(language_polish,    language_default); }
				if (ImGui::MenuItem("Russian", nullptr, vo == language_russian))  { localstr_set_language(language_russian,   language_default); }

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Rendering")) {
				if (ImGui::MenuItem("Frustum Culling", nullptr, useFrustumCulling)) { useFrustumCulling = !useFrustumCulling; }
				if (ImGui::MenuItem("Cell Boxes", nullptr, drawCellBoxes)) { drawCellBoxes = !drawCellBoxes; }
				if (ImGui::MenuItem("FXAA", nullptr, applyFXAA)) { applyFXAA = !applyFXAA; }
				ImGui::Separator();
				if (ImGui::MenuItem("Normal Buffer", nullptr, showTempBuffer == 11)) { showTempBuffer = 11; }
				if (ImGui::MenuItem("Light Buffer", nullptr, showTempBuffer == 12)) { showTempBuffer = 12; }
				if (ImGui::MenuItem("Final Buffer", nullptr, showTempBuffer == -1)) { showTempBuffer = -1; }
				ImGui::EndMenu();
			}
			if (mouseLook)
				ImGui::Text("Press TAB or ESC to release cursor");
			else
				ImGui::Text("Press TAB to grab cursor");
			ImGui::EndMainMenuBar();
		}

		if (subtitleIndex) {
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;
			const float PAD = 30.0f;
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = viewport->WorkSize;
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = work_pos.x + work_size.x*0.5f;
			window_pos.y = work_pos.y + work_size.y - PAD;
			window_pos_pivot.x = 0.5f;
			window_pos_pivot.y = 1.0f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
			bool open = true;
			if (ImGui::Begin("subtitlebox", &open, window_flags)) {
				ImGui::Text("%s", localstr_get(subtitleIndex));
			}
			ImGui::End();
		}

		imGuiDrawable.lastMinuteAdditions = [&]() {
			if (showDrawablesWindow) {
				cdc::CommonScene *xscene = captures[selectedCapture].second;
				if (!xscene)
					xscene = scene;
				ImGui::Begin("Scene drawables", &showDrawablesWindow);
				ImGui::BeginChild("capture list", ImVec2(0, 150), true);
				for (uint32_t i = 0; i < captures.size(); i++) {
					ImGui::PushID(i);
					const char *name = i ? "capture" : "live";
					if (ImGui::Selectable(name, i == selectedCapture)) {
						selectedCapture = i;
						renderDevice->revisitRenderLists(captures[i].first);
					}
					ImGui::PopID();
				}
				ImGui::EndChild();

				ImGui::Text("Note: 'Capture frame' is in main menu for now");
				// doesn't work within lastMinuteAdditions
				// if (ImGui::Button("Capture frame")) {
				// 	selectedCapture = captures.size();
				// 	captures.push_back({renderDevice->captureRenderLists(), scene});
				// }

				// buildUI(xscene->drawableListsAndMasks);
				buildUI(&renderDevice->renderPasses, xscene->drawableListsAndMasks);
				buildUI(xscene);
				ImGui::End();
			}
		};
		if (showFilesystemWindow) {
			// TODO
		}
		if (showObjectsWindow) {
			ImGui::Begin("Objects", &showObjectsWindow);
			cdc::buildObjectsUI();
			ImGui::End();
		}
		if (showDRMWindow) {
			drmexplorer.draw(&showDRMWindow);
		}
		if (showUnitsWindow) {
			ImGui::Begin("Units", &showUnitsWindow);
			buildUnitsUI();
			ImGui::End();
		}
		if (showLoadedUnitsWindow) {
			ImGui::Begin("Loaded units", &showLoadedUnitsWindow);
			for (StreamUnit &unit : StreamTracker) {
				if (!unit.used)
					continue;
				ImGui::PushID(unit.name);
				ImGui::Text("%s", unit.name);
				ImGui::DragInt2("visible intros", unit.introShowRange);
				ImGui::DragInt2("visible IMFs", unit.imfShowRange);
				ImGui::PopID();
			}
			bool anyLoaded = false;
			for (auto &unit : StreamTracker) {
				if (!unit.used)
					continue;

				anyLoaded = true;

				cdc::Level *level = unit.level;
				ImGui::Text("unit %s level %p", unit.name, level);
				if (!level) {
					ImGui::Text("not loaded");
					continue;
				}

				ImGui::PushID(unit.name);

				ImGui::PushID("cells");
				cdc::CellGroupData *cellGroupData = level->sub50;
				uint32_t numCells = cellGroupData->header->numTotalCells;
				for (uint32_t i=0; i < numCells; i++) {
					cdc::CellData *cell = cellGroupData->cells[i];
					ImGui::Text("cell %i: %s", i, cell->pHeader->name);
					// RenderTerrain at cell->pTerrainData->pTerrain
					// RenderMesh at cell->renderMesh (for visibility)
					ImGui::SameLine();
					ImGui::PushID(i);
					if (ImGui::SmallButton("Teleport to")) {
						float *mins = cell->pHeader->vec10;
						float *maxs = cell->pHeader->vec20;
						cameraPos.x = (mins[0] + maxs[0]) / 2.0f;
						cameraPos.y = (mins[1] + maxs[1]) / 2.0f;
						cameraPos.z = (mins[2] + maxs[2]) / 2.0f;
					}
					ImGui::PopID();
				}
				ImGui::PopID();

				auto *admd = level->admdData;

				ImGui::PushID("sfxmarkers");
				for (uint32_t i=0; i < admd->m_SfxMarkerCount; i++) {
					dtp::sfxmarker *marker = admd->m_ppSfxMarkers[i];
					ImGui::Text("  [%3d] sfxmarker %f %f %f (%d %s)",
						i, marker->position.x, marker->position.y, marker->position.z,
						marker->numSounds, marker->numSounds==1 ? "sound" : "sounds");
					ImGui::SameLine();
					ImGui::PushID(i);
					if (ImGui::SmallButton("Teleport to")) {
						cameraPos.x = marker->position.x;
						cameraPos.y = marker->position.y;
						cameraPos.z = marker->position.z;
					}
					ImGui::PopID();
					if (false)
						for (uint32_t j=0; j < marker->numSounds; j++) {
							dtp::SoundPlex *plex = marker->soundData[j];
							buildUI(plex, /*indent=*/ "    ");
						}
				}
				ImGui::PopID();

				ImGui::PushID("intros");
				for (uint32_t i=0; i < admd->numObjects; i++) {
					auto &intro = admd->objects[i];
					auto oid = intro.objectListIndex;
					auto name = oid >= cdc::g_objectManager->objectList->count
						? "???": cdc::g_objectManager->objectList->entries[oid].name;
					ImGui::Text("  [%3d] intro %s (%d) %f",
						i, name, oid, intro.scale[0]);
					ImGui::SameLine();
					ImGui::PushID(i);
					if (ImGui::SmallButton("Teleport to")) {
						cameraPos.x = intro.position[0];
						cameraPos.y = intro.position[1];
						cameraPos.z = intro.position[2];
					}
					ImGui::PopID();
				}
				ImGui::PopID();

				ImGui::PopID();
			}
			if (!anyLoaded)
				ImGui::Text("no units");
			ImGui::End();
		}
		if (showStringsWindow) {
			if (ImGui::Begin("Strings", &showStringsWindow)) {
				for (uint32_t i = 0; i < 1000; i++)
					ImGui::Text("%5d %s", i, localstr_get(i));
			}
			ImGui::End();
		}

		// mainMenuInstance.buildUI();
#endif

		renderDevice->drawRenderLists();

		///////////////////////////////////////////////////////////////////////////////////////////

		renderContext->present();

		cdc::GameShell::UpdateLoop(1.0f /*TODO*/);
	}
end:
#if ENABLE_IMGUI 
	ImGui_ImplDX11_Shutdown();
#ifdef _WIN32
	ImGui_ImplWin32_Shutdown();
#else
	ImGui_ImplSDL2_Shutdown();
#endif
	ImGui::DestroyContext();
#endif
	return 0;
}

bool SpinnyCubePass::pre(
	cdc::CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	auto *deviceContext = static_cast<cdc::PCDX11RenderDevice*>(cdc::g_renderDevice)->getD3DDeviceContext();

	deviceContext->RSSetViewports(1, viewport);

	// hack hack, one of the lights gets too close to the camera
	cdc::deviceManager->getStateManager()->m_rasterizerDesc.DepthClipEnable = false;

	cdc::deviceManager->getStateManager()->setDepthState(D3D11_COMPARISON_LESS, true);
	deviceContext->OMSetBlendState(keepAlphaBlend, nullptr, 0xffffffff);

	return true;
}

void SpinnyCubePass::post(
	cdc::CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// empty
}

void ImGuiDrawable::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
#if ENABLE_IMGUI
	if (lastMinuteAdditions)
		lastMinuteAdditions();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
}
