#include "imgui/imgui.h"
#include "../Scaleform/ScaleformMovie.h"
#include "fakescaleform.h"

class MainMenu : public GFxMovieRoot {
	bool canContinue = false;
	bool isNewGamePlusAvailable = false;
	bool canPlayCredits = false;

	bool visible = true;

	void init() {
		externalInterface->Callback(this, "CanContinue", nullptr, 0);
		if (lastReturnValue.type == GFxValue::VT_Boolean)
			canContinue = lastReturnValue.boolean;

		externalInterface->Callback(this, "IsNewGamePlusAvailable", nullptr, 0);
		if (lastReturnValue.type == GFxValue::VT_Boolean)
			isNewGamePlusAvailable = lastReturnValue.boolean;

		externalInterface->Callback(this, "CanPlayCredits", nullptr, 0);
		if (lastReturnValue.type == GFxValue::VT_Boolean)
			canPlayCredits = lastReturnValue.boolean;
	}

	void buildUI() {
		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		auto newGamePlusSize = ImGui::CalcTextSize("New Game +");
		newGamePlusSize.x *= 1.5f;
		newGamePlusSize.y = 0.0f;
		ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
		ImGui::SetNextWindowSize(newGamePlusSize, ImGuiCond_Always);

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetCenter(), ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (visible && ImGui::Begin("Menu", &visible, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration)) {

			if (canContinue && ImGui::Button("Continue", ImVec2(-1.0f, 0.0f))) {
				externalInterface->Callback(this, "LoadLatestSavedGame", nullptr, 0);
				visible = false;
			}

			if (isNewGamePlusAvailable && ImGui::Button("New Game +", ImVec2(-1.0f, 0.0f))) {
				externalInterface->Callback(this, "LoadNewGamePlus", nullptr, 0);
				visible = false;
			}

			if (ImGui::Button("New Game", ImVec2(-1.0f, 0.0f))) {
				externalInterface->Callback(this, "StartNewGame", nullptr, 0);
				visible = false;
			}

			if (ImGui::Button("Load", ImVec2(-1.0f, 0.0f)))
				/*TODO*/;

			if (ImGui::Button("Options", ImVec2(-1.0f, 0.0f)))
				/*TODO*/;

			if (ImGui::Button("Tutorials", ImVec2(-1.0f, 0.0f)))
				/*TODO*/;

			if (canPlayCredits && ImGui::Button("Credits", ImVec2(-1.0f, 0.0f)))
				externalInterface->Callback(this, "StartCredits", nullptr, 0);

			if (ImGui::Button("Extra", ImVec2(-1.0f, 0.0f)))
				/*TODO*/;

			if (ImGui::Button("Exit Game", ImVec2(-1.0f, 0.0f)))
				externalInterface->Callback(this, "ExitGame", nullptr, 0);

			ImGui::End();
		}
	}
};

// boilerplate

GFxMovieView *createMenuMovieView() { return new MainMenu(); }
GFxMovieDefImpl mainMenuMovieDef(createMenuMovieView);
ScaleformMovie mainMenuMovie(&mainMenuMovieDef);
