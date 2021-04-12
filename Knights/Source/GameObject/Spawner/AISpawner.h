#ifndef _AI_SPAWNER_H_
#define _AI_SPAWNER_H_

#include "Spawner.h"
#include "SpawnPattern.h"

#include "cocos2d.h"
USING_NS_CC;

class AISpawner : public Node
{
private:
	float m_currentTimeLeftToNextSpawn;
	std::vector<SpawnPattern*> m_spawnPatternList;
	int m_currentSpawnPatternIndex;
	Spawner* m_spawner;

public:
	AISpawner();
	~AISpawner();

	// Setters
	void AddSpawnPattern(	float timeToSpawn,
							int amountToSpawn,
							int type,
							Size size,
							Vec2 targetLocation);
	void SetSpawner(Spawner* spawner);
	// Getters
	SpawnPattern* GetSpawnPattern(int index);
	int GetCurrentSpawnPatternIndex();
	Spawner* GetSpawner();

	// Others
	void Update(float dt);
	void StopSpawning();
};

#endif 
