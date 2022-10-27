cmake_minimum_required(VERSION 3.20)

option(NATIVE_BUILD "Target linux using dxvk" OFF)

if (NATIVE_BUILD)
	set(triple i386-pc-linux-gnu)
	set(NOT_NATIVE_BUILD OFF)
elseif(MSVC)
	set(NOT_NATIVE_BUILD ON)
else()
	set(triple i386-pc-windows-msvc)
	set(NOT_NATIVE_BUILD ON)

	set(CMAKE_LINKER lld-link) # ignored for C / CXX
	set(CMAKE_RC_COMPILER llvm-rc)
	ENABLE_LANGUAGE(RC)
endif()

if (MSVC)
else()
	set(CMAKE_C_COMPILER clang)
	set(CMAKE_C_COMPILER_TARGET ${triple})
	set(CMAKE_CXX_COMPILER clang++)
	set(CMAKE_CXX_COMPILER_TARGET ${triple})
endif()

project(dxhr)

include(ExternalProject)

option(ENABLE_STEAM "Build with Steam integration" OFF)
option(ENABLE_MCE "Build with Windows Media Center integration" OFF)
option(ENABLE_GCS "Build with whatever GCS is" OFF)
option(ENABLE_IMGUI "Build with ImGui" ON)
option(ENABLE_D3DCOMPILER "Build with d3dcompiler dependency" ${NOT_NATIVE_BUILD})

if (NATIVE_BUILD)
	add_compile_options(-g)

	add_executable(dxhr
		spinnycube.cpp)

	ExternalProject_Add(dxvk-native
		GIT_REPOSITORY https://github.com/Joshua-Ashton/dxvk-native.git
		GIT_TAG native-1.9.2a
		PATCH_COMMAND patch -f -p1 -i ${CMAKE_CURRENT_SOURCE_DIR}/dxvk-meson.patch || true
		CONFIGURE_COMMAND meson setup <SOURCE_DIR> --cross-file ${CMAKE_CURRENT_SOURCE_DIR}/dxvk-build-linux32.txt
		BUILD_COMMAND ninja src/d3d11/libdxvk_d3d11.so
		INSTALL_COMMAND "")

	ExternalProject_Get_property(dxvk-native SOURCE_DIR BINARY_DIR)
	set(DXVK_SOURCE_DIR ${SOURCE_DIR}) # build/dxvk-native-prefix/src/dxvk-native/
	set(DXVK_BINARY_DIR ${BINARY_DIR}) # build/dxvk-native-prefix/src/dxvk-native-build/
	unset(SOURCE_DIR)
	unset(BINARY_DIR)
	add_dependencies(dxhr dxvk-native)

	include_directories(SYSTEM
		${DXVK_SOURCE_DIR}/include/native/directx
		${DXVK_SOURCE_DIR}/include/native/windows)
	target_link_directories(dxhr PRIVATE
		${DXVK_BINARY_DIR}/src/d3d11)
	target_link_libraries(dxhr PRIVATE
		dxvk_d3d11 SDL2)
	set_target_properties(dxhr PROPERTIES
		CXX_STANDARD 17)

	target_link_directories(dxhr PRIVATE
		fmod)
	target_link_libraries(dxhr PRIVATE
		fmodex)

elseif (MSVC)
	configure_file(DXHRDC.shad DXHRDC.shad COPYONLY)

	add_executable(dxhr WIN32
		spinnycube.cpp)
	target_link_libraries(dxhr PRIVATE
		user32 d3d11)

	if (ENABLE_D3DCOMPILER)
		target_link_libraries(dxhr PRIVATE
			d3dcompiler)
	endif()

	set_target_properties(dxhr PROPERTIES
		OUTPUT_NAME dxhr
		CXX_STANDARD 17)

	target_sources(dxhr PRIVATE
		res/resources.rc)

	target_link_directories(dxhr PRIVATE
		fmod)
	target_link_libraries(dxhr PRIVATE
		fmodex_vc)
	file(COPY fmod/fmodex.dll
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

else()
	add_compile_options(-g)

	add_compile_options(-target ${triple}
		-Wno-ignored-pragma-intrinsic
		-Wno-nonportable-include-path
		-Wno-ignored-attributes
		-Wno-pragma-pack
		-Wno-deprecated-declarations)
	add_link_options(-g -target ${triple} -fuse-ld=lld-link)
	add_executable(dxhr
		spinnycube.cpp)
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
		-luser32 -ld3d11)
	if (ENABLE_D3DCOMPILER)
		target_link_libraries(dxhr PRIVATE
			-ld3dcompiler)
	endif()
	set_target_properties(dxhr PROPERTIES
		OUTPUT_NAME dxhr.exe
		CXX_STANDARD 17)
	target_sources(dxhr PRIVATE
		res/resources.rc)

	target_link_directories(dxhr PRIVATE
		fmod)
	target_link_libraries(dxhr PRIVATE
		-lfmodex_vc)
	file(COPY fmod/fmodex.dll
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

configure_file(config.h.in config/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/config)

include_directories(eventually_generated)

add_subdirectory(allocator)
add_subdirectory(animation)
add_subdirectory(camera)
add_subdirectory(drm)
add_subdirectory(filesystem)
add_subdirectory(game)
add_subdirectory(gameshell)
add_subdirectory(input)
add_subdirectory(locale)
add_subdirectory(math)
add_subdirectory(miniz)
add_subdirectory(object)
add_subdirectory(postprocessing)
add_subdirectory(rendering)
add_subdirectory(scene)
add_subdirectory(sound)
add_subdirectory(sys)
add_subdirectory(world)

if (ENABLE_IMGUI)
	include_directories(imgui)
	target_sources(dxhr PRIVATE
		imgui/imgui.cpp
		imgui/imgui_draw.cpp
		imgui/imgui_tables.cpp
		imgui/imgui_widgets.cpp
		imgui/backends/imgui_impl_dx11.cpp)

	if (NATIVE_BUILD)
		target_sources(dxhr PRIVATE
			imgui/backends/imgui_impl_sdl.cpp)
	else ()
		target_sources(dxhr PRIVATE
			imgui/backends/imgui_impl_win32.cpp)
	endif()

	# clang-cl doesn't find _mm_set_ss
	add_compile_definitions(
		IMGUI_DISABLE_SSE=1)

	set_property(
		SOURCE imgui/imgui_draw.cpp
		PROPERTY COMPILE_DEFINITIONS
		_CRT_USE_BUILTIN_OFFSETOF=1)
endif()
