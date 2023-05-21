target_sources(dxhr PRIVATE
	Menu.cpp
	FakeScaleform/fakescaleform.cpp
	NsPopupRequest.cpp
	Scaleform/ScaleformManager.cpp
	Scaleform/ScaleformMovieInstance.cpp
	ui.cpp)

if (ENABLE_IMGUI)
	target_sources(dxhr PRIVATE
		FakeScaleform/mainmenu.cpp
		FakeScaleform/popups.cpp)
endif()
