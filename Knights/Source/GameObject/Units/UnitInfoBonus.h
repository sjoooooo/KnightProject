#ifndef _UNIT_INFO_BONUS_H_
#define _UNIT_INFO_BONUS_H_

#include "UnitInfo.h"

#include "cocos2d.h"
USING_NS_CC;

class UnitInfoBonus : public UnitInfo
{
protected:
	// Timer
	float m_timeLeft;
	// Weapon
	bool m_gotWeapon;
public:
	UnitInfoBonus() : UnitInfo(){}
	UnitInfoBonus(	int health, int maxHealth, int regeneration,
					int attackDamage, float attackSpeed, float attackRange,
					int defense,
					float movementSpeed,
					float timeLeft,
					bool gotWeapon);
	virtual ~UnitInfoBonus();

	// Setters
	void SetTimeLeft(float timeLeft);
	void SetGotWeapon(bool gotWeapon);
	void SetBonus(	float timeLeft,
					bool gotWeapon);

	// Getters
	float GetTimeLeft();
	bool GotWeapon();
};

#endif 
