target_sources(dxhr PRIVATE
	Actor/InventoryPlayer.cpp

	Conversation/BriefingManager.cpp
	Conversation/ConversationActor.cpp
	Conversation/ConversationBase.cpp
	Conversation/ConversationBriefing.cpp
	Conversation/ConversationGraphNodes.cpp
	Conversation/ConversationManager.cpp

	DeferredRenderingObject.cpp
	DX3Player.cpp
	Gameloop.cpp
	GlobalParamManager.cpp
	Impulse.cpp
	LensFlareAndCoronaID.cpp
	Main.cpp
	ObjectiveManager.cpp
	PackedVector.cpp
	objects/objects.cpp
	objects/UberObject_DX3.cpp)

add_subdirectory(script/game)
add_subdirectory(ui)
