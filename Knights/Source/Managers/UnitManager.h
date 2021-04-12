#ifndef _UNIT_MANAGER_H_
#define _UNIT_MANAGER_H_

#include "..\GameObject\Units\Hero.h"

#include "cocos2d.h"
USING_NS_CC;

class UnitManager
{
private:
	Unit* m_player;
	Unit* m_enemyCastle;
	Unit* m_enemyBoss;
	std::vector<Unit*> m_playerUnitsList;
	std::vector<Unit*> m_enemyUnitsList;
public:
	UnitManager();
	~UnitManager();

	// Setters
	void SetPlayer(Unit* player);
	void SetEnemyCastle(Unit* castle);
	void SetEnemyBoss(Unit* enemyBoss);
	void AddPlayerUnit(Unit* unit);
	void AddEnemyUnit(Unit* unit);

	// Getters
	Unit* GetPlayer();
	Unit* GetEnemyCastle();
	Unit* GetEnemyBoss();
	std::vector<Unit*>* GetPlayerUnitsList();
	std::vector<Unit*>* GetEnemyUnitsList();

	// Others
	void Update(float dt, bool bossCaptureScene = false);
	void PlayerBackstep();
	void StopMovingAllUnits();
};

#endif 
