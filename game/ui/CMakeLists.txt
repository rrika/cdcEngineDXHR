target_sources(dxhr PRIVATE
	Menu.cpp
	FakeScaleform/fakescaleform.cpp
	Scaleform/ScaleformMovieInstance.cpp)

if (ENABLE_IMGUI)
	target_sources(dxhr PRIVATE
		FakeScaleform/mainmenu.cpp)
endif()
