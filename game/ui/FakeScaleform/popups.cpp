#include <vector>
#include "imgui/imgui.h"
#include "game/Impulse.h"
#include "game/ui/NsPopupRequest.h"

static std::vector<NsPopupRequest*> newPopups;
static std::vector<NsPopupRequest*> popups;

void handlePopupRequest(void *popupManagerMovieController, IXYZ *arg) {
	auto *popup = (NsPopupRequest*) arg->z;
	newPopups.push_back(popup);
}

void registerPopupHandler() {
	void *popupManagerMovieController = nullptr;
	impulse_register(
		54,
		popupManagerMovieController,
		popupManagerMovieController,
		(ImpulseFunc*) &handlePopupRequest,
		0);
}

void buildPopupsUI() {
	for (auto popup : newPopups) {
		ImGui::OpenPopup(popup->m_title);
		popups.push_back(popup);
	}
	newPopups.clear();

	for (auto popup : popups) {
		ImVec2 size {400, 0};
		ImGui::SetNextWindowSize(size);
		if (ImGui::BeginPopupModal(popup->m_title, NULL,
			ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings))
		{
			ImGui::TextWrapped("%s", popup->m_message);
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			if (!ImGui::IsWindowAppearing() && !popup->m_mouseSet) {
				ImVec2 min = ImGui::GetItemRectMin();
				ImVec2 max = ImGui::GetItemRectMax();
				ImVec2 cen {0.5f*(min.x+max.x), 0.5f*(min.y+max.y)};
				ImGuiIO& io = ImGui::GetIO();
				io.WantSetMousePos = true;
				io.MousePos = cen;
				popup->m_mouseSet = true;
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
			ImGui::EndPopup();
		}
	}

	for (uint32_t i = 0; i < popups.size(); i++) {
		if (!ImGui::IsPopupOpen(popups[i]->m_title)) {
			popups[i--] = popups.back();
			popups.pop_back();
		}
	}
}

static void submitPopupRequest(NsPopupRequest *request) {
	request->submit();
}

void buildPopupsMenu() {
	if (ImGui::MenuItem("DirectX11RestartMessage (enable)"))
		submitPopupRequest(new NsPopupRequest("MainMenu_DirectX11Enabled", "MainMenu_DirectX11RestartMessage"));
	if (ImGui::MenuItem("DirectX11RestartMessage (disable)"))
		submitPopupRequest(new NsPopupRequest("MainMenu_DirectX11Disabled", "MainMenu_DirectX11RestartMessage"));
	if (ImGui::MenuItem("ExitConfirmation"))
		submitPopupRequest(new NsPopupRequest("MainMenu_ExitGame", "MainMenu_ExitConfirmation"));
	if (ImGui::MenuItem("ChangeDifficultyDesc"))
		submitPopupRequest(new NsPopupRequest("Confirmation_ChangeDifficultyTitle", "Confirmation_ChangeDifficultyDesc"));
	if (ImGui::MenuItem("Controller_Disconnected"))
		submitPopupRequest(new NsPopupRequest("Controller_Disconnected_Title", "Controller_Disconnected"));
	ImGui::Separator();
	if (ImGui::MenuItem("QuickLoadMessage"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_QuickLoadTitle", "SaveSystem_QuickLoadMessage"));
	if (ImGui::MenuItem("SaveError"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_SaveErrorTitle", "SaveSystem_SaveError"));
	if (ImGui::MenuItem("DLCMissing"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_LoadErrorTitle", "SaveSystem_DLCMissing"));
	if (ImGui::MenuItem("LoadSettingsError"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_LoadErrorTitle", "SaveSystem_LoadSettingsError"));
	if (ImGui::MenuItem("ExitToMainMenu"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_ExitToMainMenuTitle", "SaveSystem_ExitToMainMenu"));
	if (ImGui::MenuItem("SigninChanged"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_SigninChangedTitle", "SaveSystem_SigninChanged"));
	if (ImGui::MenuItem("NoProfileSelected"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_NoProfileSelectedTitle", "SaveSystem_NoProfileSelected"));
	if (ImGui::MenuItem("NotifyUnlockablePraxisPointsDesc"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_NotifyUnlockablePraxisPointsTitle", "SaveSystem_NotifyUnlockablePraxisPointsDesc"));
	if (ImGui::MenuItem("LoadingInProgress"))
		submitPopupRequest(new NsPopupRequest(0, "SaveSystem_LoadingInProgress"));
	if (ImGui::MenuItem("SavingInProgress"))
		submitPopupRequest(new NsPopupRequest(0, "SaveSystem_SavingInProgress"));
	if (ImGui::MenuItem("StorageDeviceDisconnected"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_StorageDeviceTitle", "SaveSystem_StorageDeviceDisconnected"));
	if (ImGui::MenuItem("NoStorageSelected"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_StorageDeviceTitle", "SaveSystem_NoStorageSelected"));
	if (ImGui::MenuItem("LoadError"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_LoadErrorTitle", "SaveSystem_LoadError"));
	if (ImGui::MenuItem("SaveSuccessfull"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_SaveSuccessfullTitle", "SaveSystem_SaveSuccessfull"));
	if (ImGui::MenuItem("NoSpaceForNewSlotPS3"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_InsufficientMemoryAvailableTitle", "SaveSystem_NoSpaceForNewSlotPS3"));
	if (ImGui::MenuItem("AutoSaveCanOnlyOverwrite"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_InsufficientMemoryAvailableTitle", "SaveSystem_AutoSaveCanOnlyOverwrite"));
	if (ImGui::MenuItem("OverwriteSaveGame"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_OverwriteSaveGameTitle","SaveSystem_OverwriteSaveGame"));
	if (ImGui::MenuItem("ConfirmDeleteMessage"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_ConfirmDeleteTitle", "SaveSystem_ConfirmDeleteMessage"));
	if (ImGui::MenuItem("DamagedSaveGame"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_InvalidSaveGameTitle", "SaveSystem_DamagedSaveGame"));
	if (ImGui::MenuItem("LoadSlotLoseProgress"))
		submitPopupRequest(new NsPopupRequest("SaveSystem_LoadSlotLoseProgressTitle", "SaveSystem_LoadSlotLoseProgress"));
}

