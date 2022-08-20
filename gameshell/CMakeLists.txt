if (NATIVE_BUILD)
	target_sources(dxhr PRIVATE
		linux/MainVM.cpp)
else()
	target_sources(dxhr PRIVATE
		win32/MainVM.cpp)
endif()
