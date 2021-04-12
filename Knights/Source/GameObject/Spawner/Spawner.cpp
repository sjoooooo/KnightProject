#include "Spawner.h"
#include "..\..\Scene\StatScene.h"

Spawner::Spawner()
	: m_spawnLocation(0.0f, 0.0f)
	, m_maxOffsetY(0.0f)
{
	/* initialize random seed: */
	srand(time(NULL));
}

Spawner::~Spawner()
{
	for (int i = 0; i < m_unitList.size(); i++)
	{
		if (m_unitList[i])
			delete m_unitList[i];
	}
}

// Setters
void Spawner::AddUnit(Unit* unit)
{
	m_unitList.push_back(unit);
}
void Spawner::SetSpawnLocation(Vec2 location, float maxOffsetY)
{
	m_spawnLocation = location;
	m_maxOffsetY = maxOffsetY;
}

// Getters
Vec2 Spawner::GetSpawnLocation()
{
	return m_spawnLocation;
}
std::vector<Unit*>* Spawner::GetUnitList()
{
	return &m_unitList;
}

// Others
void Spawner::Spawn(Size* size, Vec2* targetPosition, int type, int amountToSpawned, Vec2* spawnPosition)
{
	int currentAmountSpawned = 0;
	for (int i = 0; i < m_unitList.size(); i++)
	{
		if (!m_unitList[i]->IfActive())
		{
			// Activate this unit
			m_unitList[i]->SetActive(true);
			m_unitList[i]->SetType(type);
			Sprite* unit_sprite = m_unitList[i]->GetSprite();
			unit_sprite->setFlippedX(false);
			float offsetY = (((float)rand() / RAND_MAX) * m_maxOffsetY);
			m_unitList[i]->setLocalZOrder((int)m_maxOffsetY + 1 - (int)offsetY);
			Vec2 newTargetPos(targetPosition->x, targetPosition->y + offsetY);
			Vec2 newSpawnPos;
			if (spawnPosition)
				newSpawnPos.set(spawnPosition->x, spawnPosition->y + offsetY);
			else
				newSpawnPos.set(m_spawnLocation.x, m_spawnLocation.y + offsetY);

			m_unitList[i]->SelfSetInfoAccordingToTypeAndLocations(newTargetPos, newSpawnPos);
			m_unitList[i]->SetStopTexture();
			// Adjust sprite scaling of the unit
			unit_sprite->setScale(size->width / unit_sprite->getContentSize().width, size->height / unit_sprite->getContentSize().height);
			// Set unit to spawn position
			Vec2 unitSpawnLocation(newSpawnPos.x + currentAmountSpawned * size->width, newSpawnPos.y);
			m_unitList[i]->setPosition(unitSpawnLocation);

			// Move to target location
			m_unitList[i]->MoveToTarget();

			// Resize HPBar
			m_unitList[i]->ResizeHPBar();

			if (++currentAmountSpawned == amountToSpawned)
			{
				return;
			}
		}
	}
}