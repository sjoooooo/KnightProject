#ifndef _SKILL_H_
#define _SKILL_H_

#include "..\Spawner\Spawner.h"
#include "SkillEffect.h"

#include "cocos2d.h"
USING_NS_CC;

class Skill : public ProgressTimer
{
private:
	float m_cost;
	float m_coolDown;
	Unit::TYPE m_type;
	bool m_boughtItem;
public:
	Skill();
	~Skill();

	enum TYPE
	{
		ARCHER = 0,
		ARROW,
		BACKSTEP,
		SPEARMAN,
		SWORDSMAN,
		LIGHTNING,
		SHIELD,
		METEOR,
		WIZARD,
		ICE_SPEAR,
		HURRICANE,
		HORSEMAN,
		HEAL,
		RECOVERY_POTION,
		FIREBALL,
		NUM_OF_TYPES,
	};


	// Others
	void Summon(Size* size, Vec2* targetPosition, Spawner* spawner);
	void Cast(Unit* hero, SkillEffect* skillEffect = NULL, Vec2* targetPos = NULL, Size* size = NULL);
	void CalculateCost();
	void CalculateUnitType();
	void CalculateCooldown();
	void CalculateInfo(int tag);
	float GetCost();
};

#endif 
