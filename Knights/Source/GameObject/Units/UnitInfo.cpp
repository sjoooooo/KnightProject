#include "UnitInfo.h"

UnitInfo::UnitInfo()
	: m_health(0), m_maxHealth(0), m_regeneration(0)
	, m_attackDamage(0) , m_attackSpeed(0) , m_attackRange(0)
	, m_defense(0)
	, m_movementSpeed(0)
{
}

UnitInfo::UnitInfo(	int health, int maxHealth, int regeneration,
					int attackDamage, float attackSpeed, float attackRange,
					int defense,
					float movementSpeed)
	: m_health(health), m_maxHealth(maxHealth), m_regeneration(regeneration)
	, m_attackDamage(attackDamage) , m_attackSpeed(attackSpeed) , m_attackRange(attackRange)
	, m_defense(defense)
	, m_movementSpeed(movementSpeed)
{
}

UnitInfo::~UnitInfo()
{
}

// Setters
void UnitInfo::SetHealth(int health)
{
	m_health = health;
}
void UnitInfo::SetMaxHealth(int maxHealth)
{
	m_maxHealth = maxHealth;
}
void UnitInfo::SetRegeneration(int regeneration)
{
	m_regeneration = regeneration;
}
void UnitInfo::SetAttackDamage(int attackDamage)
{
	m_attackDamage = attackDamage;
}
void UnitInfo::SetAttackSpeed(float attackSpeed)
{
	m_attackSpeed = attackSpeed;
}
void UnitInfo::SetAttackRange(float attackRange)
{
	m_attackRange = attackRange;
}
void UnitInfo::SetDefense(int defense)
{
	m_defense = defense;
}
void UnitInfo::SetMovementSpeed(float movementSpeed)
{
	m_movementSpeed = movementSpeed;
}
void UnitInfo::SetInfo(	int health, int maxHealth, int regeneration,
						int attackDamage, float attackSpeed, float attackRange,
						int defense,
						float movementSpeed)
{
	m_health = health;
	m_maxHealth = maxHealth;
	m_regeneration = regeneration;
	m_attackDamage = attackDamage;
	m_attackSpeed = attackSpeed;
	m_attackRange = attackRange;
	m_defense = defense;
	m_movementSpeed = movementSpeed;
}

// Getters
int UnitInfo::GetHealth()
{
	return m_health;
}
int UnitInfo::GetMaxHealth()
{
	return m_maxHealth;
}
int UnitInfo::GetRegeneration()
{
	return m_regeneration;
}
int UnitInfo::GetAttackDamage()
{
	return m_attackDamage;
}
float UnitInfo::GetAttackSpeed()
{
	return m_attackSpeed;
}
float UnitInfo::GetAttackRange()
{
	return m_attackRange;
}
int UnitInfo::GetDefense()
{
	return m_defense;
}
float UnitInfo::GetMovementSpeed()
{
	return m_movementSpeed;
}