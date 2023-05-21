target_sources(dxhr PRIVATE
	Menu.cpp
	FakeScaleform/fakescaleform.cpp
	Scaleform/ScaleformManager.cpp
	Scaleform/ScaleformMovieInstance.cpp
	ui.cpp)

if (ENABLE_IMGUI)
	target_sources(dxhr PRIVATE
		FakeScaleform/mainmenu.cpp)
endif()
