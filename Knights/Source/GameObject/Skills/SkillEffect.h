#ifndef _SKILL_EFFECT_H_
#define _SKILL_EFFECT_H_

#include "cocos2d.h"
USING_NS_CC;

class SkillEffect : public Node
{
private:
	Sprite* m_sprite;
	bool m_active;
	float m_timeToDamage;
	int m_damage;
public:
	SkillEffect();
	~SkillEffect();

	enum TAG
	{
		ANIMATION = 1,
		MOVE,
	};

	// Setters
	void Init();
	void SetActive(bool active);
	void SetTimeToDamage(float timeToDamage);
	void SetDamage(int damage);

	// Getters
	Sprite* GetSprite();
	bool IfActive();
	float GetTimeToDamage();
	int GetDamage();

	void Stop();

	// Others
	void Activate(Vec2 targetPosition, Size size);
};

#endif 
