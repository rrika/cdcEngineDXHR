#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdcAnim/AnimComponentV2.h"
#include "cdcAnim/AnimFragment.h"

using namespace cdc;

void buildUI(UIActions& uiact, cdc::AnimFragment *fragment) {
	ImGui::Text("fragment %p", fragment);
	if (!fragment)
		return;
	ImGui::Indent();
	ImGui::Text("mAnimID 0x%x", fragment->mAnimID);
	ImGui::Text("mKeyCount %d", fragment->mKeyCount);
	ImGui::Text("mTimePerKey %d", fragment->mTimePerKey);
	ImGui::Text("mSegmentCount %d", fragment->mSegmentCount);
	ImGui::Text("mSectionCount %d", fragment->mSectionCount);
	ImGui::Text("mExtraChannelCount %d", fragment->mExtraChannelCount);
	ImGui::Unindent();
}
