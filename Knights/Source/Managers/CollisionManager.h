#ifndef _COLLISION_MANAGER_H_
#define _COLLISION_MANAGER_H_

#include "UnitManager.h"
#include "SkillEffectManager.h"

#include "cocos2d.h"
USING_NS_CC;

class CollisionManager
{
public:
	CollisionManager();
	~CollisionManager();

	// Others
	void CheckCollision(UnitManager* unit_manager = NULL, SkillEffectManager* skillEffect_manager = NULL, bool bossCaptureScene = false);
};

#endif 
