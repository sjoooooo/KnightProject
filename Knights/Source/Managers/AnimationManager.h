#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

#include "cocos2d.h"
USING_NS_CC;

class AnimationManager
{
public:
	AnimationManager();
	~AnimationManager();

	// Getters
	static Animate* GetWalkingAnimation(int tag, float speed);
	static Animate* GetAttackingAnimation(int tag, float speed);
	static Animate* GetAttackingEffectAnimation(int tag, float speed);
	static Animate* GetIdlingAnimation(int tag, float speed);
	static Animate* GetSkillAnimation(int tag, float speed);
};

#endif 
