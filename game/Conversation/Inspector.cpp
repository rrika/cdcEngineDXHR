#include "cdcLocale/localstr.h"
#include "cdcRender/surfaces/PCDX11Texture.h"
#include "cdcResource/ResolveSection.h"
#include "cdcSound/SoundPlex.h"
#include "game/dtp/conversation.h"
#include "imgui/imgui.h"
#include "UIActions.h"


void buildUI(UIActions& uiact, dtp::ConversationGraphBase *dtpGraph) {
	uint32_t tier = dtpGraph->tier;
	ImGui::Text("tier %d", tier);
	ImGui::Indent();
	// if (tier == 1) {
		auto *nodeList = &static_cast<dtp::ConversationGraph*>(dtpGraph)->sub8->nodeList;
		for (uint32_t i=0; i<nodeList->dword0; i++) {
			dtp::ConversationGraphNode *node = &nodeList->nodes[i];
			ImGui::Text("node[%d] %s %s", i, node->dtpPath, node->typeName);
			if (strcmp(node->typeName, "Line") == 0) {
				dtp::BriefingLine *line = node->line;
				ImGui::PushID(line->voiceLineIndex);
				auto *plex = (dtp::SoundPlex*)cdc::g_resolveSections[7]->GetBasePointer(line->voiceLineIndex);
				if (plex) {
					if (ImGui::SmallButton("Play")) {
						cdc::SOUND_StartPaused(plex, /*delay=*/ 0.0f);
					}
					ImGui::SameLine();
				}
				if (line->profile) {
					ImGui::Text("%s:", localstr_get(line->profile->nameIndex));
					ImGui::SameLine();
					if (line->profile->texture) {
						auto *resource = (cdc::RenderResource*)cdc::g_resolveSections[5]->GetBasePointer(line->profile->texture);
						if (auto tex = dynamic_cast<cdc::PCDX11Texture*>(resource))
							ImGui::Image(
								tex->createShaderResourceView(), ImVec2(64, 64));
					}
				}
				auto *un = line->untranslated;
				auto *tr = localstr_get(line->stringIndex);
				if (un && tr && strcmp(un, tr) != 0) {
					ImGui::Text("%s", tr);
					ImGui::Text("%s", un);
				} else {
					ImGui::Text("%s", un);
				}
				ImGui::PopID();
			}
		}
	// }
	ImGui::Unindent();
}
