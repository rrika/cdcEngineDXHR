target_sources(dxhr PRIVATE
	CellStreamPrediction.cpp
	GameTracker.cpp
	Instance.cpp
	InstanceManager.cpp
	RenderLayer.cpp
	RMIDrawableBase.cpp
	SceneLayer.cpp
	stream.cpp
	StreamUnit.cpp)

if (ENABLE_IMGUI)
	target_sources(dxhr PRIVATE
		Inspector.cpp)
endif()
