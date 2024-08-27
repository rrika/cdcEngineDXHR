target_sources(dxhr PRIVATE
	CalcSkeleton.cpp
	CellStreamPrediction.cpp
	GameTracker.cpp
	Instance.cpp
	InstncG2.cpp
	InstanceDrawable.cpp
	InstanceManager.cpp
	Intro.cpp
	RenderLayer.cpp
	RMIDrawableBase.cpp
	SceneLayer.cpp
	stream.cpp
	StreamUnit.cpp
	UserDataComponent.cpp)

if (ENABLE_IMGUI)
	target_sources(dxhr PRIVATE
		Inspector.cpp)
endif()
