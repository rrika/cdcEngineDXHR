cmake_minimum_required(VERSION 3.22)

set(CMAKE_C_COMPILER winegcc)
set(CMAKE_CXX_COMPILER wineg++)

project(dxhr)

option(ENABLE_STEAM "Build with Steam integration" OFF)
option(ENABLE_MCE "Build with Windows Media Center integration" OFF)
option(ENABLE_GCS "Build with whatever GCS is" OFF)

add_compile_options(-mno-cygwin -m32)
add_link_options(-mwindows -mno-cygwin -m32)
add_executable(dxhr
	main.cpp
	main2.cpp)
configure_file(config.h.in config/config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/config)
target_link_libraries(dxhr PRIVATE
	-ld3d11 -ld3dcompiler)
