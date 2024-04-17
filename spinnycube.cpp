#include <algorithm>
#include <cmath>
#include <cstdio>
#include <functional>
#include <iterator>
#include <memory>
#include <variant>
#include "config.h" // for ENABLE_IMGUI and ENABLE_D3DCOMPILER

#include <windows.h>
#include <d3d11_1.h>
#if ENABLE_D3DCOMPILER
#include <d3dcompiler.h>
#endif

#include "spinnycube.h"
#include "UIActions.h"
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
#include "game/LensFlareAndCoronaID.h"
#include "game/script/game/NsMainMenuMovieController.h"
#include "game/ui/FakeScaleform/fakescaleform.h"
#include "game/ui/NsPopupRequest.h"
#include "game/ui/Scaleform/ScaleformManager.h"
#include "game/ui/Scaleform/ScaleformMovieInstance.h"
#include "cdcGameShell/cdcGameShell.h" // for LOAD_IMFFileName
#ifdef _WIN32
#include "cdcGameShell/win32/MainVM.h" // for yellowCursor
#endif
#include "input/PCMouseKeyboard.h"
#include "cdcKit/Animation/anitracker.h"
#include "cdcLocale/localstr.h"
#include "cdcMath/Math.h" // for cdc::Matrix
#include "cdcObjects/ObjectManager.h" // for buildObjectsUI
#include "postprocessing/PPManager.h"
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
#include "cdcScript/Decompiler.h"
#include "cdcScript/ScriptType.h"
#include "cdcScene/IMFTypes.h"
#include "cdcScene/IScene.h"
#include "cdcScene/SceneCellGroup.h" // for SceneCellGroup to ISceneCellGroup cast
#include "cdcScene/SceneEntity.h"
#include "cdcSound/Microphone.h"
#include "cdcSound/MultiplexStream.h"
#include "cdcSound/sfxmarker.h"
#include "cdcSound/SoundPlex.h"
#include "cdcWorld/Inspector.h"
#include "cdcWorld/Instance.h"
#include "cdcWorld/InstncG2.h"
#include "cdcWorld/InstanceDrawable.h"
#include "cdcWorld/InstanceManager.h"
#include "cdcWorld/Object.h"
#include "cdcWorld/RMIDrawableBase.h"
#include "cdcWorld/stream.h" // for buildUnitsUI
#include "cdcWorld/StreamUnit.h"
#include "cdcWorld/SceneLayer.h" // for g_scene

#if ENABLE_IMGUI
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx11.h"
#ifdef _WIN32
#include "imgui/backends/imgui_impl_win32.h"
extern HCURSOR ImGui_ImplWin32_Arrow;
#else
#include "imgui/backends/imgui_impl_sdl.h"
#endif
#include "rendering/Inspector.h"
#include "ImGuizmo.h"
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

cdc::Vector halton[512];

void InitHalton(unsigned component, unsigned b) {
	unsigned n = 0, d = 1;
	for (unsigned i=0; i<512; i++) {
		unsigned x = d - n;
		if (x == 1) {
			n = 1;
			d *= b;
		} else {
			unsigned y = d / b;
			while (x <= y)
				y /= b;
			n = (b + 1) * y - x;
		}
		halton[i].vec128[component] = float(n) / d;
	}
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

	void draw(UIActions& uiact, bool *showWindow) {
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
									buildUI(plex, nullptr, /*indent=*/ "    ");
								}
								ImGui::PopID();

							}
							if (section.type == 8) { // Script
								if (auto *ty = (cdc::ScriptType*)cdc::g_resolveSections[8]->getWrapped(section.id)) {
									ImGui::SameLine();
									ImGui::Text(" %s", ty->blob->m_nativeScriptName);
									ImGui::SameLine();
									ImGui::PushID(section.id);
									if (ImGui::SmallButton("Decompile")) {
										uiact.select(ty);
									}
									ImGui::PopID();
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

struct SpinnyUIActions : public UIActions {
	cdc::IRenderTerrain *selectedRenderTerrain = 0;
	cdc::RenderMesh *selectedModel = nullptr;
	cdc::ModelBatch *selectedBatch = nullptr;
	cdc::VertexDecl *selectedVertexDecl = nullptr;
	cdc::IMaterial *selectedMaterial = nullptr;
	cdc::MaterialBlobSub *selectedSubMaterial = nullptr;
	cdc::ScriptType *selectedScriptType = nullptr;
	dtp::Intro *selectedIntro = nullptr;
	dtp::IMFRef *selectedIMFRef = nullptr;
	Instance *selectedInstance = nullptr;

	void *selectedMovable = nullptr;

	void select(cdc::IRenderTerrain *renderTerrain) override {
		selectedModel = nullptr;
		selectedRenderTerrain = renderTerrain;
	}
	void select(cdc::RenderMesh *model) override {
		selectedModel = model;
		selectedRenderTerrain = nullptr;
	}
	void select(cdc::ModelBatch *batch) override {
		selectedBatch = batch;
	}
	void select(cdc::VertexDecl *vertexDecl) override {
		selectedVertexDecl = vertexDecl;
	}
	void select(cdc::IMaterial *material) override {
		selectedMaterial = material;
	}
	void select(cdc::MaterialBlobSub *subMaterial) override {
		selectedSubMaterial = subMaterial;
	}
	void select(cdc::ScriptType *scriptType) override {
		selectedScriptType = scriptType;
	}
	void select(dtp::Intro *intro) override {
		selectedIntro = intro;
	}
	void select(dtp::IMFRef *imfRef) override {
		selectedIMFRef = imfRef;
		selectedMovable = (void*)imfRef;
	}
	void select(Instance *instance) override {
		selectedInstance = instance;
		selectedMovable = (void*)instance;
	}
};

#endif

int spinnyCube(HWND window,
	ID3D11Device *baseDevice,
	ID3D11DeviceContext *baseDeviceContext) {

	localstr_set_language(language_english, language_default);
	registerPopupHandler();

	InitHalton(0, 2);
	InitHalton(1, 3);
	InitHalton(2, 5);

	for (unsigned i=0; i<32; i++)
		printf("halton[%d] = %f %f %f\n", i, halton[i].x, halton[i].y, halton[i].z);

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
	bool drawStreamGroups = true;
	bool drawCellMeshes = true;
	bool drawCellBoxes = false;
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

	auto *dc = cdc::deviceManager->getDisplayConfig();

#if ENABLE_IMGUI
	bool loadedSarifHQ = false;
	bool showDrawablesWindow = false;
	bool showFilesystemWindow = false;
	bool showObjectsWindow = false;
	SpinnyUIActions uiact;
	bool showDRMWindow = false;
	bool showUnitsWindow = false;
	bool showLoadedUnitsWindow = false;
	bool showStringsWindow = false;
	bool showScaleformStringsWindow = false;
	bool showAnimationsWindow = false;
	bool showIntroButtons = true;
	bool showIntroButtonsIMF = true;
	bool editorMode = false;
	std::vector<std::pair<void*, cdc::CommonScene*>> captures { { nullptr, nullptr } };
	uint32_t selectedCapture = 0;

#ifdef _WIN32
	ImGui_ImplWin32_Arrow = (HCURSOR)yellowCursor;
#endif

	cdc::ScriptType *mainMenuScriptType =
		(cdc::ScriptType*)cdc::g_resolveSections[8]->getWrapped(0x154a7); // pc-w/globaloutershell.drm section 0xb7
	ScaleformMovieInstance mainMenuInstance(&mainMenuMovie);
	NsMainMenuMovieController mainMenuMovieController(mainMenuScriptType);
	cdc::GCPtr<NsMainMenuMovieController> keepControllerAlive(&mainMenuMovieController); // garbage collected else

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
		ImGuizmo::BeginFrame();

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
			ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
		} else {
			ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
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
			if (!cellGroupData)
				continue;

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

		cdc::G2Instance_BuildAllTransforms(); // this will evaluate animations
		SceneLayer::Update(); // this will create SceneEntities for Instances that don't have any yet

		cdc::PCDX11RenderTarget *tempRenderTarget = nullptr;

		if (dc->antiAliasing > 0 && !pointlessCopy)
			tempRenderTarget = static_cast<cdc::PCDX11RenderTarget*>(renderDevice->dx11_createRenderTarget(
				100, 100, DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, 0x18, cdc::kTextureClass2D));

		renderDevice->resetRenderLists();
		renderDevice->beginRenderList(nullptr);
		auto *scene = renderDevice->createSubScene( // CommonScene::CommonScene creates the projectMatrix
			&renderViewport,
			tempRenderTarget ? tempRenderTarget : renderContext->renderTarget2C,
			renderContext->depthBuffer,
			nullptr,
			nullptr);
		scene->viewMatrix = viewMatrix;
		cdc::OrthonormalInverse3x4(&renderViewport.viewMatrix, viewMatrix);

		cdc::g_microphone.m_viewMatrix = viewMatrix;
		cdc::g_microphone.m_position = cameraPos;
		cdc::SFXMARKER_ProcessAllMarkers();

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

		auto putObject = [&](cdc::PCDX11RenderModel *renderModel, cdc::Matrix& instanceMatrix, uint32_t objFamily, void *extraData) {
			RMIDrawableBase *selectedRmiDrawable = &rmiDrawable;
			if (renderModel) {
				// printf("%p %s\n", renderModel, typeid(*(cdc::RenderMesh*)renderModel).name());
				// printf("%p\n", renderModel->getMesh());

				// printf("%f %f %f\n", instanceMatrix.m[3][0], instanceMatrix.m[3][1], instanceMatrix.m[3][2]);

				if (renderModel) {
					if (objFamily == 0x50) {
						auto *deferredExtraData = (DeferredRenderingExtraData*)extraData;
						// patch all materials (even though this render model is shared between instances)
						for (uint32_t i = 0; i < renderModel->numPrimGroups; i++)
							renderModel->tab0Ext128Byte[i].material = static_cast<cdc::PCDX11Material*>(deferredExtraData->material);

					} else if (objFamily == 0x5b) {
						auto *lensFlareExtraData = (LensFlareAndCoronaExtraData*)extraData;
						// patch all materials (even though this render model is shared between instances)
						if (lensFlareExtraData->material)
							for (uint32_t i = 0; i < renderModel->numPrimGroups; i++)
								renderModel->tab0Ext128Byte[i].material = static_cast<cdc::PCDX11Material*>(lensFlareExtraData->material);
					}
				}

				selectedRmiDrawable = new RMIDrawableBase(renderModel);
				auto *rmi = static_cast<cdc::CommonRenderModelInstance*>(selectedRmiDrawable->rmi);
				if (objFamily == 0x50) {
					rmi->baseMask = 0x2000; // deferred lighting
					auto *deferredExtraData = (DeferredRenderingExtraData*)extraData;
					hackCalcInstanceParams(deferredExtraData, &instanceMatrix, rmi->ext->instanceParams);

					// patch textures (even though this render model is shared between instances)
					cdc::PersistentPGData *ppg = rmi->getPersistentPGData();
					if (deferredExtraData->texture[0]) ppg->sub10.pInstanceTextures[0] = deferredExtraData->texture[0];
					if (deferredExtraData->texture[1]) ppg->sub10.pInstanceTextures[1] = deferredExtraData->texture[1];
					if (deferredExtraData->texture[2]) ppg->sub10.pInstanceTextures[2] = deferredExtraData->texture[2];
					if (deferredExtraData->texture[3]) ppg->sub10.pInstanceTextures[3] = deferredExtraData->texture[3];

				} else if (objFamily == 0x5b) {
					rmi->baseMask = 0x100A; // normals, composite, translucent, for now
					auto *lensFlareExtraData = (LensFlareAndCoronaExtraData*)extraData;
					hackCalcInstanceParams(lensFlareExtraData, &instanceMatrix, /*invView*/ &renderViewport.viewMatrix, rmi->ext->instanceParams);

					// patch textures (even though this render model is shared between instances)
					cdc::PersistentPGData *ppg = rmi->getPersistentPGData();
					if (lensFlareExtraData->texture[0]) ppg->sub10.pInstanceTextures[0] = lensFlareExtraData->texture[0];
					if (lensFlareExtraData->texture[1]) ppg->sub10.pInstanceTextures[1] = lensFlareExtraData->texture[1];
					if (lensFlareExtraData->texture[2]) ppg->sub10.pInstanceTextures[2] = lensFlareExtraData->texture[2];
					if (lensFlareExtraData->texture[3]) ppg->sub10.pInstanceTextures[3] = lensFlareExtraData->texture[3];

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

		struct ButtonItem {
			dtp::Intro *intro = nullptr;
			cdc::IRenderTerrain *renderTerrain = nullptr;
			Instance *instance = nullptr;
			dtp::IMFRef *imfRef = nullptr;
		};

		struct FloatingButton {
			cdc::Vector4 pos;
			std::string label;
			ButtonItem item;
		};

		std::vector<FloatingButton> fbs {
			// {{modelTranslation.x, modelTranslation.y, modelTranslation.z, 1}, "alc_beer_bottle_a", {}},
			// {{0, 0, 0, 1}, "origin", {}}
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

			for (uint32_t i=unit.introShowRange[0]; i<numIntros && i<unit.introShowRange[1] && editorMode; i++) {
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

				const char *name = cdc::objectName(intro.objectListIndex); // TODO: name = object->name;
				if (name)
					fbs.push_back(FloatingButton{
						{intro.position[0], intro.position[1], intro.position[2], 1},
						name,
						{.intro=&intro}
					});

				if (object->numModels == 0)
					continue;
				if (cdc::objects[object->trackerID].debugHide)
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

				auto *renderModel = (cdc::PCDX11RenderModel*)object->models[0]->renderMesh;
				putObject(renderModel, instanceMatrix, objFamily, intro.extraData1);
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
				if (showIntroButtonsIMF && ref.m_imfDRMName && strrchr(ref.m_imfDRMName, '\\'))
					fbs.push_back(FloatingButton{
						{ref.m_transform.m[3][0],
						 ref.m_transform.m[3][1],
						 ref.m_transform.m[3][2],
						 1},
						strrchr(ref.m_imfDRMName, '\\')+1,
						{.imfRef=&ref}
					});
				if (isLoaded(ref.m_pResolveObject) || ref.m_imfDRMName == nullptr) {
					//printf("%d %04x %s ", i, ref.dtpID, ref.m_imfDRMName);
					dtp::IntermediateMesh *im = cdc::GetIMFPointerFromId(ref.dtpID);
					if (im) {
						//printf("%p ", im);
						cdc::RenderMesh *model = im->pRenderModel;
						//printf("%p\n", model);
						putObject(static_cast<cdc::PCDX11RenderModel*>(model), ref.m_transform, 0, nullptr);
					}
				}
			}
		}


		if (!editorMode) {
			for (Instance *instance : InstanceManager::s_instances) {
				const char *name = instance->object ? cdc::objectName(instance->object->dword14) : "no object";
				fbs.push_back({
					{instance->position.x, instance->position.y, instance->position.z, 1},
					name,
					nullptr,
					nullptr,
					instance
				});
			}
		}

		// single bottle at origin
		// rmiDrawable.draw(&bottleWorldMatrix, 0.0f);
		sceneCube->setMatrix(bottleWorldMatrix);
		if (!editorMode)
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

			if (visible) {
				cdc::IRenderTerrain::Node *nodes = renderTerrain->GetNodes();
				fbs.push_back(FloatingButton{
						{nodes->center[0], nodes->center[1], nodes->center[2], 1},
						"[render terrain]",
						{.renderTerrain=renderTerrain}
					});
				static_cast<cdc::PCDX11RenderTerrain*>(renderTerrain)->hackDraw(rti, &instanceMatrix);
			}
		};

		// draw cells
		for (auto& unit : StreamTracker) {
			if (!unit.used)
				continue;

			cdc::Level *level = unit.level;
			if (!level)
				continue;

			cdc::CellGroupData *cellGroupData = level->sub50;
			if (!cellGroupData)
				continue;

			uint32_t numCells = cellGroupData->header->numTotalCells;
			for (uint32_t i=0; i < numCells; i++) {
				cdc::CellData *cell = cellGroupData->cells[i];

				// draw streamgroup meshes
				if (cell->pHeader && drawStreamGroups) {
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
				if (cell->pTerrainData && drawCellMeshes) {
					cdc::IRenderTerrain *rt = cell->pTerrainData->pTerrain;
					putTerrain(rt, cdc::identity4x4);
				}

				// draw rendermesh (these are for occlusion culling only, presumably)
				if (cell->renderMesh && drawCellBoxes) {
					auto *cellRMIDrawable = new RMIDrawableBase(cell->renderMesh);
					recycleRMI.emplace_back(cellRMIDrawable);
					auto *rmi = static_cast<cdc::PCDX11RenderModelInstance*>(cellRMIDrawable->rmi);
					rmi->baseMask = 0x1002; // normals & composite
					rmi->ext->instanceParams[0] = { // assign a distinguishable color for this cell
						halton[i].x,
						halton[i].y,
						halton[i].z,
						1.0f
					};
					cellRMIDrawable->draw(&cdc::identity4x4, 0.0f);
				}
			}
		}

		static_cast<cdc::PCDX11Scene*>(scene)->debugShowTempBuffer = showTempBuffer;

		if (dc->antiAliasing > 0) {
			PPManager::s_instance->createScene(
				scene->renderTarget, // renderDevice->getSceneRenderTarget(),
				nullptr, // particle RT
				renderContext->renderTarget2C,
				scene->depthBuffer, // renderDevice->getSceneDepthBuffer(),
				&renderViewport
			);

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
				if (ImGui::MenuItem("Show scaleform strings")) { showScaleformStringsWindow = true; }
				if (ImGui::MenuItem("Show animations")) { showAnimationsWindow = true; }
				if (ImGui::MenuItem("I never asked for this")) { howDoYouHandleAllOfThis(); }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Editor")) {
				// editorMode off: show cdc::Scene, render Instances (doesn't support lights/culling currently)
				// editorMode on:  hide cdc::Scene, render cdc::Intros directly
				if (ImGui::MenuItem("Editor mode", nullptr, editorMode)) { editorMode = !editorMode; }
				if (ImGui::MenuItem("Intro Buttons", nullptr, showIntroButtons)) { showIntroButtons = !showIntroButtons; }
				if (ImGui::MenuItem("Intro Buttons IMF", nullptr, showIntroButtonsIMF)) { showIntroButtonsIMF = !showIntroButtonsIMF; }
				if (ImGui::MenuItem("Disable SFX", nullptr, cdc::neverInsideSfxPerimeter)) { cdc::neverInsideSfxPerimeter = !cdc::neverInsideSfxPerimeter; }
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
				if (ImGui::MenuItem("Capture frame")) {
					selectedCapture = captures.size();
					captures.push_back({renderDevice->captureRenderLists(), scene});
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Frustum Culling", nullptr, useFrustumCulling)) { useFrustumCulling = !useFrustumCulling; }

				if (ImGui::MenuItem("Streamgroups ", nullptr, drawStreamGroups)) { drawStreamGroups = !drawStreamGroups; }
				if (ImGui::MenuItem("Cell Meshes", nullptr, drawCellMeshes)) { drawCellMeshes = !drawCellMeshes; }
				if (ImGui::MenuItem("Cell Boxes", nullptr, drawCellBoxes)) { drawCellBoxes = !drawCellBoxes; }
				ImGui::Separator();
				if (ImGui::MenuItem("Off",         nullptr, dc->antiAliasing == 0)) { dc->antiAliasing = 0; }
				if (ImGui::MenuItem("FXAA Low",    nullptr, dc->antiAliasing == 2)) { dc->antiAliasing = 2; }
				if (ImGui::MenuItem("FXAA Medium", nullptr, dc->antiAliasing == 3)) { dc->antiAliasing = 3; }
				if (ImGui::MenuItem("FXAA High",   nullptr, dc->antiAliasing == 4)) { dc->antiAliasing = 4; }
				if (ImGui::MenuItem("MLAA",        nullptr, dc->antiAliasing == 5)) { dc->antiAliasing = 5; }
				ImGui::Separator();
				if (ImGui::MenuItem("Normal Buffer", nullptr, showTempBuffer == 11)) { showTempBuffer = 11; }
				if (ImGui::MenuItem("Light Buffer", nullptr, showTempBuffer == 12)) { showTempBuffer = 12; }
				if (ImGui::MenuItem("Final Buffer", nullptr, showTempBuffer == -1)) { showTempBuffer = -1; }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Popups")) {
				buildPopupsMenu();
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

		if (!mouseLook && showIntroButtons) {

			std::vector<FloatingButton> fbs2;

			for (auto& fb : fbs) {
				cdc::Vector4 viewPos = viewMatrix * fb.pos;
				cdc::Vector4 projPos = scene->projectMatrix * viewPos;
				projPos.x /= projPos.w;
				projPos.y /= projPos.w;
				projPos.z /= projPos.w;

				if (viewPos.z >= 0 && viewPos.z < 5000.0f)
					fbs2.push_back({projPos, (std::string&&) fb.label, fb.item});
			}

			std::sort(fbs2.begin(), fbs2.end(), [](FloatingButton const &a, FloatingButton const &b) {
				return a.pos.z > b.pos.z;
			});

			uint32_t i=0;
			for (auto& fb : fbs2) {
				ImGuiWindowFlags window_flags =
					ImGuiWindowFlags_NoBackground |
					ImGuiWindowFlags_NoDecoration |
					ImGuiWindowFlags_AlwaysAutoResize |
					ImGuiWindowFlags_NoSavedSettings |
					ImGuiWindowFlags_NoFocusOnAppearing |
					ImGuiWindowFlags_NoNav |
					ImGuiWindowFlags_NoMove;
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImVec2 window_pos = {
					0.5f * viewport->Size.x * ( fb.pos.x + 1.0f),
					0.5f * viewport->Size.y * (-fb.pos.y + 1.0f)
				};
				ImVec2 window_pos_pivot = {0.5f, 0.5f};
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
				bool open=true;
				char name[128];
				// this ensures that objects at the same position are packed into the same window
				snprintf(name, 128, "fb%f,%f,%f", fb.pos.x, fb.pos.y, fb.pos.z);
				auto& fbit = fb.item;
				if (ImGui::Begin(name, &open, window_flags)) {
					ImGui::PushID((void*)(
						uintptr_t(fbit.intro) |
						uintptr_t(fbit.renderTerrain) |
						uintptr_t(fbit.instance) |
						uintptr_t(fbit.imfRef)));
					if (ImGui::Button(fb.label.c_str())) {
						if (fbit.intro)
							uiact.select(fbit.intro);
						else if (fbit.renderTerrain)
							uiact.select(fbit.renderTerrain);
						else if (fbit.instance)
							uiact.select(fbit.instance);
						else if (fbit.imfRef)
							uiact.select(fbit.imfRef);
					}
					ImGui::PopID();
				}
				ImGui::End();
			}
		}
		fbs.clear();

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
				buildUI(uiact, &renderDevice->renderPasses, xscene->drawableListsAndMasks);
				buildUI(xscene);
				ImGui::End();
			}
		};
		if (showFilesystemWindow) {
			// TODO
		}
		if (showObjectsWindow) {
			ImGui::Begin("Objects", &showObjectsWindow);
			cdc::buildObjectsUI(uiact);
			ImGui::End();
		}
		if (uiact.selectedModel || uiact.selectedRenderTerrain) {
			bool showModelWindow = true;
			ImGui::Begin("Model", &showModelWindow);
			ImGui::PushID("model or terrain");
			if (uiact.selectedModel) {
				auto *model = static_cast<cdc::PCDX11RenderModel*>(uiact.selectedModel);
				auto *nppg = model->getTab0Ext16();
				auto *ppg = model->getTab0Ext128();
				ImGui::Text("# model batches = %d", model->numModelBatches);
				ImGui::Text("# prim groups = %d", model->numPrimGroups);
				uint32_t pg = 0;
				for (uint32_t i = 0; i < model->numModelBatches; i++) {
					cdc::ModelBatch *batch = &model->modelBatches[i];
					ImGui::Text("batch %d", i);
					for (uint32_t j = 0; j < batch->tab0EntryCount_30; j++, pg++) {
						ImGui::PushID(pg);
						ImGui::Text("  group %d", pg);
						cdc::PrimGroup *group = &model->primGroups[pg];
						ImGui::SameLine();
						ImGui::Text(": flags %02x passMask", group->flags); ImGui::SameLine();
						UIPassMask(nppg[pg].mask8); ImGui::SameLine();
						ImGui::Text(" %d tris", group->triangleCount); ImGui::SameLine();
						if (ImGui::SmallButton("vertexdecl/material")) {
							uiact.select(batch);
							uiact.select((cdc::VertexDecl*)batch->format);
							uiact.select(group->material);
						}
						bool& hidden = ppg[pg].hide;
						ImGui::SameLine();
						if (ImGui::SmallButton(hidden ? "show" : "hide")) { hidden = !hidden; }
						ImGui::PopID();
					}
				}
			} else {
				auto *terrain = static_cast<cdc::PCDX11RenderTerrain*>(uiact.selectedRenderTerrain);
				uint32_t numGroups = terrain->m_pResourceData->pHeader->numGroups;
				ImGui::Text("# groups = %d", numGroups);
				for (uint32_t i = 0; i < numGroups; i++) {
					ImGui::PushID(i);
					cdc::RenderTerrainGroup *group = &terrain->m_pGroups[i];
					ImGui::Text("group %d passes %08x", i, group->renderPasses);
					ImGui::SameLine();
					if (ImGui::SmallButton("vertexdecl/material")) {
						cdc::RenderTerrainVertexBuffer *group_vb = &terrain->m_pVertexBuffers[group->vbIndex];
						uiact.select(group_vb->pVertexDecl);
						uiact.select(group->m_pMaterial);
					}
					bool hidden = group->flags & 0x8000; // HACK
					ImGui::SameLine();
					if (ImGui::SmallButton(hidden ? "show" : "hide"))
						group->flags ^= 0x8000;
					ImGui::PopID();
				}
			}
			ImGui::PopID();
			if (uiact.selectedMaterial) {
				ImGui::Separator();
				ImGui::PushID("material");
				// auto *material = static_cast<cdc::PCDX11Material*>(uiact.selectedMaterial);
				auto *material = static_cast<cdc::CommonMaterial*>(uiact.selectedMaterial);
				ImGui::Text("material %p", material);

				ImGui::Text("  mask "); ImGui::SameLine();
				UIPassMask(material->GetRenderPassMask(/*fading=*/false)); ImGui::SameLine();
				ImGui::Text("/"); ImGui::SameLine();
				UIPassMask(material->GetRenderPassMask(/*fading=*/true));

				ImGui::Text("  blendMode"); ImGui::SameLine();
				UIBlendMode(material->GetBlendMode(), material->materialBlob->renderTargetWriteMask); ImGui::SameLine();
				ImGui::Text("rtmask %01x",
					material->materialBlob->renderTargetWriteMask);

				for (uint32_t i = 0; i < 16; i++) {
					ImGui::PushID(i);
					auto *submat = material->GetMaterialData()->subMat4C[i];
					if (submat) {
						ImGui::Text("submaterial %d: %p", i, submat);
						ImGui::SameLine();
						if (ImGui::SmallButton("show"))
							uiact.select(submat);
						ImGui::SameLine();
						if (submat->shaderVertex)
							ImGui::Text(" VS");
						else
							ImGui::TextDisabled(" VS");
						ImGui::SameLine();
						if (submat->shaderPixel)
							ImGui::Text(" PS");
						else
							ImGui::TextDisabled(" PS");
						if (i == 3) { ImGui::SameLine(); ImGui::Text("opaque"); }
						if (i == 7) { ImGui::SameLine(); ImGui::Text("normals"); }
						if (i == 8) { ImGui::SameLine(); ImGui::Text("translucent/deferred light"); }
					}
					ImGui::PopID();
				}
				ImGui::PopID();
			}
			if (uiact.selectedSubMaterial) {
				ImGui::Separator();
				ImGui::PushID("submaterial");
				cdc::MaterialBlobSub *submat = uiact.selectedSubMaterial;
				ImGui::Text("submaterial %p", submat);

				uint8_t psRefIndexEnd = 0;
				psRefIndexEnd = std::max<uint8_t>(psRefIndexEnd, submat->psRefIndexEndA);
				psRefIndexEnd = std::max<uint8_t>(psRefIndexEnd, submat->psRefIndexBeginB);
				psRefIndexEnd = std::max<uint8_t>(psRefIndexEnd, submat->psRefIndexBeginB + submat->psRefIndexCountB);
				cdc::MaterialTexRef *texref = submat->psTextureRef;

				ImGui::Indent();
				for (uint32_t i = 0; i < psRefIndexEnd; i++) {
					bool g0 = i < submat->psRefIndexEndA;
					bool g1 = i >= submat->psRefIndexEndA && i < submat->psRefIndexBeginB;
					bool g2 = i >= submat->psRefIndexBeginB && i < submat->psRefIndexBeginB + submat->psRefIndexCountB;
					ImGui::Text("texref %d: slot %d %s %p %p",
						i,
						texref[i].slotIndex,
						g0 ? "normal" :
						g1 ? "global" :
						g2 ? "instnc" :
						"unused",
						texref[i].tex,
						texref[i].tex ? static_cast<cdc::PCDX11Texture*>(texref[i].tex)->createShaderResourceView() : 0);
					if (g1 || g2) {
						ImGui::SameLine();
						ImGui::Text("%x", texref[i].fallbackIndex);
					}
				}
				ImGui::Unindent();

				ImGui::Text("vs cb4 %d..%d", submat->vsBufferFirstRow, submat->vsBufferFirstRow + submat->vsBufferNumRows);
				ImGui::Text("ps cb4 %d..%d", submat->psBufferFirstRow, submat->psBufferFirstRow + submat->psBufferNumRows);

				static bool weirdFlag = false;
				ImGui::Checkbox("???", &weirdFlag);
				uint32_t forbiddenBit = weirdFlag ? 2 : 4;

				if (ImGui::BeginTable("attribs", 2)) {
					ImGui::TableSetupColumn("VD");
					ImGui::TableSetupColumn("VS");
					ImGui::TableHeadersRow();
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					if (auto* vertexDecl = uiact.selectedVertexDecl) {
						std::vector<D3D11_INPUT_ELEMENT_DESC> elems(vertexDecl->numAttr);
						MakeD3DVertexElements(elems.data(), vertexDecl->attrib, vertexDecl->numAttr, false);
						for (uint32_t i = 0; i < vertexDecl->numAttr; i++)
							ImGui::Text("%08x %s %d",
								vertexDecl->attrib[i].attribKind,
								elems[i].SemanticName,
								elems[i].SemanticIndex);
					} else
						ImGui::Text("no VD");
					ImGui::TableSetColumnIndex(1);
					if (cdc::ShaderInputSpec *inputSpec = submat->vsLayout[0]) { // other indices?
						for (uint32_t i = 0; i < inputSpec->numAttribs; i++) {
							auto& at = inputSpec->attr[i];
							if (at.field_A & forbiddenBit)
								continue;
							ImGui::Text("[%d] %08x", i, at.attribKindA);
							if (at.nextAttribIndex != 0xffffffff) {
								ImGui::SameLine();
								ImGui::Text(" -> [%d]", at.nextAttribIndex);
							}
						}
						// TODO: extract logic from PCDX11StreamDeclCache::buildStreamDecl to
						//       determine which VertexDecl attrib this connects to
					} else
						ImGui::Text("no VS");
					ImGui::EndTable();
				}
				ImGui::PopID();
			}
			ImGui::End();
			if (!showModelWindow)
				uiact.select((cdc::RenderMesh*)nullptr);
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		auto manipulate = [&](cdc::Matrix& m) {
			ImGuizmo::Manipulate(
				(float*)viewMatrix.m,
				(float*)scene->projectMatrix.m,
				ImGuizmo::TRANSLATE,
				ImGuizmo::WORLD,
				(float*)m.m);
		};

		if (uiact.selectedIntro) {
			bool showWindow = true;
			ImGui::Begin("Intro", &showWindow);
			ImGui::Text("%p", uiact.selectedIntro);
			buildUI(uiact, uiact.selectedIntro);
			ImGui::End();
			if (!showWindow)
				uiact.select((dtp::Intro*)nullptr);
		}
		if (uiact.selectedIMFRef) {
			if (uiact.selectedIMFRef == uiact.selectedMovable)
				manipulate(uiact.selectedIMFRef->m_transform);

			bool showWindow = true;
			ImGui::Begin("IMFRef", &showWindow);
			ImGui::Text("%p", uiact.selectedIMFRef);
			ImGui::Text("debug name: %s", uiact.selectedIMFRef->m_debugName);
			ImGui::End();
			if (!showWindow)
				uiact.select((dtp::IMFRef*)nullptr);
		}
		if (uiact.selectedInstance) {
			if (uiact.selectedInstance == uiact.selectedMovable) {
				Instance *instance = uiact.selectedInstance;
				auto& tc = instance->GetTransformComponent();
				cdc::Matrix *matrices = tc.m_matrix;
				dtp::Model *model = uiact.selectedInstance->GetMeshComponent().GetModel();
				uint32_t numMatrices = model ? model->GetNumSegments() : 1;
				if (tc.GetNotAnimated() || numMatrices <= 1)
					manipulate(matrices[0]);
				else {
					// translation gizmo
					cdc::Matrix delta;
					ImGuizmo::SetID(0);
					ImGuizmo::Manipulate(
						(float*)viewMatrix.m,
						(float*)scene->projectMatrix.m,
						ImGuizmo::TRANSLATE,
						ImGuizmo::WORLD,
						(float*)matrices[-1].m,
						(float*)delta.m);
					if (delta != cdc::identity4x4) {
						instance->position.x += delta.m[3][0];
						instance->position.y += delta.m[3][1];
						instance->position.z += delta.m[3][2];
					}

					// rotation gizmos
					for (uint32_t i=0; i<numMatrices; i++) {
						ImGuizmo::SetID(i+1);
						ImGuizmo::Manipulate(
							(float*)viewMatrix.m,
							(float*)scene->projectMatrix.m,
							ImGuizmo::ROTATE,
							ImGuizmo::LOCAL,
							(float*)matrices[i].m,
							(float*)delta.m);

						if (instance->enableOverridePose && delta != cdc::identity4x4) {
							auto j = model->GetSegmentList()[i].parent;
							cdc::Matrix parentInverse;
							cdc::OrthonormalInverse3x4(&parentInverse, matrices[j]);
							instance->overridePose[i] = parentInverse * matrices[i];
						}
					}
				}
				if (auto *instanceDrawable = uiact.selectedInstance->instanceDrawable)
					static_cast<cdc::InstanceDrawable*>(instanceDrawable)->AddToDirtyList();
			}

			bool showWindow = true;
			ImGui::Begin("Instance", &showWindow);
			buildUI(uiact, uiact.selectedInstance);
			ImGui::End();
			if (!showWindow)
				uiact.select((Instance*)nullptr);
		}
		if (uiact.selectedScriptType) {
			bool showWindow = true;
			ImGui::Begin("Script Type", &showWindow);
			Decompile(uiact, *uiact.selectedScriptType);
			ImGui::End();
			if (!showWindow)
				uiact.select((cdc::ScriptType*)nullptr);
		}
		if (showDRMWindow) {
			drmexplorer.draw(uiact, &showDRMWindow);
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

				if (cdc::CellGroupData *cellGroupData = level->sub50) {
					ImGui::PushID("cells");
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
				}

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
					if (true)
						if (marker->soundHandles[0].isSet()) {
							ImGui::SameLine();
							if (ImGui::SmallButton("Stop")) {
								marker->soundHandles[0]->End((cdc::SoundTypes::EndType)0);
							}
						}
						ImGui::Indent();
						for (uint32_t j=0; j < marker->numTriggers; j++) {
							auto& trigger = marker->triggers[j];
							if (trigger.condition == 0)
								ImGui::Text("trigger %d: Init", j);
							else {
								ImGui::Text("trigger %d: %s radius=%f dist=%f breached=%d count=%d",
									j, trigger.condition == 1 ? "Enter" : "Exit",
									trigger.conditionData.radius,
									trigger.conditionData.lastDistance,
									(int)trigger.conditionData.bBreached,
									(int)trigger.conditionData.nCountFired);
							}
						}
						ImGui::Unindent();
						for (uint32_t j=0; j < marker->numSounds; j++) {
							dtp::SoundPlex *plex = marker->soundData[j];
							std::function<void(cdc::SoundHandle)> onPlay = [&](cdc::SoundHandle sh) {
								auto& c3d = sh->controls3d;
								c3d.playbackType = 0x101;
								c3d.position[0] = marker->position.x;
								c3d.position[1] = marker->position.y;
								c3d.position[2] = marker->position.z;
								marker->soundHandles[0] = sh;
							};
							buildUI(plex, &onPlay, /*indent=*/ "    ");
						}
				}
				ImGui::PopID();

				ImGui::PushID("intros");
				for (uint32_t i=0; i < admd->numObjects; i++) {
					auto &intro = admd->objects[i];
					auto oid = intro.objectListIndex;
					auto name =oid >= cdc::g_objectManager->objectList->count
						? "???" : cdc::objectName(intro.objectListIndex);
					ImGui::Text("  [%3d] intro %s (%d) %d/0x%x",
						i, name, oid, intro.uniqueID, intro.uniqueID);
					ImGui::PushID(i);
					ImGui::SameLine();
					if (ImGui::SmallButton("Teleport to")) {
						cameraPos.x = intro.position[0];
						cameraPos.y = intro.position[1];
						cameraPos.z = intro.position[2];
					}
					ImGui::SameLine();
					if (ImGui::SmallButton("Details"))
						uiact.select(&intro);
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
				ImGuiListClipper clipper;
				clipper.Begin(localstr_get_count());
				while (clipper.Step())
					for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++)
						ImGui::Text("%5d %s", i, localstr_get(i));
			}
			ImGui::End();
		}
		if (showScaleformStringsWindow) {
			if (ImGui::Begin("Scaleform strings", &showScaleformStringsWindow)) {
				for (auto [string, i] : scaleformStringMap)
					ImGui::Text("%40s : %s", string.c_str(), localstr_get(i));
			}
			ImGui::End();
		}
		if (showAnimationsWindow) {
			if (ImGui::Begin("Animations", &showAnimationsWindow)) {
				cdc::ANITRACKER_BuildUI(uiact);
			}
			ImGui::End();
		}
		buildPopupsUI();

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
