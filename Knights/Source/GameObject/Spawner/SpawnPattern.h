#ifndef _SPAWN_PATTERN_H_
#define _SPAWN_PATTERN_H_

#include "Spawner.h"
#include "..\Units\Unit.h"

#include "cocos2d.h"
USING_NS_CC;

class SpawnPattern : public Node
{
private:
	float m_timeToSpawn;
	int m_amountToSpawn;
	int m_type;
	Size m_size;
	Vec2 m_targetLocation;
public:
	SpawnPattern(	float timeToSpawn,
					int amountToSpawn,
					int type,
					Size size,
					Vec2 targetLocation);
	~SpawnPattern();

	// Getters
	float GetTimeToSpawn();
	int GetAmountToSpawn();
	int GetType();
	Size GetSize();
	Vec2 GetTargetLocation();
};

#endif 
