target_sources(dxhr PRIVATE
	DeferredRenderingObject.cpp
	DX3Player.cpp
	Gameloop.cpp
	Impulse.cpp
	LensFlareAndCoronaID.cpp
	Main.cpp
	ObjectiveManager.cpp
	PackedVector.cpp
	objects/objects.cpp
	objects/UberObject_DX3.cpp)

add_subdirectory(script/game)
add_subdirectory(ui)
