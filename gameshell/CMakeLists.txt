if (NOT NATIVE_BUILD)
	target_sources(dxhr PRIVATE
		win32/MainVM.cpp)
endif()
