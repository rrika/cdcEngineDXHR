#pragma once
#include "SceneEntity.h"
#include "ISceneLight.h"

namespace cdc {

class SceneLight :
	public SceneEntity,
	public ISceneLight
{
	// this class inherits ISceneEntity twice!
	// 25 methods from SceneEntity
	// 41 methods from ISceneLight
};

}
