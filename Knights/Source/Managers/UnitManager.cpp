#include "UnitManager.h"

UnitManager::UnitManager()
	: m_player(NULL)
	, m_enemyCastle(NULL)
	, m_enemyBoss(NULL)
{
	/* initialize random seed: */
	srand(time(NULL));
}

UnitManager::~UnitManager()
{
}

// Setters
void UnitManager::SetPlayer(Unit* player)
{
	if (m_player)
		delete m_player;
	m_player = player;
}
void UnitManager::SetEnemyCastle(Unit* castle)
{
	if (m_enemyCastle)
		delete m_enemyCastle;
	m_enemyCastle = castle;
}
void UnitManager::SetEnemyBoss(Unit* enemyBoss)
{
	if (m_enemyBoss)
		delete m_enemyBoss;
	m_enemyBoss = enemyBoss;
}
void UnitManager::AddPlayerUnit(Unit* unit)
{
	m_playerUnitsList.push_back(unit);
}
void UnitManager::AddEnemyUnit(Unit* unit)
{
	m_enemyUnitsList.push_back(unit);
}

// Getters
Unit* UnitManager::GetPlayer()
{
	return m_player;
}
Unit* UnitManager::GetEnemyCastle()
{
	return m_enemyCastle;
}
Unit* UnitManager::GetEnemyBoss()
{
	return m_enemyBoss;
}
std::vector<Unit*>* UnitManager::GetPlayerUnitsList()
{
	return &m_playerUnitsList;
}
std::vector<Unit*>* UnitManager::GetEnemyUnitsList()
{
	return &m_enemyUnitsList;
}

// Others
void UnitManager::Update(float dt, bool bossCaptureScene)
{
	// Get num of active player units
	int numOfActivePlayerUnits = 0;
	for (Unit* unit : m_playerUnitsList)
	{
		if (unit->IfActive())
			numOfActivePlayerUnits++;
	}
	// Get num of active enemy units
	int numOfActiveEnemyUnits = 0;
	for (Unit* unit : m_enemyUnitsList)
	{
		if (unit->IfActive())
			numOfActiveEnemyUnits++;
	}

	// Player
	if (m_player)
	{
		if (!bossCaptureScene)
		{
			Hero* playerHero = (Hero*)m_player;
			playerHero->Update(dt);
		}
		else
		{
			m_player->UpdateOthers(dt);
		}
	}
	// Boss
	if (m_enemyBoss)
	{
		if (m_enemyBoss->IfActive())
		{
			if (bossCaptureScene &&
				m_enemyBoss->GetState() == Unit::MOVING)
			{
				if (m_enemyBoss->chasingTarget == NULL || !m_enemyBoss->chasingTarget->IfActive())
				{
					if (numOfActivePlayerUnits > 0)
					{
						int randIndex = rand() % numOfActivePlayerUnits;
						for (Unit* unit : m_playerUnitsList)
						{
							if (unit->IfActive())
							{
								if (--randIndex < 0)
								{
									m_enemyBoss->chasingTarget = unit;
									break;
								}
							}
						}
					}
					else if (m_player)
						m_enemyBoss->chasingTarget = m_player;
				}
				m_enemyBoss->SetTargetLocation(m_enemyBoss->chasingTarget->getPosition());
			}
			m_enemyBoss->Update(dt);
		}
	}
	// Player Units
	for (Unit* unit : m_playerUnitsList)
	{
		if (unit->IfActive())
		{
			if (bossCaptureScene &&
				unit->GetState() == Unit::MOVING)
			{
				if (unit->chasingTarget == NULL || !unit->chasingTarget->IfActive())
				{
					if (numOfActiveEnemyUnits > 0)
					{
						int randIndex = rand() % numOfActiveEnemyUnits;
						for (Unit* unitEnemy : m_enemyUnitsList)
						{
							if (unitEnemy->IfActive())
							{
								if (--randIndex < 0)
								{
									unit->chasingTarget = unitEnemy;
									break;
								}
							}
						}
					}
					else if (m_enemyBoss)
						unit->chasingTarget = m_enemyBoss;
				}
				unit->SetTargetLocation(unit->chasingTarget->getPosition());
			}
			unit->Update(dt);
		}
	}
	// Enemy Units
	for (Unit* unit : m_enemyUnitsList)
	{
		if (unit->IfActive())
		{
			if (bossCaptureScene
				&& unit->GetState() == Unit::MOVING)
			{
				if (unit->chasingTarget == NULL || !unit->chasingTarget->IfActive())
				{
					if (numOfActivePlayerUnits > 0)
					{
						int randIndex = rand() % numOfActivePlayerUnits;
						for (Unit* unitPlayer : m_playerUnitsList)
						{
							if (unitPlayer->IfActive())
							{
								if (--randIndex < 0)
								{
									unit->chasingTarget = unitPlayer;
									break;
								}
							}
						}
					}
					else if (m_player)
						unit->chasingTarget = m_player;
				}
				unit->SetTargetLocation(unit->chasingTarget->getPosition());
			}
			unit->Update(dt);
		}
	}
}

void UnitManager::PlayerBackstep()
{
	// Player
	m_player->RemoveAllTargets();
	// Enemy Units
	for (Unit* unit : m_enemyUnitsList)
	{
		if (unit->IfActive())
		{
			unit->RemoveTarget(m_player);
		}
	}
}

void UnitManager::StopMovingAllUnits()
{
	// Player
	if (m_player)
	{
		auto idle = DelayTime::create(100.f);
		auto idleForever = RepeatForever::create(idle);
		idleForever->setTag(Unit::IDLING);
		m_player->StopAllActions();
		m_player->runAction(idleForever);
		m_player->SetState(Unit::IDLING);
	}
	// Boss
	if (m_enemyBoss)
	{
		if (m_enemyBoss->IfActive())
		{
			auto idle = DelayTime::create(100.f);
			auto idleForever = RepeatForever::create(idle);
			idleForever->setTag(Unit::IDLING);
			m_enemyBoss->StopAllActions();
			m_enemyBoss->runAction(idleForever);
			m_enemyBoss->SetState(Unit::IDLING);
		}
	}
	// Player Units
	for (Unit* unit : m_playerUnitsList)
	{
		if (unit->IfActive())
		{
			auto idle = DelayTime::create(100.f);
			auto idleForever = RepeatForever::create(idle);
			idleForever->setTag(Unit::IDLING);
			unit->StopAllActions();
			unit->runAction(idleForever);
			unit->SetState(Unit::IDLING);
		}
	}
	// Enemy Units
	for (Unit* unit : m_enemyUnitsList)
	{
		if (unit->IfActive())
		{
			auto idle = DelayTime::create(100.f);
			auto idleForever = RepeatForever::create(idle);
			idleForever->setTag(Unit::IDLING);
			unit->StopAllActions();
			unit->runAction(idleForever);
			unit->SetState(Unit::IDLING);
		}
	}
}