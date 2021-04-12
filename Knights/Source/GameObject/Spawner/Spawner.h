#ifndef _SPAWNER_H_
#define _SPAWNER_H_

#include "..\Units\Unit.h"

#include "cocos2d.h"
USING_NS_CC;

class Spawner : public Node
{
private:
	std::vector<Unit*> m_unitList;
	Vec2 m_spawnLocation;
	float m_maxOffsetY;
public:
	Spawner();
	~Spawner();

	// Setters
	void AddUnit(Unit* unit);
	void SetSpawnLocation(Vec2 location, float maxOffsetY);

	// Getters
	Vec2 GetSpawnLocation();
	std::vector<Unit*>* GetUnitList();

	// Others
	void Spawn(Size* size, Vec2* targetPosition, int type, int amountToSpawned = 1, Vec2* spawnPosition = NULL);
};

#endif 
