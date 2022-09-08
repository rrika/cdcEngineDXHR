if (NATIVE_BUILD)
	target_sources(dxhr PRIVATE
		linux/MainVM.cpp
		MainG2.cpp)
else()
	target_sources(dxhr PRIVATE
		win32/MainVM.cpp
		MainG2.cpp)
endif()
