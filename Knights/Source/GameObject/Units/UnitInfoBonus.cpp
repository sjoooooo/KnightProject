#include "UnitInfoBonus.h"

UnitInfoBonus::UnitInfoBonus(	int health, int maxHealth, int regeneration,
								int attackDamage, float attackSpeed, float attackRange,
								int defense,
								float movementSpeed,
								float timeLeft,
								bool gotWeapon)
								 : UnitInfo(	health, maxHealth, regeneration,
												attackDamage, attackSpeed, attackRange,
												defense,
												movementSpeed)
	, m_timeLeft(timeLeft)
	, m_gotWeapon(gotWeapon)
{
}

UnitInfoBonus::~UnitInfoBonus()
{
}

// Setters
void UnitInfoBonus::SetTimeLeft(float timeLeft)
{
	m_timeLeft = timeLeft;
}
void UnitInfoBonus::SetGotWeapon(bool gotWeapon)
{
	m_gotWeapon = gotWeapon;
}
void UnitInfoBonus::SetBonus(	float timeLeft,
								bool gotWeapon)
{
	m_timeLeft = timeLeft;
	m_gotWeapon = gotWeapon;
}

// Getters
float UnitInfoBonus::GetTimeLeft()
{
	return m_timeLeft;
}
bool UnitInfoBonus::GotWeapon()
{
	return m_gotWeapon;
}