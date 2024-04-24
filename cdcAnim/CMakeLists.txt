target_sources(dxhr PRIVATE
	AnimDecoder.cpp
	AnimPipelineNode.cpp
	AnimNodes/AnimFragmentNode.cpp)

if (ENABLE_IMGUI)
	target_sources(dxhr PRIVATE
		Inspector.cpp)
endif()
