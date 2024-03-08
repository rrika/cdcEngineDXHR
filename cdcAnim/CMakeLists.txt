target_sources(dxhr PRIVATE
	AnimComponentV2.cpp
	AnimDecoder.cpp
	AnimFragment.cpp
	AnimPipelineNode.cpp
	AnimPose.cpp
	AnimNodes/AnimFragmentNode.cpp)

if (ENABLE_IMGUI)
	target_sources(dxhr PRIVATE
		Inspector.cpp)
endif()
