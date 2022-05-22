cmake_minimum_required(VERSION 3.22)

set(triple i386-pc-windows-msvc)
set(CMAKE_C_COMPILER clang)
set(CMAKE_C_COMPILER_TARGET ${triple})
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_COMPILER_TARGET ${triple})
set(CMAKE_LINKER lld-link) # ignored for C / CXX

project(dxhr)

option(ENABLE_STEAM "Build with Steam integration" OFF)
option(ENABLE_MCE "Build with Windows Media Center integration" OFF)
option(ENABLE_GCS "Build with whatever GCS is" OFF)

add_compile_options(-g -target ${triple}
	-Wno-ignored-pragma-intrinsic
	-Wno-nonportable-include-path
	-Wno-ignored-attributes
	-Wno-pragma-pack
	-Wno-deprecated-declarations)
add_link_options(-g -target ${triple} -fuse-ld=lld-link)
add_executable(dxhr
	main.cpp
	main2.cpp
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
add_subdirectory(input)
add_subdirectory(miniz)
add_subdirectory(rendering)
add_subdirectory(scene)
