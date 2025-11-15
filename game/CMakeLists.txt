target_sources(dxhr PRIVATE
	Actor/InventoryPlayer.cpp

	Billboard.cpp
	DeferredRenderingObject.cpp
	DistanceFade.cpp
	DX3Player.cpp
	Gameloop.cpp
	GlobalParamManager.cpp
	Impulse.cpp
	LensFlareAndCoronaID.cpp
	LogicInput.cpp
	Main.cpp
	ObjectiveManager.cpp
	PackedVector.cpp
	objects/objects.cpp
	objects/UberObject_DX3.cpp)

add_subdirectory(script/game)
add_subdirectory(ui)
