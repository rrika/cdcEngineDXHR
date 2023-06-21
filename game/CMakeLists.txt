target_sources(dxhr PRIVATE
	Animation/AnimNodes/AnimDynamicFragmentNode.cpp
	DeferredRenderingObject.cpp
	Gameloop.cpp
	Impulse.cpp
	LensFlareAndCoronaID.cpp
	Main.cpp
	PackedVector.cpp
	objects/objects.cpp
	objects/UberObject_DX3.cpp)

add_subdirectory(script/game)
add_subdirectory(ui)
