target_sources(dxhr PRIVATE
	DeferredRenderingObject.cpp
	Gameloop.cpp
	Main.cpp
	objects/objects.cpp)

add_subdirectory(script/game)
add_subdirectory(ui)
