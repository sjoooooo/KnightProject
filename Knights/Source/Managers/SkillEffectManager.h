#ifndef _SKILL_EFFECT_MANAGER_H_
#define _SKILL_EFFECT_MANAGER_H_

#include "..\GameObject\Skills\SkillEffect.h"

#include "cocos2d.h"
USING_NS_CC;

class SkillEffectManager
{
private:
	std::vector<SkillEffect*> m_skillEffectList;
public:
	SkillEffectManager();
	~SkillEffectManager();

	// Setters
	void AddSkillEffect(SkillEffect* skillEffect);

	// Getters
	std::vector<SkillEffect*>* GetSkillEffectList();
	SkillEffect* GetInactiveSkillEffect();

	// Others
	void Update(float dt);
};

#endif 
