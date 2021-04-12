#include "AISpawner.h"

AISpawner::AISpawner()
	: m_currentTimeLeftToNextSpawn(0.0f)
	, m_currentSpawnPatternIndex(-1)
	, m_spawner(NULL)
{
}

AISpawner::~AISpawner()
{
	for (int i = 0; i < m_spawnPatternList.size(); i++)
	{
		if (m_spawnPatternList[i])
			delete m_spawnPatternList[i];
	}
	if (m_spawner)
		delete m_spawner;
}

// Setters
void AISpawner::AddSpawnPattern(float timeToSpawn,
								int amountToSpawn,
								int type,
								Size size,
								Vec2 targetLocation)
{
	m_spawnPatternList.push_back(new SpawnPattern(timeToSpawn, amountToSpawn, type, size, targetLocation));
	if (m_spawnPatternList.size() == 1)
	{
		m_currentTimeLeftToNextSpawn = timeToSpawn;
		m_currentSpawnPatternIndex = 0;
	}
}
void AISpawner::SetSpawner(Spawner* spawner)
{
	m_spawner = spawner;
}
// Getters
SpawnPattern* AISpawner::GetSpawnPattern(int index)
{
	if (index < m_spawnPatternList.size())
		return m_spawnPatternList[index];
	else
		return NULL;
}
int AISpawner::GetCurrentSpawnPatternIndex()
{
	return m_currentSpawnPatternIndex;
}
Spawner* AISpawner::GetSpawner()
{
	return m_spawner;
}

// Others
void AISpawner::Update(float dt)
{
	if (m_currentSpawnPatternIndex < m_spawnPatternList.size())
	{
		m_currentTimeLeftToNextSpawn -= dt;
		if (m_currentTimeLeftToNextSpawn <= 0.0f)
		{
			Size unitsize(m_spawnPatternList[m_currentSpawnPatternIndex]->GetSize());
			Vec2 targetlocation(m_spawnPatternList[m_currentSpawnPatternIndex]->GetTargetLocation());
			m_spawner->Spawn(&unitsize, &targetlocation, m_spawnPatternList[m_currentSpawnPatternIndex]->GetType(), m_spawnPatternList[m_currentSpawnPatternIndex]->GetAmountToSpawn());
			if (++m_currentSpawnPatternIndex < m_spawnPatternList.size())
				m_currentTimeLeftToNextSpawn = m_spawnPatternList[m_currentSpawnPatternIndex]->GetTimeToSpawn();
		}
	}
}

void AISpawner::StopSpawning()
{
	m_currentSpawnPatternIndex = m_spawnPatternList.size();
}