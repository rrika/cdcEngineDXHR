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
#include "types.h"
#include "drm/DRMIndex.h"
#include "drm/ResolveObject.h"
#include "drm/ResolveReceiver.h"
#include "drm/ResolveSection.h"
#include "filesystem/ArchiveFileSystem.h"
#include "filesystem/FileHelpers.h" // for archiveFileSystem_default
#include "filesystem/FileUserBufferReceiver.h"
#include "game/dtp/objecttypes/globaldatabase.h"
#ifdef _WIN32
#include "gameshell/win32/MainVM.h" // for yellowCursor
#endif
#include "input/PCMouseKeyboard.h"
#include "locale/localstr.h"
#include "math/Math.h" // for float4x4
#include "object/Object.h"
#include "object/ObjectManager.h" // for buildObjectsUI
#include "rendering/buffers/PCDX11ConstantBufferPool.h"
#include "rendering/buffers/PCDX11IndexBuffer.h"
#include "rendering/buffers/PCDX11SimpleStaticIndexBuffer.h"
#include "rendering/buffers/PCDX11SimpleStaticVertexBuffer.h"
#include "rendering/buffers/PCDX11UberConstantBuffer.h"
#include "rendering/IPCDeviceManager.h"
#include "rendering/IRenderPassCallback.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/PCDX11MatrixState.h"
#include "rendering/PCDX11RenderContext.h"
#include "rendering/PCDX11RenderDevice.h"
#include "rendering/PCDX11RenderModel.h"
#include "rendering/PCDX11RenderModelInstance.h"
#include "rendering/PCDX11Scene.h"
#include "rendering/PCDX11StateManager.h"
#include "rendering/PCDX11StreamDecl.h"
#include "rendering/RenderModelInstance.h"
#include "rendering/RenderPasses.h"
#include "rendering/shaders/PCDX11PixelShader.h"
#include "rendering/shaders/PCDX11VertexShader.h"
#include "rendering/surfaces/PCDX11DefaultRenderTarget.h"
#include "rendering/surfaces/PCDX11DepthBuffer.h"
#include "rendering/surfaces/PCDX11Texture.h"
#include "rendering/VertexAttribute.h"
#include "world/RMIDrawableBase.h"
#include "world/stream.h" // for buildUnitsUI

#if ENABLE_IMGUI
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_win32.h"
#include "imgui/backends/imgui_impl_dx11.h"
#include "rendering/Inspector.h"
#endif

#ifdef __linux__
#include <SDL2/SDL.h>
#endif

struct float3 { float x, y, z; };

class ImGuiDrawable : public cdc::IRenderDrawable {
public:
	std::function<void()> lastMinuteAdditions;

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other) override { /*TODO*/ return 0; };
};

class SpinnyCubePass : public cdc::IRenderPassCallback {
public:
	D3D11_VIEWPORT *viewport;
	ID3D11RasterizerState1* rasterizerState;
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

cdc::ResolveObject *requestDRM(
	const char *path,
	void **rootPtr = nullptr,
	void (*callback)(void*, void*, void*, cdc::ResolveObject*) = nullptr,
	void *callbackArg1 = nullptr,
	void *callbackArg2 = nullptr
) {
	printf("loading %s\n", path);

	cdc::ResolveObject *ro = new cdc::ResolveObject(path);
	auto *rr = new cdc::ResolveReceiver(callback, callbackArg1, callbackArg2, rootPtr, nullptr, nullptr, ro, 0, &drmIndex);
	cdc::FileRequest *req = cdc::archiveFileSystem_default->createRequest(rr, path, 0);
	req->submit(3);
	req->decrRefCount();
	cdc::archiveFileSystem_default->processAll();
	// req is owned by fs which takes care of it in processAll()
	// rr self-deletes

	return ro;
}

int spinnyCube(HWND window,
	ID3D11Device *baseDevice,
	ID3D11DeviceContext *baseDeviceContext) {

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
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(baseDevice, baseDeviceContext);
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////

	D3D11_RASTERIZER_DESC1 rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_BACK;

	ID3D11RasterizerState1* rasterizerState;

	device->CreateRasterizerState1(&rasterizerDesc, &rasterizerState);

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
	cdc::requestObject3(bottleIndex);

	auto lightIndex = cdc::objectIdByName("deferred_fast_omni_diffuse");
	cdc::requestObject3(lightIndex);

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
		3
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
		3
	);
/*
	auto obj5 = cdc::ResolveObject::create(
		"pc-w\\scenario_database.drm",
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		0,
		3
	);
*/
	cdc::archiveFileSystem_default->processAll();

	cdc::ResolveSection *objectSection = cdc::g_resolveSections[11];
	cdc::ObjectBlob *bottleObject = (cdc::ObjectBlob*)objectSection->getWrapped(objectSection->getDomainId(0x04a8));
	printf("have bottle object: %p\n", bottleObject);

	// unrelated: get the name of the first map in the game
	uint32_t globalDatabaseId = cdc::objectIdByName("GlobalDatabase");
	cdc::ObjectBlob *globalDatabaseObject = (cdc::ObjectBlob*)objectSection->getWrapped(objectSection->getDomainId(globalDatabaseId));
	auto *globalDatabase = (GlobalDatabase*)globalDatabaseObject->dword58;

	printf("first map is: %s\n", globalDatabase->newGameMap);

	auto bottleTexture = (cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x0396);
	printf("have bottle cdc texture: %p\n", bottleTexture);
	bottleTexture->asyncCreate();
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

	// cdc::RenderModelInstance *bottleRenderModelInstance =
	// 	renderDevice->createRenderModelInstance(bottleRenderModel);

	///////////////////////////////////////////////////////////////////////////////////////////////

	// for some reason deferred_fast_omni_diffuse.drm depends on shaderlibs/decal_853189b19db6f909_dx11.drm
	// when it should be using a shader from shaderlibs/decal_853189b19db6f909_dx11.drm
	// namely section 5: ShaderLib 0 cd2 unk6:2ce3 DX11 (4f38 bytes)
	//
	// one of the few materials referencing this shaderlib is
	// s_scn_det_sarifhq_rail_tutorial_barrettintro_det_sarifhq_rail_tutorial.drm/7: Material 12 a4 unk6:7a84 DX11 (7e0 bytes)


	cdc::ObjectBlob *lightObject = (cdc::ObjectBlob*)objectSection->getWrapped(objectSection->getDomainId(lightIndex));
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
	float f = 9.0f;                             // far

	float scale = 0.05f;
	float3 modelRotation    = { 0.0f, 0.0f, 0.0f };
	float3 modelScale       = { scale, scale, scale };
	float3 modelTranslation = { 0.0f, 0.0f, 4.0f };

	///////////////////////////////////////////////////////////////////////////////////////////////

	cdc::RenderViewport renderViewport;
	renderViewport.mask = 0x3103; // pass 0, 12. 13, 1, and 8
	// pass 12 normals (function set 10, draw bottle normals)
	// pass 13 deferred shading (just contains a cleardrawable)
	// pass 1 composite (draw bottle textures)
	// pass 8 runs last and is where I put imgui since it messes with the render state

	SpinnyCubePass cubePass;
	cubePass.viewport = &viewport;
	cubePass.rasterizerState = rasterizerState;
	cubePass.keepAlphaBlend = keepAlphaBlend;
	renderDevice->setPassCallback(0, &cubePass);

	ImGuiDrawable imGuiDrawable;

	///////////////////////////////////////////////////////////////////////////////////////////////

#if ENABLE_IMGUI
	bool showDrawablesWindow = false;
	bool showFilesystemWindow = false;
	bool showObjectsWindow = false;
	bool showDRMWindow = false;
	bool showUnitsWindow = false;
	bool showStringsWindow = false;
	std::vector<std::pair<void*, cdc::CommonScene*>> captures { { nullptr, nullptr } };
	uint32_t selectedCapture = 0;
#endif

	bool mouseLook = false;

	while (true)
	{
#ifdef WIN32
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
			switch (event.type) {
				case SDL_QUIT:
					goto end;
				default:
					break;
			}
		}
#endif

		mouseKeyboard->update();

#if ENABLE_IMGUI
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame(); // this will reset our pretty cursor
		ImGui::NewFrame();

		if (ImGui::IsKeyPressed(ImGuiKey_Tab)) {
			mouseLook = !mouseLook;
			mouseKeyboard->setCursorGrab(mouseLook);
		}

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

		///////////////////////////////////////////////////////////////////////////////////////////

		float4x4 rotateX   = { 1, 0, 0, 0, 0, static_cast<float>(cos(modelRotation.x)), -static_cast<float>(sin(modelRotation.x)), 0, 0, static_cast<float>(sin(modelRotation.x)), static_cast<float>(cos(modelRotation.x)), 0, 0, 0, 0, 1 };
		float4x4 rotateY   = { static_cast<float>(cos(modelRotation.y)), 0, static_cast<float>(sin(modelRotation.y)), 0, 0, 1, 0, 0, -static_cast<float>(sin(modelRotation.y)), 0, static_cast<float>(cos(modelRotation.y)), 0, 0, 0, 0, 1 };
		float4x4 rotateZ   = { static_cast<float>(cos(modelRotation.z)), -static_cast<float>(sin(modelRotation.z)), 0, 0, static_cast<float>(sin(modelRotation.z)), static_cast<float>(cos(modelRotation.z)), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		float4x4 bottleScale = { modelScale.x, 0, 0, 0, 0, modelScale.y, 0, 0, 0, 0, modelScale.z, 0, 0, 0, 0, 1 };
		float4x4 cameraTranslate = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, modelTranslation.x, modelTranslation.y, modelTranslation.z, 1 };
		float4x4 lightScaleTranslate = { lightRadius * 2.0f, 0, 0, 0, 0, lightRadius * 2.0f, 0, 0, 0, 0, lightRadius * 2.0f, 0, 1, 1, 1, 1 };

		modelRotation.x += 0.005f;
		modelRotation.y += 0.009f;
		modelRotation.z += 0.001f;

		if (mouseLook) {
			modelRotation.x += mouseKeyboard->state.deltaY;
			modelRotation.y += mouseKeyboard->state.deltaX;
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		float4x4 world = rotateZ * rotateY * rotateX;
		float4x4 project = { 2 * n / w, 0, 0, 0, 0, 2 * n / h, 0, 0, 0, 0, f / (f - n), 1, 0, 0, n * f / (n - f), 0 };

		// Constants constants;
		// constants.WorldViewProject = project * world;
		// constants.World = world;
		// constants.ViewProject = project;

		stateManager.setWorldMatrix(world);

		// memcpy(cdcConstantBuffer.data, &constants, sizeof(Constants));
		// cdcConstantBuffer.syncBuffer(deviceContext);

		renderDevice->resetRenderLists();
		renderDevice->beginRenderList(nullptr);
		auto *scene = renderDevice->createSubScene(
			&renderViewport,
			renderContext->renderTarget2C,
			renderContext->depthBuffer);
		scene->viewMatrix = cameraTranslate;
		scene->projectMatrix = project;

		cdc::Matrix bottleWorldMatrix = world * bottleScale;
		// cdc::PCDX11MatrixState bottleMatrixState(renderDevice);
		// bottleMatrixState.resize(0);
		// auto *pBottleWorldMatrix = reinterpret_cast<float4x4*>(bottleMatrixState.poseData->getMatrix(0));
		// *pBottleWorldMatrix = bottleWorldMatrix;

		cdc::PCDX11MatrixState lightMatrixState(renderDevice);
		lightMatrixState.resize(1);
		auto *lightWorldMatrix = reinterpret_cast<float4x4*>(lightMatrixState.poseData->getMatrix(0));
		*lightWorldMatrix = /*world * */ lightScaleTranslate;

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
		rmiDrawable.draw(&bottleWorldMatrix, 0.0f);
		renderDevice->recordDrawable(&imGuiDrawable, /*mask=*/ 0x100, /*addToParent=*/ 0);

		renderDevice->finishScene();
		renderDevice->endRenderList();

#if ENABLE_IMGUI
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Windows")) {
				if (ImGui::MenuItem("Capture frame")) {

					selectedCapture = captures.size();
					captures.push_back({renderDevice->captureRenderLists(), scene});
				}
				if (ImGui::MenuItem("Show drawables")) { showDrawablesWindow = true; }
				// if (ImGui::MenuItem("Show filesystem")) { showFilesystemWindow = true; }
				if (ImGui::MenuItem("Show objects")) { showObjectsWindow = true; }
				if (ImGui::MenuItem("Show DRMs")) { showDRMWindow = true; }
				if (ImGui::MenuItem("Show units")) { showUnitsWindow = true; }
				if (ImGui::MenuItem("Show strings")) { showStringsWindow = true; }
				ImGui::EndMenu();
			}
			if (mouseLook)
				ImGui::Text("Press TAB to release cursor");
			ImGui::EndMainMenuBar();
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
			ImGui::Begin("DRMs", &showDRMWindow);
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
						ImGui::Text("%3d: %04x %s unk6:%x (%d bytes)",
							i++, section.id, names[section.type], section.unknown06, section.payloadSize);
						if (section.type == 5) {
							ImGui::Text("    ");
							ImGui::SameLine();
							auto *resource = (cdc::RenderResource*)cdc::g_resolveSections[5]->getWrapped(section.id);
							if (auto tex = dynamic_cast<cdc::PCDX11Texture*>(resource)) {
								ImGui::Image(
									tex->createShaderResourceView(), ImVec2(256, 256));
							}
						}
					}
					ImGui::TreePop();
				}
			}
			ImGui::End();
		}
		if (showUnitsWindow) {
			ImGui::Begin("Units", &showUnitsWindow);
			buildUnitsUI();
			ImGui::End();
		}
		if (showStringsWindow) {
			if (ImGui::Begin("Strings", &showStringsWindow)) {
				for (uint32_t i = 0; i < 1000; i++)
					ImGui::Text("%5d %s", i, localstr_get(i));
			}
			ImGui::End();
		}
#endif

		renderDevice->drawRenderLists();

		///////////////////////////////////////////////////////////////////////////////////////////

		renderContext->present();
	}
end:
#if ENABLE_IMGUI 
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
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
	deviceContext->RSSetState(rasterizerState);

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
