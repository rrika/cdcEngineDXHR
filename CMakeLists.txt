cmake_minimum_required(VERSION 3.20)

option(NATIVE_BUILD "Target linux using dxvk" OFF)

if (NATIVE_BUILD)
	set(triple i386-pc-linux-gnu)
	set(NOT_NATIVE_BUILD OFF)
elseif(CMAKE_HOST_WIN32)
	set(NOT_NATIVE_BUILD ON)
else()
	set(triple i386-pc-windows-msvc)
	set(NOT_NATIVE_BUILD ON)

	set(CMAKE_LINKER lld-link) # ignored for C / CXX
	set(CMAKE_RC_COMPILER llvm-rc)
	ENABLE_LANGUAGE(RC)
endif()

if (CMAKE_HOST_WIN32)
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

option(ENABLE_ASAN "Build with address sanitizer" OFF)

configure_file(res/DXHRDC.shad DXHRDC.shad COPYONLY)
configure_file(res/DX11SimplePrimVS3D.shaderlib DX11SimplePrimVS3D.shaderlib COPYONLY)
configure_file(res/dtpdata.ids dtpdata.ids COPYONLY)

if (NATIVE_BUILD)
	add_compile_options(-g)

	if (ENABLE_ASAN)
		add_compile_options(-fsanitize=address)
		add_link_options(-fsanitize=address)
	endif()

	add_executable(dxhr
		spinnycube.cpp)

	ExternalProject_Add(dxvk-native
		GIT_REPOSITORY https://github.com/doitsujin/dxvk.git
		GIT_TAG v2.2
		CONFIGURE_COMMAND meson setup <SOURCE_DIR> --cross-file ${CMAKE_CURRENT_SOURCE_DIR}/dxvk-build-linux32.txt -Dcpp_std=c++20
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
		CXX_STANDARD 20)

	target_link_directories(dxhr PRIVATE
		3rdParty/fmod)
	target_link_libraries(dxhr PRIVATE
		fmodex)

elseif (MSVC)
	add_executable(dxhr WIN32
		spinnycube.cpp)
	target_link_libraries(dxhr PRIVATE
		user32 advapi32 d3d11)

	if (ENABLE_D3DCOMPILER)
		target_link_libraries(dxhr PRIVATE
			d3dcompiler)
	endif()

	set_target_properties(dxhr PROPERTIES
		OUTPUT_NAME dxhr
		CXX_STANDARD 20)

	target_sources(dxhr PRIVATE
		res/resources.rc)

	target_link_directories(dxhr PRIVATE
		3rdParty/fmod)
	target_link_libraries(dxhr PRIVATE
		fmodex_vc)
	file(COPY 3rdParty/fmod/fmodex.dll
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
		-luser32 -ladvapi32 -ld3d11)
	if (ENABLE_D3DCOMPILER)
		target_link_libraries(dxhr PRIVATE
			-ld3dcompiler)
	endif()
	set_target_properties(dxhr PROPERTIES
		OUTPUT_NAME dxhr.exe
		CXX_STANDARD 20)
	target_sources(dxhr PRIVATE
		res/resources.rc)

	target_link_directories(dxhr PRIVATE
		3rdParty/fmod)
	target_link_libraries(dxhr PRIVATE
		-lfmodex_vc)
	file(COPY 3rdParty/fmod/fmodex.dll
		DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()

configure_file(config.h.in config/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/config)

include_directories(.)
include_directories(3rdParty)
include_directories(eventually_generated)

add_subdirectory(3rdParty/miniz)
add_subdirectory(cdcAnim)
add_subdirectory(camera)
add_subdirectory(eventually_generated)
add_subdirectory(cdcFile)
add_subdirectory(game)
add_subdirectory(cdcGameShell)
add_subdirectory(input)
add_subdirectory(cdcKit/Animation)
add_subdirectory(cdcLocale)
add_subdirectory(cdcMath)
add_subdirectory(cdcMulticore)
add_subdirectory(cdcObjects)
add_subdirectory(postprocessing)
add_subdirectory(cdcRender)
add_subdirectory(cdcResource)
add_subdirectory(cdcScene)
add_subdirectory(cdcScript)
add_subdirectory(cdcSound)
add_subdirectory(cdcSys)
add_subdirectory(cdcWorld)

if (ENABLE_IMGUI)
	include_directories(3rdParty/imgui)
	target_sources(dxhr PRIVATE
		3rdParty/imgui/imgui.cpp
		3rdParty/imgui/imgui_draw.cpp
		3rdParty/imgui/imgui_tables.cpp
		3rdParty/imgui/imgui_widgets.cpp
		3rdParty/imgui/backends/imgui_impl_dx11.cpp)

	if (NATIVE_BUILD)
		target_sources(dxhr PRIVATE
			3rdParty/imgui/backends/imgui_impl_sdl.cpp)
	else ()
		target_sources(dxhr PRIVATE
			3rdParty/imgui/backends/imgui_impl_win32.cpp)
	endif()

	include_directories(3rdParty/imguizmo)
	target_sources(dxhr PRIVATE
		3rdParty/imguizmo/ImGuizmo.cpp)

	# clang-cl doesn't find _mm_set_ss
	add_compile_definitions(
		IMGUI_DISABLE_SSE=1)

	set_property(
		SOURCE 3rdParty/imgui/imgui_draw.cpp
		PROPERTY COMPILE_DEFINITIONS
		_CRT_USE_BUILTIN_OFFSETOF=1)
endif()
