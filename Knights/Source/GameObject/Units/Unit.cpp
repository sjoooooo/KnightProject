#include "Unit.h"

#include "SimpleAudioEngine.h"
#include "../Spawner/Spawner.h"
#include "../../Managers/AnimationManager.h"
#include "../../Scene/StatScene.h"
#include "..\Item\Item.h"
#include "..\Skills\Skill.h"

using namespace CocosDenshion;
USING_NS_CC;

Unit::Unit()
	: m_sprite(NULL)
	, m_hpbar(NULL)
	, m_active(false)
	, m_currentState(IDLING)
	, m_targetLocation(0.0f, 0.0f)
	, m_spawnLocation(0.0f, 0.0f)
	, m_timeLeftToNextRegenaration(1.0f)
	, m_type(NIL)
	, m_totalDistanceToMove(0.0f)
	, chasingTarget(NULL)
	, m_hasShooted(false)
	, m_hasHitted(false)
{
	/* initialize random seed: */
	srand(time(NULL));

	m_attackEffect = Sprite::create();
	this->addChild(m_attackEffect);
	m_attackEffect->setAnchorPoint(Point(0.5f, 0.0f));
	m_attackEffect->setVisible(false);
}

Unit::~Unit()
{
	if (m_sprite)
		delete m_sprite;
	if (m_hpbar)
		delete m_hpbar;
	for (Unit* target : m_targetList)
	{
		if (target)
			delete target;
	}
	if (m_attackEffect)
		delete m_attackEffect;
}


// Setters
void Unit::SetSprite(Sprite* sprite)
{
	m_sprite = sprite;
	this->addChild(m_sprite, 0);
}
void Unit::SetInfo(	int health, int maxHealth, int regeneration,
					int attackDamage, float attackSpeed, float attackRange,
					int defense,
					float movementSpeed,
					Point targetLocation,
					Point spawnLocation)
{
	m_info.SetInfo(health, maxHealth, regeneration, attackDamage, attackSpeed, attackRange, defense, movementSpeed);
	m_targetLocation = targetLocation;
	m_spawnLocation = spawnLocation;
}

void Unit::SelfSetInfoAccordingToTypeAndLocations(Point targetLocation, Point spawnLocation)
{
	m_targetLocation = targetLocation;
	m_spawnLocation = spawnLocation;
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	int prcPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::PRC_PLUS_BUTTON)).c_str(), 0);
	int hpPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::HP_PLUS_BUTTON)).c_str(), 0);
	int strPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::STR_PLUS_BUTTON)).c_str(), 0);
	int defPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::DEF_PLUS_BUTTON)).c_str(), 0);

	switch (m_type)
	{
		case Unit::HERO:
		{
			m_info.SetInfo(	400 + hpPoints * 10, 400 + hpPoints * 10, 5,
							45 + strPoints * 5, 1.5f, 0.0f,
							5 + defPoints * 5,
							10.f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::HERO_TWO_HANDED_SWORD:
		{
			m_info.SetInfo(	400 + hpPoints * 10, 400 + hpPoints * 10, 5,
							45 + strPoints * 5, 1.5f, 0.0f,
							5 + defPoints * 5,
							10.f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::HERO_SPEAR:
		{
			m_info.SetInfo(	400 + hpPoints * 10, 400 + hpPoints * 10, 5,
							45 + strPoints * 5, 1.5f, 0.0f,
							5 + defPoints * 5,
							10.f);
			m_attacktype = NORMAL;
			break;
		}		
		case Unit::HERO_BOW:
		{
			m_info.SetInfo(	400 + hpPoints * 10, 400 + hpPoints * 10, 5,
							45 + strPoints * 5, 1.5f, 0.0f,
							5 + defPoints * 5,
							10.f);
			m_attacktype = SHOOTING;
			break;
		}
		// Player Units
		case Unit::SWORDSMAN:
		{
			m_info.SetInfo(	150 + prcPoints * 10,
							150 + prcPoints * 10,
							0,
		
							15 + prcPoints * 5,
							2.5f,
							0.f,
		
							0,
							9.f);
			if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", Skill::SWORDSMAN)).c_str(), false))
			{
				Item::SetItemEffect_Info(Skill::SWORDSMAN, &m_info);
			}
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ARCHER:
		{
			m_info.SetInfo(	100 + prcPoints * 5,
							100 + prcPoints * 5,
							0,
		
							25 + prcPoints * 10,
							3.0f,
							10.f,
		
							0,
							8.f);
			if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", Skill::ARCHER)).c_str(), false))
			{
				Item::SetItemEffect_Info(Skill::ARCHER, &m_info);
			}
			m_attacktype = NORMAL;
			break;
		}
		case Unit::HORSEMAN:
		{
			m_info.SetInfo(	120 + prcPoints * 10,
							120 + prcPoints * 10,
							0,
		
							20 + prcPoints * 15,
							2.5f,
							0.f,
		
							0,
							20.f);
			if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", Skill::HORSEMAN)).c_str(), false))
			{
				Item::SetItemEffect_Info(Skill::HORSEMAN, &m_info);
			}
			m_attacktype = NORMAL;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			//SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/horse_man_spawn.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/horse_man_spawn.wav");
#endif
			break;
		}
		case Unit::SPEARMAN:
		{
			m_info.SetInfo(	150 + prcPoints * 10,
							150 + prcPoints * 10,
							0,
		
							30 + prcPoints * 10,
							3.0f,
							0.f,
		
							0,
							6.f);
			if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", Skill::SPEARMAN)).c_str(), false))
			{
				Item::SetItemEffect_Info(Skill::SPEARMAN, &m_info);
			}
			m_attacktype = NORMAL;
			break;
		}
		case Unit::WIZARD:
		{
			m_info.SetInfo(	190 + prcPoints * 5,
							190 + prcPoints * 5,
							0,
		
							50 + prcPoints * 20,
							3.0f,
							15.0f,
		
							0,
							8.0f);
			if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", Skill::WIZARD)).c_str(), false))
			{
				Item::SetItemEffect_Info(Skill::WIZARD, &m_info);
			}
			m_attacktype = NORMAL;
			break;
		}
		case Unit::SHIELDMAN:
		{
			m_info.SetInfo(	400 + prcPoints * 20,
							400 + prcPoints * 20,
							0,
		
							10 + prcPoints * 3,
							10.f,
							0.f,
		
							0,
							10.f);
			if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", Skill::SHIELD)).c_str(), false))
			{
				Item::SetItemEffect_Info(Skill::SHIELD, &m_info);
			}
			m_attacktype = NORMAL;
			break;
		}
		// Bosses
		case Unit::BOSS_RICH:
		{
			m_info.SetInfo(	400, 400, 0,
							0, 0.0f, 0.f,
							5,
							13.f);
			if (m_targetLocation.x > m_spawnLocation.x)
				m_sprite->setFlippedX(true);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::BOSS_FALLEN_ALCHEMIST:
		{
			m_info.SetInfo(	450, 450, 0,
							40, 2.5f, 14.f,
							15,
							8.f);
			if (m_targetLocation.x > m_spawnLocation.x)
				m_sprite->setFlippedX(true);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::BOSS_ESCAPED_PRISONER:
		{
			m_info.SetInfo(	550, 550, 0,
							60, 2.f, 0.f,
							20,
							10.0f);
			if (m_targetLocation.x > m_spawnLocation.x)
				m_sprite->setFlippedX(true);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			m_info.SetInfo(	800, 800, 0,
							80, 3.0f, 0.f,
							30,
							13.f);
			if (m_targetLocation.x > m_spawnLocation.x)
				m_sprite->setFlippedX(true);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::BOSS_COMMANDER:
		{
			m_info.SetInfo(	1000, 1000, 0,
							90, 3.0f, 0.f,
							40,
							15.f);
			if (m_targetLocation.x > m_spawnLocation.x)
				m_sprite->setFlippedX(true);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::BOSS_QUEEN:
		{
			m_info.SetInfo(	900, 900, 0,
							120, 3.0f	, 0.f,
							35,
							15.f);
			if (m_targetLocation.x > m_spawnLocation.x)
				m_sprite->setFlippedX(true);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::BOSS_WIZARD:
		{
			m_info.SetInfo(	1200, 1200, 0,
							85, 2.0f, 10.f,
							45,
							13.f);
			if (m_targetLocation.x > m_spawnLocation.x)
				m_sprite->setFlippedX(true);
			m_attacktype = CAST_SKILL;
			break;
		}
		case Unit::BOSS_KING:
		{
			m_info.SetInfo(	1600, 1600, 0,
							130, 1.25f, 9.f,
							60,
							13.f);
			if (m_targetLocation.x > m_spawnLocation.x)
				m_sprite->setFlippedX(true);
			m_attacktype = SHOOTING;
			break;
		}
		// Enemy Units
		case Unit::ENEMY_SLAVE:
		{
			m_info.SetInfo(	120, 120, 0,
							7, 2.f, 0.f,
							0,
							10.f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_FARMER:
		{
			m_info.SetInfo(	90, 90, 0,
							5, 5.f, 0.f,
							0,
							5.0f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_PEOPLE:
		{
			m_info.SetInfo(	80, 80, 0,
							9, 10.f, 6.f,
							0,
							7.0f);
			m_attacktype = NORMAL;
			break;
		}

		case Unit::ENEMY_BEGGAR:
		{
			m_info.SetInfo(	100, 100, 0,
							10, 4.f, 0.f,
							0,
							5.0f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_BARON:
		{
			m_info.SetInfo(	130, 130, 0,
							14, 3.3f, 0.f,
							5,
							6.0f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_ALCHEMIST:
		{
			m_info.SetInfo(	100, 100, 0,
							13, 3.f, 12.f,
							3,
							5.0f);
			m_attacktype = NORMAL;
			break;
		}

		case Unit::ENEMY_SHIELDMAN:
		{
			m_info.SetInfo(	200, 200, 0,
							10, 10.f, 0.f,
							15,
							6.0f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_KNIGHT:
		{
			m_info.SetInfo(	150, 150, 0,
							15, 2.3f, 0.f,
							5,
							9.0f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_HEAVY_ARMOR_KNIGHT:
		{
			m_info.SetInfo(	180, 180, 0,
							18, 2.5f, 0.f,
							8,
							8.0f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_SPEARMAN:
		{
			m_info.SetInfo(	190, 190, 0,
							20, 3.f, 0.f,
							5,
							7.0f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_PLATE_KNIGHT:
		{
			m_info.SetInfo(	200, 200, 0,
							23, 2.f, 0.f,
							7,
							9.0f);
			m_attacktype = NORMAL;
			break;
		}

		case Unit::ENEMY_ICE_WIZARD:
		{
			m_info.SetInfo(	100, 100, 0,
							25, 3.f, 10.f,
							0,
							7.0f);
			m_attacktype = SHOOTING;
			break;
		}
		case Unit::ENEMY_LIGHTNING_WIZARD:
		{
			m_info.SetInfo(	80, 80, 0,
							30, 2.3f, 10.f,
							0,
							7.0f);
			m_attacktype = CAST_SKILL;
			break;
		}
		case Unit::ENEMY_FIRE_WIZARD:
		{
			m_info.SetInfo(	95, 95, 0,
							27, 3.f, 10.f,
							0,
							7.0f);
			m_attacktype = SHOOTING;
			break;
		}

		case Unit::ENEMY_HEAVY_ARMOR_SPEARMAN:
		{
			m_info.SetInfo(	210, 210, 0,
							21, 2.f, 0.f,
							10,
							6.0f);
			m_attacktype = NORMAL;
			break;
		}
		
		case Unit::ENEMY_STRENGTHEN_ICE_WIZARD:
		{
			m_info.SetInfo(	130, 130, 0,
							40, 2.3f, 15.f,
							5,
							8.0f);
			m_attacktype = SHOOTING;
			break;
		}
		case Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD:
		{
			m_info.SetInfo(	120, 120, 0,
							45, 2.5f, 15.f,
							5,
							8.0f);
			m_attacktype = CAST_SKILL;
			break;
		}
		case Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT:
		{
			m_info.SetInfo(	250, 250, 0,
							30, 3.0f, 0.f,
							15,
							10.0f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_STRENGTHEN_SHIELDMAN:
		{
			m_info.SetInfo(	400, 400, 0,
							13, 10.0f, 0.f,
							30,
							7.0f);
			m_attacktype = NORMAL;
			break;
		}
		case Unit::ENEMY_MASS_UNIT:
		{
			m_info.SetInfo(	2000, 2000, 0,
							130, 3.0f, 0.f,
							35,
							13.0f);
			m_attacktype = NORMAL;
			break;
		}
	}
}
void Unit::SetInfo(UnitInfo* info)
{
	m_info = *info;
}
void Unit::AddBonusInfoList(UnitInfoBonus* bonusInfo)
{
	m_bonusInfoList.push_back(*bonusInfo);
}
void Unit::AddBonusInfoList(int health, int maxHealth, int regeneration,
							int attackDamage, float attackSpeed, float attackRange,
							int defense,
							float movementSpeed,
							float timeLeft,
							bool gotWeapon)
{
	m_bonusInfoList.push_back(UnitInfoBonus(health, maxHealth, regeneration, attackDamage, attackSpeed, attackRange, defense, movementSpeed, timeLeft, gotWeapon));
}
void Unit::SetActive(bool active)
{
	m_active = active;
	this->setVisible(m_active);
}
void Unit::SetState(int currentState)
{
	m_currentState = currentState;
}
void Unit::SetTargetLocation(Point targetLocation)
{
	m_targetLocation = targetLocation;
	if (m_currentState == MOVING)
	{
		auto MoveToAction = (MoveTo*)this->getActionByTag(MOVING);
		if (MoveToAction && MoveToAction->getElapsed() > 0.25f)
		{
			this->stopActionByTag(MOVING);

			// If moving toward right
			if (this->getPositionX() < targetLocation.x)
			{
				// Flip the image to face Right
				if (m_type >= SWORDSMAN && m_type <= HERO)
				{
					m_sprite->setFlippedX(false);
				}
				else
				{
					m_sprite->setFlippedX(true);
				}
			}
			else if (this->getPositionX() > targetLocation.x)
			{
				// Flip the image to face Left
				if (m_type >= SWORDSMAN && m_type <= HERO)
				{
					m_sprite->setFlippedX(true);
				}
				else
				{
					m_sprite->setFlippedX(false);
				}
			}
			// Get time to reach target position
			float distanceToMove = (targetLocation - this->getPosition()).length();
			float timeTaken = ((distanceToMove / m_totalDistanceToMove) * 100.0f) / this->GetMovementSpeed();

			// Set unit to move to target position
			MoveToAction = MoveTo::create(timeTaken, targetLocation);
			MoveToAction->setTag(MOVING);
			this->runAction(MoveToAction);
		}
	}
}
void Unit::SetSpawnLocation(Point spawnLocation)
{
	m_spawnLocation = spawnLocation;
}
void Unit::InitHPBar(std::string background, std::string hpbar, std::string outline, bool fixedToUnit)
{
	if (!m_hpbar)
	{
		m_hpbar = new HPBar;
		if (fixedToUnit)
		{
			this->addChild(m_hpbar, 3);
		}
	}
	m_hpbar->Init(background, hpbar, outline);
}
void Unit::ResizeHPBar()
{
	if (m_hpbar)
	{
		Size spriteSize(m_sprite->getContentSize().width * m_sprite->getScaleX(),
						m_sprite->getContentSize().height * m_sprite->getScaleY());
		Size hpBarSize(spriteSize.width * 1.5f, spriteSize.height * 0.1f);
		m_hpbar->Resize(hpBarSize);
		m_hpbar->setPositionY(spriteSize.height);
	}
}
void Unit::SetType(int type)
{
	m_type = type;
}

// Getters
Sprite* Unit::GetSprite()
{
	return m_sprite;
}
Sprite* Unit::GetAttackEffect()
{
	return m_attackEffect;
}
UnitInfo* Unit::GetInfo()
{
	return &m_info;
}
std::vector<UnitInfoBonus>* Unit::GetBonusInfoList()
{
	return &m_bonusInfoList;
}
bool Unit::IfActive()
{
	return m_active;
}
int Unit::GetState()
{
	return m_currentState;
}
Point Unit::GetTargetLocation()
{
	return m_targetLocation;
}
Point Unit::GetSpawnLocation()
{
	return m_spawnLocation;
}
int Unit::GetType()
{
	return m_type;
}
HPBar* Unit::GetHPBar()
{
	return m_hpbar;
}
// Get Total Info
int Unit::GetHealth()
{
	int total = m_info.GetHealth();
	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		total += m_bonusInfoList[i].GetHealth();
	}
	return total;
}
int Unit::GetMaxHealth()
{
	int total = m_info.GetMaxHealth();
	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		total += m_bonusInfoList[i].GetMaxHealth();
	}
	return total;
}
int Unit::GetRegeneration()
{
	int total = m_info.GetRegeneration();
	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		total += m_bonusInfoList[i].GetRegeneration();
	}
	return total;
}
int Unit::GetAttackDamage()
{
	int total = m_info.GetAttackDamage();
	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		total += m_bonusInfoList[i].GetAttackDamage();
	}
	return total;
}
float Unit::GetAttackSpeed()
{
	float total = m_info.GetAttackSpeed();
	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		total += m_bonusInfoList[i].GetAttackSpeed();
	}
	return total;
}
float Unit::GetAttackRange()
{
	float total = m_info.GetAttackRange();
	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		total += m_bonusInfoList[i].GetAttackRange();
	}
	return total;
}
int Unit::GetDefense()
{
	int total = m_info.GetDefense();
	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		total += m_bonusInfoList[i].GetDefense();
	}

	// Rot
	Sprite* rot = (Sprite*)this->getChildByTag(ROT);
	if (rot)
	{
		if (rot->isVisible())
			total -= m_corruptedDefense;
	}
	return total;
}
float Unit::GetMovementSpeed()
{
	float total = m_info.GetMovementSpeed();
	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		total += m_bonusInfoList[i].GetMovementSpeed();
	}
	return total;
}

// Others
void Unit::Update(float dt)
{
	switch (m_currentState)
	{
		// KNOCKED_UP
		case KNOCKED_UP:
		{
			// if no knock up Action
			if (!this->getActionByTag(KNOCKED_UP))
			{
				MinusHeallth(m_knockedUpDamage);
				StopAllActions();
				Attack();
			}
			break;
		}
		// MOVING
		case MOVING:
		{
			// if no moving Action
			if (!this->getActionByTag(MOVING))
			{
				// If at target location,
				if (this->getPosition() == m_targetLocation)
				{
					StopAllActions();
					Attack();
				}
				else
				{
					MoveToTarget();
				}
			}
			break;
		}
		// ATTACKING
		case ATTACKING:
		{
			CheckActivateAttackEffect();
			// numOfHitted
			int hitted = 0;
			// Had rotted
			bool rotted = false;
			for (int i = 0; i < m_targetList.size(); i++)
			{
				// if no moving Action
				if (!this->getActionByTag(ATTACKING))
				{
					// Skip castle if castle is not the only target
					if (m_targetList[i]->GetType() == CASTLE &&
						m_targetList.size() > 1)
					{
						continue;
					}
					// For not Boss Wizard nor King
					if (m_attacktype == NORMAL)
					{
						if ((hitted < 1 && m_type != SPEARMAN) ||
							(hitted < 2 && m_type == SPEARMAN))
						{
							if (m_targetList[i]->IfActive())
							{
								// Minus target's health
								m_targetList[i]->MinusHeallth(this->GetAttackDamage());
								hitted++;
							}
							// If 2nd boss
							if (m_targetList[i]->IfActive() &&
								m_type == BOSS_FALLEN_ALCHEMIST &&
								!rotted &&
								m_targetList[i]->GetType() != CASTLE)
							{
								m_targetList[i]->Rot(20.0f, 10, 50.f);
								rotted = true;
							}
						}
					}
				}
				// remove target if target is dead
				if (!m_targetList[i]->IfActive())
				{
					m_targetList.erase(m_targetList.begin() + i);
					i--;
				}
			}
			// if no targets left
			if (m_targetList.size() == 0)
			{
				MoveToTarget();
			}
			// else continue to attack
			else if (!this->getActionByTag(ATTACKING))
			{
				WaitToNextAttack();
			}
			break;
		}
		// WAITING_TO_NEXT_ATTACK
		case WAITING_TO_NEXT_ATTACK:
		{
			// Minus each target's health 
			for (int i = 0; i < m_targetList.size(); i++)
			{
				// remove target if target is dead
				if (!m_targetList[i]->IfActive())
				{
					m_targetList.erase(m_targetList.begin() + i);
					i--;
				}
			}
			// if no targets left
			if (m_targetList.size() == 0)
			{
				MoveToTarget();
			}
			// else continue to attack
			else if (!this->getActionByTag(WAITING_TO_NEXT_ATTACK))
			{
				Attack();
			}
			break;
		}
		// TANKING
		case TANKING:
		{
			for (int i = 0; i < m_targetList.size(); i++)
			{
				// remove target if target is dead
				if (!m_targetList[i]->IfActive())
				{
					m_targetList.erase(m_targetList.begin() + i);
					i--;
				}
			}
			// if no targets left
			if (m_targetList.size() == 0)
			{
				MoveToTarget();
			}
			break;
		}
		// FROZEN
		case FROZEN:
		{
			Sprite* ice = (Sprite*)this->getChildByTag(FROZEN);
			if (ice)
			{
				if (!ice->getActionByTag(FROZEN) &&
					ice->isVisible())
				{
					ice->setVisible(false);
					Attack();
				}
			}
			break;
		}
	}

	CheckAttackEffect();

	UpdateOthers(dt);
}

void Unit::UpdateOthers(float dt)
{
	// Regenaration
	if (!this->getActionByTag(REGENEARTION))
	{
		Heal(this->GetRegeneration());

		auto regenerate = DelayTime::create(1.0f);
		regenerate->setTag(REGENEARTION);
		this->runAction(regenerate);
	}

	// Burning
	ParticleSystem* burner = (ParticleSystem*)this->getChildByTag(BURNING);
	if (burner)
	{
		if (burner->getActionByTag(BURNING))
		{
			if (!this->getActionByTag(TO_BURN))
			{
				MinusHeallth(m_burningDamage);

				auto to_burn = DelayTime::create(1.0f);
				to_burn->setTag(TO_BURN);
				this->runAction(to_burn);
			}
		}
		else if (burner->isActive())
		{
			burner->stopSystem();
		}
	}
	// Electric Shock
	Sprite* electricShock = (Sprite*)this->getChildByTag(ELECTRIC_SHOCK);
	if (electricShock)
	{
		if (!electricShock->getActionByTag(ELECTRIC_SHOCK) &&
			electricShock->isVisible())
		{
			electricShock->setVisible(false);
		}
	}
	// Rot
	Sprite* rot = (Sprite*)this->getChildByTag(ROT);
	if (rot)
	{
		if (!rot->getActionByTag(ROT) &&
			rot->isVisible())
		{
			rot->setVisible(false);
		}
	}

	UpdateBonusInfo(dt);
}

void Unit::UpdateBonusInfo(float dt)
{
	// For each bonus info,
	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		// Reduce time left
		m_bonusInfoList[i].SetTimeLeft(m_bonusInfoList[i].GetTimeLeft() - dt);
		// If time up,
		if (m_bonusInfoList[i].GetTimeLeft() <= 0.0f)
		{
			// if there is weapon,
			if (m_bonusInfoList[i].GotWeapon())
			{
			}
			auto it = m_bonusInfoList.begin() + i;
			m_bonusInfoList.erase(it);
			i--;
		}
	}
}


void Unit::Reset()
{
	// Burner
	ParticleSystem* burner = (ParticleSystem*)this->getChildByTag(BURNING);
	if (burner)
	{
		burner->stopAllActions();
		burner->stopSystem();
	}
	// Freezer
	Sprite* ice = (Sprite*)this->getChildByTag(FROZEN);
	if (ice)
	{
		ice->stopAllActions();
		ice->setVisible(false);
	}
	// Electric shock
	Sprite* electricShock = (Sprite*)this->getChildByTag(ELECTRIC_SHOCK);
	if (electricShock)
	{
		electricShock->stopAllActions();
		electricShock->setVisible(false);
	}
	// Rot
	Sprite* rot = (Sprite*)this->getChildByTag(ROT);
	if (rot)
	{
		rot->stopAllActions();
		rot->setVisible(false);
	}
	// Stop all actions
	this->StopAllActions(true);
	// Set Active to false
	SetActive(false);
	// Set info all to 0
	m_info.SetInfo(0, 0, 0, 0, 0, 0, 0, 0);
	// clear bonus info list
	m_bonusInfoList.clear();
	// Set target and spawn location to zero
	m_targetLocation.setZero();
	m_spawnLocation.setZero();
	// Clear all target
	m_targetList.clear();
	m_currentState = NIL;
	chasingTarget = NULL;
}


void Unit::StopAction(int state)
{
	this->stopAllActionsByTag(state);
	m_sprite->stopAllActionsByTag(state);
	if (state == ATTACKING)
	{
		m_attackEffect->stopAllActionsByTag(ATTACKING);
		m_attackEffect->setVisible(false);
		m_hasShooted = false;
	}
}

void Unit::StopAllActions(bool includingMinusHealthEffect)
{
	this->stopAllActions();
	if (includingMinusHealthEffect)
	{
		// Stop all actions
		m_sprite->stopAllActions();
		m_sprite->setColor(Color3B::WHITE);
	}
	else
	{
		// Stop all actions but m_sprite glow effect
		m_sprite->stopAllActionsByTag(MOVING);
		m_sprite->stopAllActionsByTag(ATTACKING);
	}
	m_attackEffect->setVisible(false);
	m_attackEffect->stopAllActionsByTag(ATTACKING);
	SetStopTexture();
}

void Unit::SetStopTexture()
{
	switch(m_type)
	{
		case Unit::SWORDSMAN:
		{
			m_sprite->setTexture("CharacterAnimations/unit/swordsman/stop.png");
			break;
		}
		case Unit::ARCHER:
		{
			m_sprite->setTexture("CharacterAnimations/unit/archer/stop.png");
			break;
		}
		case Unit::HORSEMAN:
		{
			m_sprite->setTexture("CharacterAnimations/unit/horseman/stop.png");
			break;
		}
		case Unit::SPEARMAN:
		{
			m_sprite->setTexture("CharacterAnimations/unit/spearman/stop.png");
			break;
		}
		case Unit::WIZARD:
		{
			m_sprite->setTexture("CharacterAnimations/unit/wizard/stop.png");
			break;
		}
		case Unit::SHIELDMAN:
		{
			m_sprite->setTexture("CharacterAnimations/unit/shieldman/stop.png");
			break;
		}
		case Unit::HERO_TWO_HANDED_SWORD:
		{
			m_sprite->setTexture("CharacterAnimations/stop_sd.png");
			break;
		}
		case Unit::HERO_SPEAR:
		{
			m_sprite->setTexture("CharacterAnimations/spear/stop.png");
			break;
		}
		case Unit::HERO_BOW:
		{
			m_sprite->setTexture("CharacterAnimations/bow/stop.png");
			break;
		}
		case Unit::HERO:
		{
			m_sprite->setTexture("CharacterAnimations/stop_sd.png");
			break;
		}
		// Enemy Units
		case Unit::ENEMY_SLAVE:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/slave/walk/walk-1.png");
			break;
		}
		case Unit::ENEMY_FARMER:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/farmer/walk/walk-1.png");
			break;
		}
		case Unit::ENEMY_PEOPLE:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/people/stop.png");
			break;
		}

		case Unit::ENEMY_BEGGAR:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/beggar/stop.png");
			break;
		}
		case Unit::ENEMY_BARON:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/baron/stop.png");
			break;
		}
		case Unit::ENEMY_ALCHEMIST:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/enemy_alchemist/stop.png");
			break;
		}

		case Unit::ENEMY_SHIELDMAN:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/enemy_shieldman/shieldman/stop.png");
			break;
		}
		case Unit::ENEMY_KNIGHT:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/knight/knight/stop.png");
			break;
		}
		case Unit::ENEMY_HEAVY_ARMOR_KNIGHT:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/knight/heavy_armor_knight/stop.png");
			break;
		}
		case Unit::ENEMY_SPEARMAN:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/spearman/spearman/stop.png");
			break;
		}
		case Unit::ENEMY_PLATE_KNIGHT:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/knight/knight/stop.png");
			break;
		}

		case Unit::ENEMY_ICE_WIZARD:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/wizard/ice/stop.png");
			break;
		}
		case Unit::ENEMY_LIGHTNING_WIZARD:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/wizard/thunder/stop.png");
			break;
		}
		case Unit::ENEMY_FIRE_WIZARD:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/wizard/fire/stop.png");
			break;
		}

		case Unit::ENEMY_HEAVY_ARMOR_SPEARMAN:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/spearman/heavy_armor_spearman/stop.png");
			break;
		}

		case Unit::ENEMY_STRENGTHEN_ICE_WIZARD:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/strengthen_wizard/ice/stop.png");
			break;
		}
		case Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/strengthen_wizard/thunder/stop.png");
			break;
		}
		case Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/knight/strengthen_plate_knight/stop.png");
			break;
		}
		case Unit::ENEMY_STRENGTHEN_SHIELDMAN:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/enemy_shieldman/strengthen_shieldman/stop.png");
			break;
		}
		case Unit::ENEMY_MASS_UNIT:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/crowd/stop.png");
			break;
		}
		// Bosses
		case Unit::BOSS_RICH:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/boss/1stage/stop.png");
			break;
		}
		case Unit::BOSS_FALLEN_ALCHEMIST:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/boss/2stage/stop.png");
			break;
		}
		case Unit::BOSS_ESCAPED_PRISONER:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/boss/3stage/stop.png");
			break;
		}
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/boss/4stage/stop/stop-1.png");
			break;
		}
		case Unit::BOSS_COMMANDER:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/boss/5stage/stop.png");
			break;
		}
		case Unit::BOSS_QUEEN:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/boss/6stage/stop.png");
			break;
		}
		case Unit::BOSS_WIZARD:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/boss/7stage/stop.png");
			break;
		}
		case Unit::BOSS_KING:
		{
			m_sprite->setTexture("CharacterAnimations/Enemy/boss/8stage/stop.png");
			break;
		}
	}
}


void Unit::Idle(float duration)
{
	StopAllActions();
	m_currentState = IDLING;

	// Set unit to for certain duration
	auto idle = DelayTime::create(duration);
	idle->setTag(IDLING);
	this->runAction(idle);
}


void Unit::MoveToTarget()
{
	MoveToLocation(m_targetLocation);
}

void Unit::MoveToLocation(Vec2 location)
{
	StopAllActions();
	m_currentState = MOVING;

	// Get time to reach target position
	float distanceToMove = (location - this->getPosition()).length();
	float timeTaken = ((distanceToMove / m_totalDistanceToMove) * 100.0f) / this->GetMovementSpeed();

	// Set unit to move to target position
	auto MoveToAction = MoveTo::create(timeTaken, location);
	MoveToAction->setTag(MOVING);
	this->runAction(MoveToAction);

	// If moving toward right
	if (this->getPositionX() < location.x)
	{
		// Flip the image to face Right
		if (m_type >= SWORDSMAN && m_type <= HERO)
		{
			m_sprite->setFlippedX(false);
		}
		else
		{
			m_sprite->setFlippedX(true);
		}
	}
	else if (this->getPositionX() > location.x)
	{
		// Flip the image to face Left
		if (m_type >= SWORDSMAN && m_type <= HERO)
		{
			m_sprite->setFlippedX(true);
		}
		else
		{
			m_sprite->setFlippedX(false);
		}
	}

	MoveAnimate();
}

void Unit::MoveBy(Vec2 distance)
{
	StopAllActions();
	m_currentState = MOVING;

	// Get time to reach target position
	float timeTaken = ((distance.length() / m_totalDistanceToMove) * 100.0f) / this->GetMovementSpeed();

	// Set unit to move to target position
	auto MoveToAction = MoveBy::create(timeTaken, distance);
	MoveToAction->setTag(MOVING);
	this->runAction(MoveToAction);

	// If moving toward right
	if (distance.x > 0)
	{
		// Flip the image to face Right
		if (m_type >= SWORDSMAN && m_type <= HERO)
		{
			m_sprite->setFlippedX(false);
		}
		else
		{
			m_sprite->setFlippedX(true);
		}
	}
	else if (distance.x < 0)
	{
		// Flip the image to face Left
		if (m_type >= SWORDSMAN && m_type <= HERO)
		{
			m_sprite->setFlippedX(true);
		}
		else
		{
			m_sprite->setFlippedX(false);
		}
	}

	MoveAnimate();
}

void Unit::MoveAnimate()
{
	RepeatForever*	animateForever = RepeatForever::create(AnimationManager::GetWalkingAnimation(m_type, this->GetMovementSpeed()));
	animateForever->setTag(MOVING);
	m_sprite->runAction(animateForever);
}

void Unit::Attack()
{
	if (m_targetList.size() > 0)
	{
		m_currentState = ATTACKING;

		auto attacktime = DelayTime::create(0.5f);
		attacktime->setTag(ATTACKING);
		this->runAction(attacktime);

		AttackAnimate();

		m_hasShooted = false;

		// If moving toward right
		if (this->getPositionX() < m_targetList[0]->getPositionX())
		{
			// Flip the image to face Right
			if (m_type >= SWORDSMAN && m_type <= HERO)
			{
				m_sprite->setFlippedX(false);
			}
			else
			{
				m_sprite->setFlippedX(true);
			}
		}
		else if (this->getPositionX() > m_targetList[0]->getPositionX())
		{
			// Flip the image to face Left
			if (m_type >= SWORDSMAN && m_type <= HERO)
			{
				m_sprite->setFlippedX(true);
			}
			else
			{
				m_sprite->setFlippedX(false);
			}
		}
		if (m_type == ARCHER || m_type == BOSS_KING)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			//SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/archer_man_attack_sound.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/archer_man_attack_sound.wav");
#endif
		}
		else if (m_type == ENEMY_LIGHTNING_WIZARD || m_type == ENEMY_STRENGTHEN_LIGHTNING_WIZARD)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			//SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/lightning_wizard_attack.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/lightning_wizard_attack.wav");
#endif
		}
		else if (m_type == ENEMY_LIGHTNING_WIZARD || m_type == ENEMY_STRENGTHEN_LIGHTNING_WIZARD)
		{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
			//SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/lightning_wizard_attack.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/lightning_wizard_attack.wav");
#endif
		}
	}
}

void Unit::AttackAnimate()
{
	auto	animateAttack = AnimationManager::GetAttackingAnimation(m_type, 0.5f);
	animateAttack->setTag(ATTACKING);
	m_sprite->runAction(animateAttack);
}

void Unit::WaitToNextAttack()
{
	m_currentState = WAITING_TO_NEXT_ATTACK;

	auto attacktime = DelayTime::create(this->GetAttackSpeed());
	attacktime->setTag(WAITING_TO_NEXT_ATTACK);
	this->runAction(attacktime);

	auto	animateIdling = AnimationManager::GetIdlingAnimation(m_type, 0.5f);
	if (animateIdling)
	{
		RepeatForever*	animateForever = RepeatForever::create(animateIdling);
		animateForever->setTag(WAITING_TO_NEXT_ATTACK);
		m_sprite->runAction(animateForever);
	}
}

bool Unit::CheckIfTargeted(Unit* target)
{
	for (Unit* alreadyTargetedUnit : m_targetList)
	{
		if (target == alreadyTargetedUnit)
			return true;
	}
	return false;
}

void Unit::CheckTargetWhetherToAttack(Unit* target, bool IgnoredY)
{
	if (m_currentState != FROZEN)
	{
		if (!CheckIfTargeted(target))
		{
			// Is hero for boss capture scene
			if (m_type >= HERO_TWO_HANDED_SWORD && m_type <= HERO_BOW)
			{
				// Facing Right
				if (!m_sprite->isFlippedX())
				{
					if (this->getPositionX() > target->getPositionX())
						return;
				}
				// Facing Left
				else
				{
					if (this->getPositionX() < target->getPositionX())
						return;
				}
			}
			float distanceBetweenTarget = 0.0f;
			if (IgnoredY)
			{
				Vec2 distanceBetweenTargetX(this->getPositionX() - target->getPositionX(), 0);
				distanceBetweenTarget = distanceBetweenTargetX.length();
				distanceBetweenTarget -= (m_sprite->getScaleX() * m_sprite->getContentSize().width * 0.25f + target->GetSprite()->getScaleX() * target->GetSprite()->getContentSize().width * 0.25f);
			}
			else
			{
				distanceBetweenTarget = (this->getPosition() - target->getPosition()).length();
				distanceBetweenTarget -= (Vec2(	(m_sprite->getScaleX() * m_sprite->getContentSize().width * 0.25f + target->GetSprite()->getScaleX() * target->GetSprite()->getContentSize().width * 0.25f),
												(m_sprite->getScaleY() * m_sprite->getContentSize().height * 0.25f + target->GetSprite()->getScaleY() * target->GetSprite()->getContentSize().height * 0.25f))).length();
			}
			if (distanceBetweenTarget / m_totalDistanceToMove * 100.f <= this->GetAttackRange())
			{
				m_targetList.push_back(target);
				if (m_type == Unit::BOSS_RICH)
				{
					if (m_currentState != TANKING)
					{
						StopAllActions();
						m_currentState = TANKING;
					}
				}
				else
				{
					if (m_type < HERO_TWO_HANDED_SWORD || m_type > HERO_BOW)
					{
						if (m_currentState != ATTACKING &&
							m_currentState != WAITING_TO_NEXT_ATTACK)
						{
							StopAllActions();
							Attack();
						}
					}
				}
			}
		}
	}
}
void Unit::CheckTargetWhetherToRemove(Unit* target, bool IgnoredY)
{
	if (m_currentState == ATTACKING || m_currentState == WAITING_TO_NEXT_ATTACK)
	{
		if (CheckIfTargeted(target))
		{
			// Is hero for boss capture scene
			if (m_type >= HERO_TWO_HANDED_SWORD && m_type <= HERO_BOW)
			{
				// Facing Right
				if (!m_sprite->isFlippedX())
				{
					if (this->getPositionX() > target->getPositionX())
						RemoveTarget(target, IgnoredY);
				}
				// Facing Left
				else
				{
					if (this->getPositionX() < target->getPositionX())
						RemoveTarget(target, IgnoredY);
				}
			}
			float distanceBetweenTarget = 0.0f;
			if (IgnoredY)
			{
				Vec2 distanceBetweenTargetX(this->getPositionX() - target->getPositionX(), 0);
				distanceBetweenTarget = distanceBetweenTargetX.length();
				distanceBetweenTarget -= (m_sprite->getScaleX() * m_sprite->getContentSize().width * 0.25f + target->GetSprite()->getScaleX() * target->GetSprite()->getContentSize().width * 0.25f);
			}
			else
			{
				distanceBetweenTarget = (this->getPosition() - target->getPosition()).length();
				distanceBetweenTarget -= (Vec2(	(m_sprite->getScaleX() * m_sprite->getContentSize().width * 0.25f + target->GetSprite()->getScaleX() * target->GetSprite()->getContentSize().width * 0.25f),
												(m_sprite->getScaleY() * m_sprite->getContentSize().height * 0.25f + target->GetSprite()->getScaleY() * target->GetSprite()->getContentSize().height * 0.25f))).length();
			}
			if (distanceBetweenTarget / m_totalDistanceToMove * 100.f > this->GetAttackRange())
			{
				RemoveTarget(target, IgnoredY);
			}
		}
	}
}

void Unit::MinusHeallth(int minusBy, bool ignoreDef)
{
	// Electric Shock
	Sprite* electricShock = (Sprite*)this->getChildByTag(ELECTRIC_SHOCK);
	if (electricShock)
	{
		if (electricShock->isVisible())
			minusBy += m_electricShockDamage;
	}
	// Reduced damged
	if (!ignoreDef)
	{
		minusBy -= this->GetDefense();
		if (minusBy <= 0)
		{
			return;
		}
	}

	auto minusHealthEffect = Repeat::create(Sequence::create(TintTo::create(0.045f, Color3B::RED), TintTo::create(0.045f, Color3B::WHITE), NULL), 2);
	minusHealthEffect->setTag(MINUS_HEALTH_EFFECT);
	m_sprite->runAction(minusHealthEffect);

	for (int i = 0; i < m_bonusInfoList.size(); i++)
	{
		if (m_bonusInfoList[i].GetHealth() < minusBy)
		{
			minusBy -= m_bonusInfoList[i].GetHealth();
			m_bonusInfoList[i].SetHealth(0);
		}
		else
		{
			m_bonusInfoList[i].SetHealth(m_bonusInfoList[i].GetHealth() - minusBy);
			if (m_hpbar)
				m_hpbar->SetHealth(this->GetHealth(), this->GetMaxHealth());
			return;
		}
	}
	if (m_info.GetHealth() <= minusBy)
	{
		m_info.SetHealth(0);
		if (m_hpbar)
			m_hpbar->SetHealth(this->GetHealth(), this->GetMaxHealth());
		Reset();
	}
	else
	{
		m_info.SetHealth(m_info.GetHealth() - minusBy);
		if (m_hpbar)
			m_hpbar->SetHealth(this->GetHealth(), this->GetMaxHealth());
	}
}

void Unit::Heal(int healAmount)
{
	// If there is heal
	if (healAmount != 0)
	{
		// If health is less than max health
		if (this->GetHealth() < this->GetMaxHealth())
		{
			// If unit health is less than max health
			if (m_info.GetHealth() < m_info.GetMaxHealth())
			{
				// heal
				m_info.SetHealth(m_info.GetHealth() + healAmount);
				// if after regeneration, current health exceed max Health
				if (m_info.GetHealth() > m_info.GetMaxHealth())
				{
					m_info.SetHealth(m_info.GetMaxHealth());
					// Get the excess heal
					healAmount = m_info.GetHealth() - m_info.GetMaxHealth();
				}
				else
				{
					if (m_hpbar)
						m_hpbar->SetHealth(this->GetHealth(), this->GetMaxHealth());
					return;
				}
			}
			// for each bonus info
			for (int i = 0; i < m_bonusInfoList.size(); i++)
			{
				// heal
				m_bonusInfoList[i].SetHealth(m_info.GetHealth() + healAmount);
				// if after regeneration, current health exceed max Health
				if (m_bonusInfoList[i].GetHealth() > m_bonusInfoList[i].GetMaxHealth())
				{
					m_bonusInfoList[i].SetHealth(m_bonusInfoList[i].GetMaxHealth());
					// Get the excess heal
					healAmount = m_bonusInfoList[i].GetHealth() - m_bonusInfoList[i].GetMaxHealth();
				}
				else
				{
					if (m_hpbar)
						m_hpbar->SetHealth(this->GetHealth(), this->GetMaxHealth());
					return;
				}
			}
			if (m_hpbar)
				m_hpbar->SetHealth(this->GetHealth(), this->GetMaxHealth());

		}
	}
}


void Unit::RemoveTarget(Unit* target, bool MoveIfNoTargets)
{
	if (target)
	{
		if (m_targetList.size() != 0)
		{
			for (int i = 0; i < m_targetList.size(); i++)
			{
				// remove this target if this target is the target
				if (target == m_targetList[i])
				{
					m_targetList.erase(m_targetList.begin() + i);
					break;
				}
			}
			if (MoveIfNoTargets)
			{
				// if no targets left
				if (m_targetList.size() == 0)
				{
					MoveToTarget();
				}
			}
		}
	}
}

void Unit::RemoveAllTargets()
{
	m_targetList.clear();
}


void Unit::KnockUp(float duration, float distanceY, int knockedUpDamage)
{
	StopAllActions();
	m_currentState = KNOCKED_UP;

	auto knockup = JumpBy::create(duration, Vec2(0,0), distanceY, 1);
	knockup->setTag(KNOCKED_UP);
	this->runAction(knockup);

	m_knockedUpDamage = knockedUpDamage;
}

void Unit::Burn(float duration, int damage, float chance)
{
	if (((float)rand() / RAND_MAX * 100.f) < chance)
	{
		ParticleSystem* burner = (ParticleSystem*)this->getChildByTag(BURNING);
		if (!burner)
		{
			burner = ParticleFire::create();

			// Gravity Mode
			burner->setEmitterMode(ParticleSystem::Mode::GRAVITY);
			burner->setEndSize(0.0f);
			// Gravity Mode: gravity
			burner->setGravity(Vec2(0, 10));

			burner->setTag(BURNING);
			burner->setPosition(Vec2(0, (m_sprite->getContentSize().height * m_sprite->getScaleY()) * 0.5f));
			this->addChild(burner, 2);
		}
		if (!burner->getActionByTag(BURNING))
		{
			burner->setStartSize(m_sprite->getContentSize().height * m_sprite->getScaleY() * 0.1f);
			burner->setStartSizeVar(m_sprite->getContentSize().height * m_sprite->getScaleY());
			burner->start();
			auto burning = DelayTime::create(duration);
			burning->setTag(BURNING);
			burner->runAction(burning);

			// Burn damage
			m_burningDamage = damage;

			auto to_burn = DelayTime::create(1.0f);
			to_burn->setTag(TO_BURN);
			this->runAction(to_burn);
		}
	}
}

void Unit::Freeze(float duration, float chance)
{
	if (((float)rand() / RAND_MAX * 100.f) < chance)
	{
		Sprite* ice = (Sprite*)this->getChildByTag(FROZEN);
		if (!ice)
		{
			ice = Sprite::create("Effects/ice.png");
			ice->setAnchorPoint(Point(0.5f, 0.0f)); ice->setTag(FROZEN);
			this->addChild(ice, 2);
		}
		if (!ice->getActionByTag(FROZEN))
		{
			this->StopAllActions();
			m_currentState = FROZEN;


			ice->setScaleX(m_sprite->getContentSize().width * m_sprite->getScaleX() / ice->getContentSize().width);
			ice->setScaleY(m_sprite->getContentSize().height * m_sprite->getScaleY() / ice->getContentSize().height);
			ice->setVisible(true);
			auto freezing = DelayTime::create(duration);
			freezing->setTag(FROZEN);
			ice->runAction(freezing);
		}
	}
}

void Unit::ElectricShock(float duration, int damage, float chance)
{
	if (((float)rand() / RAND_MAX * 100.f) < chance)
	{
		Sprite* electircShock = (Sprite*)this->getChildByTag(ELECTRIC_SHOCK);
		if (!electircShock)
		{
			electircShock = Sprite::create("Effects/spark/spark-1.png");
			electircShock->setAnchorPoint(Point(0.5f, 0.0f)); electircShock->setTag(ELECTRIC_SHOCK);
			this->addChild(electircShock, 2);

			Animation* effectAnimation = Animation::create();
			int numOfSprites = 3;
			for (int i = 1; i <= numOfSprites; i++) {
				effectAnimation->addSpriteFrameWithFile(StringUtils::format("Effects/spark/spark-%d.png", i));
			}
			effectAnimation->setDelayPerUnit(0.3f / (float)numOfSprites);
			Animate*		attackEffectAnimate = Animate::create(effectAnimation);

			electircShock->runAction(RepeatForever::create(attackEffectAnimate));
		}
		if (!electircShock->getActionByTag(ELECTRIC_SHOCK))
		{
			// Electric shock damage
			m_electricShockDamage = damage;


			electircShock->setScaleX(m_sprite->getContentSize().width * m_sprite->getScaleX() / electircShock->getContentSize().width);
			electircShock->setScaleY(m_sprite->getContentSize().height * m_sprite->getScaleY() / electircShock->getContentSize().height);
			electircShock->setVisible(true);
			auto electricShocking = DelayTime::create(duration);
			electricShocking->setTag(ELECTRIC_SHOCK);
			electircShock->runAction(electricShocking);
		}
	}
}

void Unit::Rot(float duration, int corruptedDefense, float chance)
{
	if (((float)rand() / RAND_MAX * 100.f) < chance)
	{
		Sprite* rot = (Sprite*)this->getChildByTag(ROT);
		if (!rot)
		{
			rot = Sprite::create("Effects/rot/rot-1.png");
			rot->setAnchorPoint(Point(0.5f, 0.0f));
			rot->setScaleX(m_sprite->getContentSize().width * m_sprite->getScaleX() / rot->getContentSize().width);
			rot->setScaleY(m_sprite->getContentSize().height * m_sprite->getScaleY() / rot->getContentSize().height);
			rot->setTag(ROT);
			this->addChild(rot, 2);

			Animation* effectAnimation = Animation::create();
			int numOfSprites = 3;
			for (int i = 1; i <= numOfSprites; i++) {
				effectAnimation->addSpriteFrameWithFile(StringUtils::format("Effects/rot/rot-%d.png", i));
			}
			effectAnimation->setDelayPerUnit(0.3f / (float)numOfSprites);
			Animate*		attackEffectAnimate = Animate::create(effectAnimation);

			rot->runAction(RepeatForever::create(attackEffectAnimate));
		}
		if (!rot->getActionByTag(ROT))
		{
			// Corrupted defense
			m_corruptedDefense = corruptedDefense;

			rot->setVisible(true);
			auto rotting = DelayTime::create(duration);
			rotting->setTag(ROT);
			rot->runAction(rotting);
		}
	}
}

void Unit::DamageTargets()
{
	int hitted = 0;
	for (int i = 0; i < m_targetList.size(); i++)
	{
		if ((hitted < 1 && m_type != HERO_SPEAR) ||
			(hitted < 2 && m_type == HERO_SPEAR))
		{
			if (m_targetList[i]->IfActive())
			{
				int damage = this->GetAttackDamage();
				switch (m_type)
				{
					case HERO_TWO_HANDED_SWORD:
					{
						damage += damage * 0.2f;
						break;
					}
					case HERO_BOW:
					{
						damage += 20.f;
						break;
					}
				}
				// Minus target's health
				m_targetList[i]->MinusHeallth(damage);
				hitted++;
			}
			if (!m_targetList[i]->IfActive())
			{
				RemoveTarget(m_targetList[i], false);
			}
		}
	}
}

void Unit::ChangeWeapon(int type)
{
	if (type < HERO_TWO_HANDED_SWORD ||
		type > HERO_BOW)
		return;

	m_type = type;

	// Stop animation
	StopAction(Unit::ATTACKING);
	StopAction(Unit::WAITING_TO_NEXT_ATTACK);
	StopAction(Unit::MOVING);
	SetStopTexture();

	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	int strPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::STR_PLUS_BUTTON)).c_str(), 0);

	switch (m_type)
	{
		case HERO_TWO_HANDED_SWORD:
		{
			m_info.SetAttackDamage(45 + strPoints * 5);
			m_info.SetAttackSpeed(1.5f);
			m_info.SetAttackRange(0.0f);
			m_attacktype = NORMAL;
			return;
		}
		case HERO_SPEAR:
		{
			m_info.SetAttackDamage(50 + strPoints * 5);
			m_info.SetAttackSpeed(3.f);
			m_info.SetAttackRange(0.0f);
			m_attacktype = NORMAL;
			return;
		}
		case HERO_BOW:
		{
			m_info.SetAttackDamage(40 + strPoints * 5);
			m_info.SetAttackSpeed(1.5f);
			m_info.SetAttackRange(20.0f);
			m_attacktype = SHOOTING;
			return;
		}
	}
}


void Unit::CastSpell(float duration)
{
	StopAllActions();
	m_currentState = CASTING_SPELL;

	auto castspell = DelayTime::create(duration);
	castspell->setTag(CASTING_SPELL);
	this->runAction(castspell);
}

void Unit::CheckActivateAttackEffect()
{
	if (m_attacktype == CAST_SKILL || m_attacktype == SHOOTING && m_targetList.size() != 0)
	{
		auto attackTime = (DelayTime*)this->getActionByTag(ATTACKING);
		if (attackTime && attackTime->getElapsed() > 0.25f && !m_hasShooted)
		{
			m_hasShooted = true;
			m_hasHitted = false;
			switch (m_type)
			{
				case BOSS_WIZARD:
				{
					m_attackEffect->setTexture("CharacterAnimations/Enemy/boss/7stage/skill/skill-1.png");
					break;
				}
				case BOSS_KING:
				{
					m_attackEffect->setTexture("CharacterAnimations/Enemy/boss/8stage/arrow/arrow-1.png");
					break;
				}
				case ENEMY_ICE_WIZARD:
				{
					m_attackEffect->setTexture("CharacterAnimations/Enemy/wizard/ice/skill.png");
					break;
				}
				case ENEMY_LIGHTNING_WIZARD:
				{
					m_attackEffect->setTexture("CharacterAnimations/Enemy/wizard/thunder/skill/skill-1.png");
					break;
				}
				case ENEMY_FIRE_WIZARD:
				{
					m_attackEffect->setTexture("CharacterAnimations/Enemy/wizard/fire/skill/skill-1.png");
					break;
				}
				case ENEMY_STRENGTHEN_ICE_WIZARD:
				{
					m_attackEffect->setTexture("CharacterAnimations/Enemy/strengthen_wizard/ice/skill/skill-1.png");
					break;
				}
				case ENEMY_STRENGTHEN_LIGHTNING_WIZARD:
				{
					m_attackEffect->setTexture("CharacterAnimations/Enemy/strengthen_wizard/thunder/skill/skill-1.png");
					break;
				}
				case HERO_BOW:
				{
					m_attackEffect->setTexture("CharacterAnimations/bow/attack/arrow.png");
					break;
				}
			}
			m_attackEffect->setVisible(true);
			auto	animateAttack = AnimationManager::GetAttackingEffectAnimation(m_type, 0.5f);
			animateAttack->setTag(ATTACKING);
			m_attackEffect->runAction(animateAttack);
			if (m_targetList[0]->getPositionX() - this->getPositionX() > 0.0f)
			{
				m_attackEffect->setFlippedX(true);
			}
			else
			{
				m_attackEffect->setFlippedX(false);
			}
			switch (m_attacktype)
			{
				case CAST_SKILL:
				{
					m_attackEffect->setScale(	(m_sprite->getScaleX() * m_sprite->getContentSize().width * 2.0f) / m_attackEffect->getContentSize().width,
												(m_sprite->getScaleY() * m_sprite->getContentSize().height * 2.0f) / m_attackEffect->getContentSize().height);
					
					m_attackEffect->setPosition(m_targetList[0]->getPosition() - this->getPosition());

					auto delay = DelayTime::create(0.5f);
					delay->setTag(ATTACK_SKILL_DURATION);
					m_attackEffect->runAction(delay);
					break;
					break;
				}
				case SHOOTING:
				{
					if (m_type == ENEMY_ICE_WIZARD ||
						m_type == ENEMY_STRENGTHEN_ICE_WIZARD)
					{
						m_attackEffect->setScale(	(m_sprite->getScaleX() * m_sprite->getContentSize().width) / m_attackEffect->getContentSize().width,
													(m_sprite->getScaleY() * m_sprite->getContentSize().height * 0.5f) / m_attackEffect->getContentSize().height);
					}
					else if(m_type == HERO_BOW)
					{
						m_attackEffect->setScale(	(m_sprite->getScaleX() * m_sprite->getContentSize().width * 0.5f) / m_attackEffect->getContentSize().width,
													(m_sprite->getScaleY() * m_sprite->getContentSize().height * 0.05f) / m_attackEffect->getContentSize().height);
					}
					else
					{
						m_attackEffect->setScale(	(m_sprite->getScaleX() * m_sprite->getContentSize().width) / m_attackEffect->getContentSize().width,
													(m_sprite->getScaleY() * m_sprite->getContentSize().height) / m_attackEffect->getContentSize().height);
					}
					m_attackEffect->setPosition(0.0f, m_sprite->getScaleY() * m_sprite->getContentSize().height * 0.5f);

					// Move Arrow
					auto moveTo = MoveTo::create(0.5f, m_targetList[0]->getPosition() - this->getPosition());
					moveTo->setTag(ATTACKING);
					m_attackEffect->runAction(moveTo);
					break;
				}
			}
		}
	}
}

void Unit::CheckAttackEffect()
{
	if (m_attackEffect->isVisible())
	{
		bool canHhit = false;
		if (m_attacktype == CAST_SKILL)
		{
			auto attackSkillTime = (DelayTime*)m_attackEffect->getActionByTag(ATTACK_SKILL_DURATION);
			if (attackSkillTime && attackSkillTime->getElapsed() > 0.25f && !m_hasHitted)
			{
				m_hasHitted = true;
				canHhit = true;
			}
		}
		if (m_attacktype == SHOOTING)
			canHhit = true;
		if (canHhit)
		{
			// numOfHitted
			int hitted = 0;
			Rect attackEffectRect(m_attackEffect->getPositionX() + this->getPositionX(), m_attackEffect->getPositionY() + this->getPositionY(),
				m_attackEffect->getBoundingBox().size.width, m_attackEffect->getBoundingBox().size.height);
			for (int i = 0; i < m_targetList.size(); i++)
			{
				if ((hitted < 1 && m_type != BOSS_WIZARD) ||
					(hitted < 3 && m_type == BOSS_WIZARD))
				{
					if (m_targetList[i]->IfActive())
					{
						Size rectSize(m_targetList[i]->GetSprite()->getBoundingBox().size.width,
							m_targetList[i]->GetSprite()->getBoundingBox().size.height);
						Rect targetRect(m_targetList[i]->getPositionX() + rectSize.width * 0.45f, m_targetList[i]->getPositionY(),
							rectSize.width * 0.1f, rectSize.height);
						if (attackEffectRect.intersectsRect(targetRect))
						{
							// Minus target's health
							m_targetList[i]->MinusHeallth(this->GetAttackDamage());
							hitted++;

						}
					}
				}
				// remove target if target is dead
				if (!m_targetList[i]->IfActive())
				{
					m_targetList.erase(m_targetList.begin() + i);
					i--;
				}
			}
			// if no targets left
			if (m_targetList.size() == 0 && 
				(m_type < HERO_TWO_HANDED_SWORD ||
				m_type > HERO_BOW))
			{
				MoveToTarget();
			}
			if (m_attacktype == SHOOTING && hitted > 0)
			{
				m_attackEffect->stopAllActionsByTag(ATTACKING);
				m_attackEffect->setVisible(false);
			}
		}
		if (!m_attackEffect->getActionByTag(ATTACKING))
		{
			m_attackEffect->setVisible(false);
		}
	}
}