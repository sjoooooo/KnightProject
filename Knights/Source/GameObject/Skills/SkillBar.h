#ifndef _SKILLBAR_H_
#define _SKILLBAR_H_

#include "Skill.h"

#include "cocos2d.h"
USING_NS_CC;

class Skillbar : public Node
{
private:
	// Background
	Sprite* m_background;
	// Skills
	std::vector<Skill*> m_skillList;
	// Moving skillbar
	float m_currentOffsetX;
	float m_maxOffsetX;
public:
	Skillbar();
	~Skillbar();

	// Setters
	void SetCurrentOffsetX(float currentOffsetX);
	void SetMaxOffsetX(float maxOffsetX);

	// Getters
	float GetCurrentOffsetX();
	float GetMaxOffsetX();
	Sprite* GetBackground();

	// Others
	void Init(Size* skillBarSize, Vec2* Position, float spacing);

	void MoveSkillsX(float moveby);
	void ContraintsMovedSkillsX();
	bool IfSkillsMoving();

	Skill* TouchedSkill(Point* location);
};

#endif 
