#include "SpawnPattern.h"

SpawnPattern::SpawnPattern(	float timeToSpawn,
							int amountToSpawn,
							int type,
							Size size,
							Vec2 targetLocation)
	: m_timeToSpawn(timeToSpawn)
	, m_amountToSpawn(amountToSpawn)
	, m_type(type)
	, m_size(size)
	, m_targetLocation(targetLocation)
{
}

SpawnPattern::~SpawnPattern()
{
}

// Getters
float SpawnPattern::GetTimeToSpawn()
{
	return m_timeToSpawn;
}
int SpawnPattern::GetAmountToSpawn()
{
	return m_amountToSpawn;
}
int SpawnPattern::GetType()
{
	return m_type;
}
Size SpawnPattern::GetSize()
{
	return m_size;
}
Vec2 SpawnPattern::GetTargetLocation()
{
	return m_targetLocation;
}