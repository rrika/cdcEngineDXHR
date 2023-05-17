target_sources(dxhr PRIVATE
	DeferredRenderingObject.cpp
	Gameloop.cpp
	LensFlareAndCoronaID.cpp
	Main.cpp
	PackedVector.cpp
	objects/objects.cpp)

add_subdirectory(script/game)
add_subdirectory(ui)
