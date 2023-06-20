#include <cstdint>
#include "Inspector.h"
#include "3rdParty/imgui/imgui.h"
#include "cdcAnim/AnimComponentV2.h"
#include "cdcAnim/AnimDecoder.h"
#include "cdcAnim/AnimFragment.h"
#include "cdcAnim/AnimPoseNode.h"

using namespace cdc;

static float cbShortArray(void* data, int idx) {
	auto *offsets = (int16_t*)data;
	return offsets[idx] / 4096.f;
}

static float cbLinear16(void* data, int idx) {
	auto *decoder = (AnimDecoder*)data;
	return decoder->GetValues(true, idx);
}

static float cbLinear32(void* data, int idx) {
	auto *decoder = (AnimDecoder*)data;
	return decoder->GetValues(false, idx);
}

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

	ImGui::Indent();
	if (fragment->mKeyCount == 1) { // decode pose
		uint16_t *masks = fragment->mSegKeyListPtr + 1;
		float *values = fragment->mValueDataPtr + 1;
		for (int i=0; i < fragment->mSegmentCount; i++) {
			ImGui::Text("bone %d", i);
			uint8_t mask = uint8_t(*masks++);
			for (int j=0; j<8; j++) {
				if ((mask >> j) & 1) {
					ImGui::SameLine();
					ImGui::Text("%c=%f", "ijklxyzw"[j], *values++);
				}
			}
		}
	} else { // decode frame
		uint16_t *masks = fragment->mSegKeyListPtr + 1;
		uint16_t *lengths = fragment->mLengthDataPtr;
		char *values = (char*)(fragment->mValueDataPtr + 1);
		for (int i=0; i < fragment->mSegmentCount; i++) {
			uint8_t mask = uint8_t(*masks++);
			ImGui::Text("bone %d mask=%x", i, mask);
			for (int j=0; j<8; j++) {
				if ((mask >> j) & 1) {
					uint16_t mode = lengths[0];
					static const char *modes[] = { "tabulated", "constant", "piece-wise linear", "zero" };
					ImGui::Text(" channel %c %s @ %p %p",
						"ijklxyzw"[j],
						mode < 4 ? modes[mode] : "invalid mode",
						(void*)values,
						(void*)lengths);
					if (mode == 0) { // tabulated
						uint16_t fmt16 = lengths[1];
						if (fmt16) {
							auto *offsets = (int16_t*)values;
							//for (int k=0; k<fragment->mKeyCount; k++)
							//	ImGui::Text("  [%d] = %d", k, offsets[k]);
							ImGui::PlotLines("Lines", &cbShortArray, (void*)offsets, fragment->mKeyCount); // , 0, 0, -1.0f, 1.0f, ImVec2(0, 80.0f));
							values += sizeof(uint16_t) * ((fragment->mKeyCount+1) & ~1);
						} else {
							auto *offsets = (float*)values;
							//for (int k=0; k<fragment->mKeyCount; k++)
							//	ImGui::Text("  [%d] = %f", k, offsets[k]);
							ImGui::PlotLines("Lines", offsets, fragment->mKeyCount); // , 0, 0, -1.0f, 1.0f, ImVec2(0, 80.0f));
							values += sizeof(float) * fragment->mKeyCount;
						}
						lengths += 2;

					} else if (mode == 1) { // constant
						ImGui::Text("  %f", *(float*)values);
						values += sizeof(float);
						lengths++;

					} else if (mode == 2) { // piece-wise linear
						const char *fmts[] = { "f32", "i16" };
						uint16_t fmt = lengths[1];
						uint16_t numEntries = lengths[2];
						ImGui::Text("  %s #points %d", fmts[fmt], numEntries);
						AnimDecoder decoder;
						decoder.SetChannel(fmt, values, (uint8_t*)lengths);
						// ImGui::Text("  sum of lengths %d", decoder.mSumOfLengths);
						if (fmt) {
							ImGui::PlotLines("Lines", &cbLinear16, (void*)&decoder, fragment->mKeyCount);
						} else {
							ImGui::PlotLines("Lines", &cbLinear32, (void*)&decoder, fragment->mKeyCount);
						}
						if (fmt) {
							values += sizeof(uint16_t) * ((numEntries+1) & ~1);
						} else {
							values += sizeof(float) * numEntries;
						}
						lengths = (uint16_t*)(uintptr_t(lengths) + 5 + (numEntries|1));
					} else { // zero
						// empty
					}
				}
			}
		}
	}

	uint16_t *extraInfo = fragment->mExtraChannelLengthPtr;
	float *extraValues = fragment->mExtraChannelDataPtr;
	for (int i=0; i < fragment->mExtraChannelCount; i++) {
		uint8_t vectorIndex = *extraInfo++;
		uint8_t componentIndex = *extraInfo++;
		ImGui::Text("extra[%d].%c=%f", vectorIndex, "?xyzw"[componentIndex], *extraValues++);
	}
	ImGui::Unindent();

	ImGui::Unindent();
}

void buildUI(UIActions& uiact, AnimComponentV2 *ac) {
	ImGui::Text("todo");
}
