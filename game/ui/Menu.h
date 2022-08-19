#pragma once

namespace df {

class IMenu;

class IMenuManager {
public:
	// virtual void method_0() = 0;
	// virtual void method_4() = 0;
	// virtual bool method_8_hasMenus() = 0;
	// virtual void method_C_eachMenu_method_14() = 0;
	// virtual void clearMenus() = 0;
	// virtual uint32_t pushMenu(df_IMenu *) = 0;
	// virtual void popMenu() = 0;
	// virtual void setSpecialMenu(df_IMenu *) = 0;
	// virtual void method_20() = 0;
	// virtual void method_24_eachMenu_method_24() = 0;
	// virtual void method_28_eachMenu_method_28() = 0;
};

class IMenu {
public:
	// virtual void method_0() = 0;
	// virtual void method_4() = 0;
	// virtual void method_8() = 0;
	// virtual void method_C_wantsToBeDeleted() = 0;
	// virtual void method_10() = 0;
	// virtual void method_14() = 0;
	// virtual void addedToManager() = 0;
	// virtual void method_1C() = 0;
	// virtual void method_20() = 0;
	// virtual void method_24() = 0;
	// virtual void method_28() = 0;
};

}

class DefaultMenuManager : public df::IMenuManager {
	// TODO
};

class TitleMenu : public df::IMenu {
	// TODO
};

class PauseMenu : public df::IMenu {
	// TODO
};

class EndingMenu : public df::IMenu {
	// TODO
};

class MakingOfMenu : public df::IMenu {
	// TODO
};
