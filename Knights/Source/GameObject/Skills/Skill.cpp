#include "Skill.h"
#include "..\Item\Item.h"

Skill::Skill()
	: m_type(Unit::NIL)
	, m_boughtItem(false)
{
}

Skill::~Skill()
{
}

// Others
void Skill::CalculateInfo(int tag)
{
	this->setTag(tag);

	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	std::stringstream ss1;
	ss1 << "Data" << currentData << "_ItemSelected" << this->getTag();
	m_boughtItem = def->getBoolForKey(ss1.str().c_str());
	
	// Calculate Cost
	CalculateCost();
	// Calculate Unit type
	CalculateUnitType();
	// Calculate Cooldown
	CalculateCooldown();
}

void Skill::Summon(Size* size, Vec2* targetPosition, Spawner* spawner)
{
	// Set Info and image depends on the unit type
	spawner->Spawn(size, targetPosition, m_type);
	this->runAction(ProgressFromTo::create(m_coolDown, 0.0f, 100.0f));
	/*this->stopAllActions();
	this->runAction(ProgressFromTo::create(0.f, 0.0f, 100.0f));*/
}
void Skill::Cast(Unit* hero, SkillEffect* skillEffect, Vec2* targetPos, Size* size)
{
	switch (this->getTag())
	{
		// HEALING
		case HEAL:
		{
			hero->AddBonusInfoList(0, 0, 20, 0, 0, 0, 0, 0, 10);
			if (m_boughtItem)
			{
				Item::SetItemEffect_BonusInfo(this->getTag(), &hero->GetBonusInfoList()->back());
			}
			break;
		}
		case RECOVERY_POTION:
		{
			hero->Heal(100);
			if (m_boughtItem)
			{
				hero->Heal(Item::GetItemEffect_Heal(this->getTag()));
			}
			break;
		}
		// AOE
		case ARROW:
		{
			if (skillEffect && size)
			{
				skillEffect->setPosition(hero->GetSpawnLocation());
				skillEffect->GetSprite()->setTag(this->getTag());
				skillEffect->Activate(*targetPos, *size);
			}
			else
				return;
			break;
		}
		case LIGHTNING:
		{
			if (skillEffect && size)
			{
				skillEffect->GetSprite()->setTag(this->getTag());
				skillEffect->Activate(*targetPos, *size);
			}
			else
				return;
			break;
		}
		case METEOR:
		{
			if (skillEffect && size)
			{
				skillEffect->GetSprite()->setTag(this->getTag());
				skillEffect->Activate(*targetPos, *size);
			}
			else
				return;
			break;
		}
		case HURRICANE:
		{
			if (skillEffect && size)
			{
				skillEffect->GetSprite()->setTag(this->getTag());
				skillEffect->Activate(*targetPos, *size);
			}
			else
				return;
			break;
		}
		case ICE_SPEAR:
		{
			if (skillEffect && size)
			{
				skillEffect->setPosition(hero->getPosition());
				skillEffect->GetSprite()->setTag(this->getTag());
				skillEffect->Activate(*targetPos, *size);
			}
			else
				return;
			break;
		}
		case FIREBALL:
		{
			if (skillEffect && size)
			{
				skillEffect->setPosition(hero->getPosition());
				skillEffect->GetSprite()->setTag(this->getTag());
				skillEffect->Activate(*targetPos, *size);
			}
			else
				return;
			break;
		}
	}
	this->runAction(ProgressFromTo::create(m_coolDown, 0.0f, 100.0f));
	//this->stopAllActions();
	//this->runAction(ProgressFromTo::create(0.f, 0.0f, 100.0f));
}

void Skill::CalculateCost()
{
	//m_cost = 0;
	//return;
	switch (this->getTag())
	{
		// Summon
		case SWORDSMAN:
		{
			m_cost = 10;
			return;
		}
		case ARCHER:
		{
			m_cost = 15;
			return;
		}
		case HORSEMAN:
		{
			m_cost = 25;
			return;
		}
		case SPEARMAN:
		{
			m_cost = 35;
			return;
		}
		case WIZARD:
		{
			m_cost = 50;
			return;
		}
		case SHIELD:
		{
			m_cost = 30;
			return;
		}
		// HEALING
		case HEAL:
		{
			m_cost = 15;
			return;
		}
		case RECOVERY_POTION:
		{
			m_cost = 15;
			return;
		}
		// AOE
		case ARROW:
		{
			m_cost = 25;
			return;
		}
		case LIGHTNING:
		{
			m_cost = 40;
			return;
		}
		case METEOR:
		{
			m_cost = 50;
			return;
		}
		case HURRICANE:
		{
			m_cost = 25;
			return;
		}
		case ICE_SPEAR:
		{
			m_cost = 45;
			return;
		}
		case FIREBALL:
		{
			m_cost = 35;
			return;
		}
		// backstep
		case BACKSTEP:
		{
			m_cost = 5;
			return;
		}
	}
	if (m_boughtItem)
	{
		m_cost += Item::GetItemEffect_Gauge(this->getTag());
	}
}

float Skill::GetCost()
{
	return m_cost;
}

void Skill::CalculateUnitType()
{
	switch (this->getTag())
	{
		case SWORDSMAN:
		{
			m_type = Unit::SWORDSMAN;
			break;
		}
		case ARCHER:
		{
			m_type = Unit::ARCHER;
			break;
		}
		case HORSEMAN:
		{
			m_type = Unit::HORSEMAN;
			break;
		}
		case SPEARMAN:
		{
			m_type = Unit::SPEARMAN;
			break;
		}
		case WIZARD:
		{
			m_type = Unit::WIZARD;
			break;
		}
		case SHIELD:
		{
			m_type = Unit::SHIELDMAN;
			break;
		}
	}
}
void Skill::CalculateCooldown()
{
	switch (this->getTag())
	{
		case SWORDSMAN:
		{
			m_coolDown = 3.0f;
			break;
		}
		case ARCHER:
		{
			m_coolDown = 8.0f;
			break;
		}
		case HORSEMAN:
		{
			m_coolDown = 15.0f;
			break;
		}
		case SPEARMAN:
		{
			m_coolDown = 25.0f;
			break;
		}
		case WIZARD:
		{
			m_coolDown = 30.0f;
			break;
		}
		case SHIELD:
		{
			m_coolDown = 18.0f;
			break;
		}
		// HEALING
		case HEAL:
		{
			m_coolDown = 10.0f;
			break;
		}
		case RECOVERY_POTION:
		{
			m_coolDown = 10.0f;
			break;
		}
		// AOE
		case ARROW:
		{
			m_coolDown = 20.0f;
			break;
		}
		case LIGHTNING:
		{
			m_coolDown = 35.0f;
			break;
		}
		case METEOR:
		{
			m_coolDown = 30.0f;
			break;
		}
		case HURRICANE:
		{
			m_coolDown = 25.0f;
			break;
		}
		case ICE_SPEAR:
		{
			m_coolDown = 30.0f;
			break;
		}
		case FIREBALL:
		{
			m_coolDown = 20.0f;
			break;
		}
		// backstep
		case BACKSTEP:
		{
			m_coolDown = 5;
			return;
		}
	}
	if (m_boughtItem)
	{
		m_coolDown += Item::GetItemEffect_Cooldown(this->getTag());
	}
}