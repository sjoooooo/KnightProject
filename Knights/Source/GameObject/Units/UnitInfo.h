#ifndef _UNIT_INFO_H_
#define _UNIT_INFO_H_

#include "cocos2d.h"
USING_NS_CC;

class UnitInfo
{
protected:
	// Health Info
	int m_health;
	int m_maxHealth;
	int m_regeneration;
	// Attack Info
	int m_attackDamage;
	float m_attackSpeed;
	float m_attackRange;
	// Defense
	int m_defense;
	// Movement Info
	float m_movementSpeed;
public:
	UnitInfo();
	UnitInfo(	int health, int maxHealth, int regeneration,
				int m_attackDamage, float m_attackSpeed, float m_attackRange,
				int defense,
				float m_movementSpeed);
	virtual ~UnitInfo();

	// Setters
	void SetHealth(int health);
	void SetMaxHealth(int maxHealth);
	void SetRegeneration(int regeneration);
	void SetAttackDamage(int attackDamage);
	void SetAttackSpeed(float attackSpeed);
	void SetAttackRange(float attackRange);
	void SetDefense(int defense);
	void SetMovementSpeed(float movementSpeed);
	void SetInfo(	int health, int maxHealth, int regeneration,
					int attackDamage, float attackSpeed, float attackRange,
					int defense,
					float movementSpeed);

	// Getters
	int GetHealth();
	int GetMaxHealth();
	int GetRegeneration();
	int GetAttackDamage();
	float GetAttackSpeed();
	float GetAttackRange();
	int GetDefense();
	float GetMovementSpeed();
};

#endif 
