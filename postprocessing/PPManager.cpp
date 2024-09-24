#include "PPBuiltins.h"
#include "PPManager.h"
#include "PPPass.h"
#include "rendering/CommonMaterial.h"
#include "rendering/MaterialData.h"
#include "rendering/PCDX11RenderDevice.h"
#include "rendering/surfaces/PCDX11RenderTarget.h"

#include "config.h"

#if ENABLE_IMGUI
#include "imgui/imgui.h"
#include "UIActions.h"
#endif

using namespace cdc;

static cdc::VertexDecl *makeVertexDecl() {
	std::vector<VertexAttributeA> attrs;
	attrs.push_back({VertexAttributeA::kPosition,  0,  2, 0});
	attrs.push_back({VertexAttributeA::kTexcoord1, 12, 1, 0});
	auto *vd = VertexDecl::Create(attrs.data(), attrs.size(), /*stride=*/ sizeof(float)*5);
	return vd;
}

VertexDecl *ppVertexDecl = makeVertexDecl();

void PPQuad(float depth, cdc::IMaterial *material, cdc::MaterialInstanceParams *instanceParams, float sortZ, uint32_t primFlags, uint32_t passMask) {
	float d = depth;
	float verts[6 * 5] = {
		0, 0, d, 0, 0,
		1, 0, d, 1, 0,
		0, 1, d, 0, 1,

		1, 0, d, 1, 0,
		1, 1, d, 1, 1,
		0, 1, d, 0, 1
	};
	g_renderDevice->DrawIndexedPrimitive(
		&identity4x4,
		verts,
		ppVertexDecl,
		/*numVerts=*/    0, // automatic
		/*indexBuffer=*/ nullptr,
		/*numPrims=*/    2,
		primFlags | 0x10000040,
		material,
		instanceParams,
		sortZ,
		passMask,
		0);
}

PPManager *PPManager::s_instance = nullptr;

PPManager::PPManager() {
	activeSets.reserve(16);
	// inputs.reserve(16);
	textures.reserve(16);
	variables.reserve(16);
}

static void markPrePassLive(uint32_t passIndex, uint32_t prePassIndex, dtp::PPPassBlobArray *passArray, uint32_t *outputPrePassMasks, uint32_t *outVariablesMask) {

	dtp::PPPrePassBlob *prePass = &passArray->data[passIndex].prePasses[prePassIndex];
	uint32_t textureIndices[8];
	memcpy(textureIndices, prePass->textureIndices, sizeof(uint32_t) * 8);

	// for every variable
	for (uint32_t i=0; i<24; i++)
		if (prePass->variableIndices[i] != ~0u)
			*outVariablesMask |= 1 << prePass->variableIndices[i];

	for (uint32_t i=0; i<8; i++) {
		if (textureIndices[i] == ~0u)
			continue;

		if (textureIndices[i] == 0) // this isn't in the original game but avoids false dependencies
			continue;

		uint32_t writerPrePass = 255;
		uint32_t writerPass = 255;

		for (uint32_t j=0; j<=passIndex; j++) {

			dtp::PPPassBlob *jpass = &passArray->data[j];

			for (uint32_t k=0; k<jpass->numPrePasses; k++) {
				if (j == passIndex && k >= prePassIndex)
					continue;

				if (textureIndices[i] == jpass->prePasses[k].outputTextureIndex) {
					writerPass = j;
					writerPrePass = k;
				}
			}
		}

		if (writerPass != 255) {
			outputPrePassMasks[writerPass] |= 1 << writerPrePass;
			markPrePassLive(writerPass, writerPrePass, passArray, outputPrePassMasks, outVariablesMask);
		}
	}
}

static void markPassLive(uint32_t passIndex, dtp::PPPassBlobArray *passArray, uint32_t *outputPrePassMasks, uint32_t *outVariablesMask) {

	dtp::PPPassBlob *pass = &passArray->data[passIndex];
	uint32_t textureIndices[8];
	memcpy(textureIndices, pass->textureIndices, sizeof(uint32_t) * 8);

	// for every variable
	for (uint32_t i=0; i<24; i++)
		if (pass->variableIndices[i] != ~0u)
			*outVariablesMask |= 1 << pass->variableIndices[i];

	// for every texture
	for (uint32_t i=0; i<8; i++) {
		if (textureIndices[i] == ~0u)
			continue;

		uint32_t writerPrePass = 255;
		uint32_t writerPass = 255;

		// for every prior pass
		for (uint32_t j=0; j<=passIndex; j++) {

			dtp::PPPassBlob *jpass = &passArray->data[j];

			// for every prepass thereof
			for (uint32_t k=0; k<jpass->numPrePasses; k++)
				if (textureIndices[i] == jpass->prePasses[k].outputTextureIndex) {
					writerPass = j;
					writerPrePass = k;
				}
		}

		// mark the prepass
		if (writerPass != 255) {
			outputPrePassMasks[writerPass] |= 1 << writerPrePass;
			markPrePassLive(writerPass, writerPrePass, passArray, outputPrePassMasks, outVariablesMask);
		}
	}
}

static uint32_t RootPassesForActiveSet(dtp::PPActiveSet *activeSet) {
	uint32_t rootPasses = 0;

	for (uint32_t i = 0; i < activeSet->alwaysActivePassCount; i++)
		rootPasses |= 1 << activeSet->alwaysActivePassIndices[i];

	DisplayConfig *dc = deviceManager->getDisplayConfig();
	if (dc->antiAliasing == 1)
		for (uint32_t i = 0; i < activeSet->antialiasPassCount; i++)
			rootPasses |= 1 << activeSet->antialiasPassIndices[i];

	else if (dc->antiAliasing > 1)
		for (uint32_t i = 0; i < activeSet->antialias2PassCount; i++)
			rootPasses |= 1 << activeSet->antialias2PassIndices[i];

	return rootPasses;
}

bool PPManager::prepare() {

	dtp::PPVarPassTexBlobs *varPassTex = fallbackVarPassTex; // activeSets[0]->varPassTex;

	// TODO

	bool redoTextures = textures.size() != varPassTex->textures.size;

	if (width != g_renderDevice->getContextWidth() ||
		height != g_renderDevice->getContextHeight())
	{
		width = g_renderDevice->getContextWidth();
		height = g_renderDevice->getContextHeight();
		redoTextures = true;
	}

	if (redoTextures) {
		for (PPTexture& tex : textures)
			tex.freeRenderTarget();

		while (textures.size() < varPassTex->textures.size)
			textures.push_back(PPTexture());

		for (uint32_t i = 0; i < textures.size(); i++)
			textures[i].init(&varPassTex->textures.data[i]);
	}

	variables.resize(0);
	variables.resize(varPassTex->variables.size);
	for (uint32_t i = 0; i < variables.size(); i++)
		variables[i].init(&varPassTex->variables.data[i]);

	rootPasses = 0;
	for (auto *activeSet : activeSets)
		rootPasses |= RootPassesForActiveSet(activeSet);

	rootPasses ^= (rootPasses ^ rootOverride) & rootOverrideMask;

	// TODO

	for (uint32_t i = 0; i < 32; i++)
		prePassMasks[i] = 0;
	variablesMask = 0;

	for (uint32_t i = 0; i < varPassTex->passes.size; i++)
		if ((1 << i) & rootPasses)
			markPassLive(i, &varPassTex->passes, prePassMasks, &variablesMask);

	// TODO

	return true;
}

bool PPManager::run(
	cdc::CommonRenderTarget *rt,
	cdc::CommonRenderTarget *rtParticle,
	cdc::CommonRenderTarget *rtDest, // HACK: allow specifying a different target
	cdc::CommonDepthBuffer *depth,
	cdc::RenderViewport *viewport
) {
	if (!rtDest)
		rtDest = rt;

	if (!prepare())
		return false;

	if (false) { // HACK

		auto renderDevice = static_cast<cdc::PCDX11RenderDevice*>(cdc::g_renderDevice);
		cdc::CommonRenderTarget *aaDst;
		bool fastblur = false;

		if (rt == rtDest) {
			auto *tempRenderTarget = renderDevice->dx11_createRenderTarget(
				100, 100,
				fastblur ? DXGI_FORMAT_B8G8R8A8_UNORM : DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
				0x18, cdc::kTextureClass2D);
			tempRenderTarget->getRenderTexture11()->createRenderTargetView();
			aaDst = tempRenderTarget;

		} else {
			static_cast<cdc::PCDX11RenderTarget*>(rt)->getRenderTexture11()->createRenderTargetView();
			aaDst = rtDest;
		}

		renderDevice->createSubScene(
			viewport,
			rtDest,
			depth,
			nullptr,
			nullptr);
		renderDevice->getScene()->debugName = "post-process";

		if (fastblur)
			PPFastBlur(
				/*src=*/ rt->getRenderTexture(),
				/*dst=*/ aaDst,
				/*passMask=*/ 0x100,
				/*isHorizonalPass=*/ true,
				/*weighted=*/ false);
		else
			PPAntiAlias(
				/*src=*/ rt->getRenderTexture(),
				/*dst=*/ aaDst,
				/*passMask=*/ 0x100);

		rt = aaDst;
		renderDevice->finishScene();

		if (rt != rtDest) {
			renderDevice->createSubScene(
				viewport,
				rtDest,
				nullptr,
				rt,
				nullptr);
			renderDevice->getScene()->debugName = "copy rt to rtDest";
			renderDevice->finishScene();
		}

		return true;
	}

	auto renderDevice = static_cast<cdc::PCDX11RenderDevice*>(cdc::g_renderDevice);
	auto *newRenderTarget = renderDevice->dx11_createRenderTarget(
		100, 100,
		DXGI_FORMAT_B8G8R8A8_UNORM_SRGB,
		0x18, cdc::kTextureClass2D);
	newRenderTarget->getRenderTexture11()->createRenderTargetView();

	PPRTs rts = {
		.rt0 = newRenderTarget,
		.db = depth,
		.rt8 = rtParticle,
		.rtC = rt
	};

	dtp::PPVarPassTexBlobs *vpt = fallbackVarPassTex; // activeSets[0]->varPassTex;
	dtp::PPPassBlob *passBlobs = vpt->passes.data;
	CommonRenderTarget *currentRt = rt;
	PPPass pass;
	for (uint32_t i=0; i < vpt->passes.size; i++) {
		if (pass.init(&passBlobs[i],
			textures.data(),
			textures.size(),
			variables.data(),
			variables.size()))
		{
			uint32_t texturesMask = 0;
			pass.run(
				&currentRt,
				&rts,
				viewport,
				prePassMasks[i],
				(rootPasses >> i) & 1,
				texturesMask
			);
		}
	}

	// if (rts.rtC != rt) {
	if (currentRt != rtDest) {
		cdc::RenderViewport newViewport;
		newViewport.clearMode = 0;
		newViewport.mask = 0xFFFF8FFF; // TODO
		newViewport.byteC9 = 0;
		auto copyScene = g_renderDevice->createSubScene(
			&newViewport,
			rtDest, // nullptr
			nullptr,
			currentRt, // newRenderTarget,
			nullptr);
		g_renderDevice->finishScene();

		copyScene->debugName = "copy to final dest";
	}
	return true;
}

void PPManager::resetActiveSets() {
	activeSets.clear();
}

void PPManager::addActiveSet(dtp::PPActiveSet *activeSet, float f) {
	// HACK
	activeSets.push_back(activeSet);
}

#if ENABLE_IMGUI


static void buildUI(UIActions& uiact, dtp::PPPrePassBlob *prePassBlob, dtp::PPVarPassTexBlobs *varPassTex) {
	ImGui::PushID(prePassBlob);
	ImGui::Text("%p %s\n", prePassBlob, prePassBlob->name);
	ImGui::Indent();
	ImGui::Text("output to %02x, clearMode %02x, clearColor/sourceTexture %08x, clearColorScale %f",
		(int)prePassBlob->outputTextureIndex,
		(int)prePassBlob->clearMode,
		(int)prePassBlob->clearColor,
		prePassBlob->clearColorScale);
	ImGui::Text("enabled %02x, useDepth %02x, byte16 %02x",
		(int)prePassBlob->enabled,
		(int)prePassBlob->useDepthBuffer,
		(int)prePassBlob->byte16);

	if (prePassBlob->builtinShaderType > 0) {
		ImGui::Text("built-in shader %d", prePassBlob->builtinShaderType);
	} else {
		if (ImGui::SmallButton("Material")) {
			uiact.select(prePassBlob->material);
			uiact.select(static_cast<CommonMaterial*>(prePassBlob->material)->materialBlob->subMat4C[8 /*transparent*/]);
			uiact.select(ppVertexDecl);
		}
	}

	ImGui::Indent();
	for (uint32_t i=0; i<24; i++) {
		if (prePassBlob->variableIndices[i] == ~0u)
			continue;
		if (prePassBlob->variableIndices[i] >= varPassTex->variables.size) {
			ImGui::Text("out of bounds variable %d", prePassBlob->variableIndices[i]);
			continue;
		}
		ImGui::Text("%s %s",
			i < 8 ? "vari" : "varg",
			varPassTex->variables.data[prePassBlob->variableIndices[i]].name);
	}
	for (uint32_t i=0; i<8; i++) {
		if (prePassBlob->textureIndices[i] == ~0u)
			continue;
		if (prePassBlob->textureIndices[i] >= varPassTex->textures.size) {
			ImGui::Text("out of bounds texture %d", prePassBlob->textureIndices[i]);
			continue;
		}
		ImGui::Text("tex %s", varPassTex->textures.data[prePassBlob->textureIndices[i]].name);
	}
	ImGui::Unindent();
	ImGui::Unindent();
	ImGui::PopID();
}

static void buildUI(UIActions& uiact, dtp::PPPassBlob *passBlob, dtp::PPVarPassTexBlobs *varPassTex) {
	ImGui::PushID(passBlob);
	ImGui::Text("%s\n", passBlob->name);
	ImGui::Indent();
	ImGui::Text("en=%02x %s xE=%02x gate=%02x blend=%02x, scaleform %02x settings dependent %02x",
		(int)passBlob->enabled,
		passBlob->useDepthBuffer ? "use depth" : "not reading depth",
		(int)passBlob->byteE,
		(int)passBlob->gatePrePasses,
		(int)passBlob->blendOverPrevious,
		(int)passBlob->scaleform,
		(int)passBlob->canBeDisabledThroughSettings);

	for (uint32_t i=0; i<passBlob->numPrePasses; i++) {
		auto *prePassBlob = passBlob->prePasses+i;
		// ImGui::Text("%s\n", prePassBlob->name);
		buildUI(uiact, prePassBlob, varPassTex);
	}

	if (passBlob->builtinShaderType > 0) {
		ImGui::Text("built-in shader %d", passBlob->builtinShaderType);
	} else {
		if (ImGui::SmallButton("Material")) {
			uiact.select(passBlob->material);
			uiact.select(static_cast<CommonMaterial*>(passBlob->material)->materialBlob->subMat4C[8 /*transparent*/]);
			uiact.select(ppVertexDecl);
		}
	}

	ImGui::Indent();
	for (uint32_t i=0; i<24; i++) {
		if (passBlob->variableIndices[i] == ~0u)
			continue;
		if (passBlob->variableIndices[i] >= varPassTex->variables.size) {
			ImGui::Text("out of bounds variable %d", passBlob->variableIndices[i]);
			continue;
		}
		ImGui::Text("%s %s",
			i < 8 ? "vari" : "varg",
			varPassTex->variables.data[passBlob->variableIndices[i]].name);
	}
	for (uint32_t i=0; i<8; i++) {
		if (passBlob->textureIndices[i] == ~0u)
			continue;
		if (passBlob->textureIndices[i] >= varPassTex->textures.size) {
			ImGui::Text("out of bounds texture %d", passBlob->textureIndices[i]);
			continue;
		}
		ImGui::Text("tex %s", varPassTex->textures.data[passBlob->textureIndices[i]].name);
	}
	ImGui::Unindent();
	ImGui::Unindent();
	ImGui::PopID();
}

static void VarEdit(dtp::PPVariableBlob *variable) {
	bool packed = variable->format == 12 ;
	unsigned char minu8[4] = {0, 0, 0, 0};
	unsigned char maxu8[4] = {255, 255, 255, 255};
	float minf32[4] = {0.f, 0.f, 0.f, 0.f};
	float maxf32[4] = {1.f, 1.f, 1.f, 1.f};
	ImGui::SliderScalarN(variable->name,
		packed ? ImGuiDataType_U8 : ImGuiDataType_Float,
		packed ? (void*)&variable->packed : (void*)&variable->x,
		packed ? 4 : (variable->format % 4)+1,
		packed ? (void*)&minu8 : (void*)&minf32,
		packed ? (void*)&maxu8 : (void*)&maxf32);
}

void PPManager::buildUI(UIActions& uiact) {

	auto *varPassTex = fallbackVarPassTex;
	if (!varPassTex)
		return;

	if (!ImGui::BeginTabBar("PPManager"))
		return;

	if (ImGui::BeginTabItem("Dynamic")) {

		dtp::PPActiveSet *hoverSet = nullptr;

		ImGui::BeginGroup();
		auto *dc = cdc::deviceManager->getDisplayConfig();
		ImGui::RadioButton("Off",         (int*)&dc->antiAliasing, 0);
		ImGui::RadioButton("FXAA Low",    (int*)&dc->antiAliasing, 2);
		ImGui::RadioButton("FXAA Medium", (int*)&dc->antiAliasing, 3);
		ImGui::RadioButton("FXAA High",   (int*)&dc->antiAliasing, 4);
		ImGui::RadioButton("MLAA",        (int*)&dc->antiAliasing, 5);
		for (auto set : activeSets) {
			ImGui::Text("%s", set->name);
			if (ImGui::IsItemHovered())
				hoverSet = set;
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		uint32_t hoverRootPasses = hoverSet ? RootPassesForActiveSet(hoverSet) : 0;

		ImGui::BeginGroup();
		ImGui::BeginDisabled(rootOverrideMask == 0);
		if (ImGui::Button("Reset overrides"))
				rootOverride = rootOverrideMask = 0;
		ImGui::EndDisabled();

		for (uint32_t i=0; i<varPassTex->passes.size; i++) {
			auto *pass = varPassTex->passes.data + i;
			bool highlight = (hoverRootPasses >> i) & 1;
			if (highlight)
				ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.0, 0.0, 1.0));
			if (ImGui::CheckboxFlags(pass->name, &rootPasses, 1<<i)) {
				rootOverride ^= (rootOverride ^ rootPasses) & (1<<i);
				rootOverrideMask |= 1<<i;
			}
			if (highlight)
				ImGui::PopStyleColor();
		}
		ImGui::EndGroup();

		ImGui::Checkbox("Show unused variables", &showUnusedVariables);
		uint32_t numVariables = varPassTex->variables.size;
		for (uint32_t i=0; i<numVariables; i++) {
			if (showUnusedVariables || (variablesMask & 1 << i) != 0)
				VarEdit(varPassTex->variables.data + i);
		}

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Static")) {

		bool copy = ImGui::Button("Copy to clipboard");
		if (copy)
			ImGui::LogToClipboard();
		for (uint32_t i=0; i<varPassTex->passes.size; i++) {
			auto *pass = varPassTex->passes.data + i;
			::buildUI(uiact, pass, varPassTex);
		}
		if (copy)
			ImGui::LogFinish();

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Routing")) {

		uint32_t numTextures = varPassTex->textures.size;
		const char **modes = new const char*[numTextures];
		if (ImGui::BeginTable("table", numTextures + 1, ImGuiTableFlags_SizingFixedFit)) {
			for (uint32_t i=0; i<varPassTex->passes.size; i++) {
				auto *pass = varPassTex->passes.data + i;
				// if ((1 << i & rootPasses) == 0 && pass->gatePrePasses)
				// 	continue;
				for (uint32_t j=0; j<pass->numPrePasses; j++) {
					auto *prePass = pass->prePasses + j;
					ImGui::TableNextRow();
					ImGui::PushID(prePass);
					ImGui::TableSetColumnIndex(0);
					ImGui::Text("pre %s", prePass->name);
					if (prePass->builtinShaderType == 0) {
						ImGui::SameLine();
						if (ImGui::SmallButton("Material")) {
							uiact.select(prePass->material);
							uiact.select(static_cast<CommonMaterial*>(prePass->material)->materialBlob->subMat4C[8 /*transparent*/]);
							uiact.select(ppVertexDecl);
						}
					}
					for (uint32_t k=0; k<numTextures; k++)
						modes[k] = "";
					for (uint32_t k=0; k<8; k++) {
						if (prePass->textureIndices[k] == ~0u)
							continue;
						modes[prePass->textureIndices[k]] = ".";
					}
					if (prePass->outputTextureIndex < numTextures)
						modes[prePass->outputTextureIndex] = "W";
					for (uint32_t k=0; k<numTextures; k++) {
						ImGui::TableSetColumnIndex(1+k);
						ImGui::Text("%s", modes[k]);
					}
					ImGui::PopID();
				}
				// if ((1 << i & rootPasses) == 0)
				// 	continue;
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::PushID(pass);
				ImGui::Text("%s", pass->name);
				if (pass->builtinShaderType == 0) {
					ImGui::SameLine();
					if (ImGui::SmallButton("Material")) {
						uiact.select(pass->material);
						uiact.select(static_cast<CommonMaterial*>(pass->material)->materialBlob->subMat4C[8 /*transparent*/]);
						uiact.select(ppVertexDecl);
					}
				}
				for (uint32_t k=0; k<numTextures; k++)
					modes[k] = "";
				for (uint32_t k=0; k<8; k++) {
					if (pass->textureIndices[k] == ~0u)
						continue;
					modes[pass->textureIndices[k]] = ".";
				}
				bool readingPrimaryRT = false;
				for (uint32_t i=0; i<8; i++)
					if (pass->textureIndices[i] != ~0u) {
						auto *textureBlob = &varPassTex->textures.data[pass->textureIndices[i]];
						if (textureBlob->dword4 == 0 && textureBlob->dword8 == 0) // reading the primary RT
							readingPrimaryRT = true;
					}
					modes[0] =
						readingPrimaryRT ? "RW" :
						pass->blendOverPrevious ? "rW" :
						"W";
				for (uint32_t k=0; k<numTextures; k++) {
					ImGui::TableSetColumnIndex(1+k);
					ImGui::Text("%s", modes[k]);
				}
				ImGui::PopID();
			}
			ImGui::EndTable();
		}

		ImGui::EndTabItem();
	}

	return ImGui::EndTabBar();
}
#endif

