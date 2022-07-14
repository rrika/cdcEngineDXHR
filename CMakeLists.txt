cmake_minimum_required(VERSION 3.22)

set(triple i386-pc-windows-msvc)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_LINKER lld-link) # ignored for C / CXX
set(CMAKE_RC_COMPILER llvm-rc)
ENABLE_LANGUAGE(RC)

project(dxhr)

option(ENABLE_STEAM "Build with Steam integration" OFF)
option(ENABLE_MCE "Build with Windows Media Center integration" OFF)
option(ENABLE_GCS "Build with whatever GCS is" OFF)
option(ENABLE_IMGUI "Build with ImGui" ON)

add_compile_options(-g -target ${triple}
	-Wno-ignored-pragma-intrinsic
	-Wno-nonportable-include-path
	-Wno-ignored-attributes
	-Wno-pragma-pack
	-Wno-deprecated-declarations)
add_link_options(-g -target ${triple} -fuse-ld=lld-link)
add_executable(dxhr
	mainloop.cpp
	matrix.cpp
	spinnycube.cpp)
configure_file(config.h.in config/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/config)
include_directories(SYSTEM
	../winsdk/crt/include
	../winsdk/sdk/include/ucrt
	../winsdk/sdk/include/um
	../winsdk/sdk/include/shared)
# cmake will try pass -rpath to the linker, ignoring that it's in msvc link mode
set(CMAKE_SKIP_RPATH TRUE)
target_link_directories(dxhr PRIVATE
	../winsdk/crt/lib/x86
	../winsdk/sdk/lib/um/x86
	../winsdk/sdk/lib/ucrt/x86)
target_link_libraries(dxhr PRIVATE
	-luser32 -ld3d11 -ld3dcompiler)
set_target_properties(dxhr PROPERTIES
	OUTPUT_NAME dxhr.exe
	CXX_STANDARD 17)

add_subdirectory(allocator)
add_subdirectory(camera)
add_subdirectory(drm)
add_subdirectory(filesystem)
add_subdirectory(gameshell)
add_subdirectory(input)
add_subdirectory(miniz)
add_subdirectory(rendering)
add_subdirectory(scene)
add_subdirectory(sound)
add_subdirectory(sys)

target_sources(dxhr PRIVATE
	res/resources.rc)

if (ENABLE_IMGUI)
	include_directories(imgui)
	target_sources(dxhr PRIVATE
		imgui/imgui.cpp
		imgui/imgui_draw.cpp
		imgui/imgui_tables.cpp
		imgui/imgui_widgets.cpp
		imgui/backends/imgui_impl_dx11.cpp
		imgui/backends/imgui_impl_win32.cpp)

	# clang-cl doesn't find _mm_set_ss
	add_compile_definitions(
		IMGUI_DISABLE_SSE=1)

	set_property(
		SOURCE imgui/imgui_draw.cpp
		PROPERTY COMPILE_DEFINITIONS
		_CRT_USE_BUILTIN_OFFSETOF=1)
endif()
