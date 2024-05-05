#pragma once
#include "SceneEntity.h"
#include "cdcScene.h"

namespace cdc {

class SceneLight :
	public SceneEntity,
	public ISceneLight
{
	
public:
	// this class inherits ISceneEntity twice!
	// 25 methods from SceneEntity
	// 41 methods from ISceneLight
};

}
