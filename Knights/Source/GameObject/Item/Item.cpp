#include "Item.h"
#include "..\Skills\Skill.h"

Item::Item()
{
}

Item::~Item()
{
}

// Others
void Item::SetItemEffect_Info(int type, UnitInfo* info)
{
	if (!info)
		return;

	switch (type)
	{
		case Skill::ARCHER:
		{
			info->SetHealth(info->GetHealth() + 20);
			info->SetMaxHealth(info->GetMaxHealth() + 20);

			info->SetDefense(info->GetDefense() + 3);

			info->SetAttackDamage(info->GetAttackDamage() + 5);

			info->SetAttackRange(info->GetAttackRange() + 13.0f);
			return;
		}
		case Skill::SPEARMAN:
		{
			info->SetHealth(info->GetHealth() + 30);
			info->SetMaxHealth(info->GetMaxHealth() + 30);

			info->SetDefense(info->GetDefense() + 8);

			info->SetAttackDamage(info->GetAttackDamage() + 5);

			info->SetAttackRange(info->GetAttackRange() + 2.0f);
			return;
		}
		case Skill::SWORDSMAN:
		{
			info->SetHealth(info->GetHealth() + 10);
			info->SetMaxHealth(info->GetMaxHealth() + 10);

			info->SetDefense(info->GetDefense() + 5);

			info->SetAttackDamage(info->GetAttackDamage() + 3);
			return;
		}
		case Skill::SHIELD:
		{
			info->SetHealth(info->GetHealth() + 30);
			info->SetMaxHealth(info->GetMaxHealth() + 30);

			info->SetDefense(info->GetDefense() + 10);

			info->SetAttackDamage(info->GetAttackDamage() + 4);
			return;
		}
		case Skill::WIZARD:
		{
			info->SetHealth(info->GetHealth() + 10);
			info->SetMaxHealth(info->GetMaxHealth() + 10);

			info->SetDefense(info->GetDefense() + 4);

			info->SetAttackDamage(info->GetAttackDamage() + 10);

			info->SetAttackRange(info->GetAttackRange() + 4.0f);
			return;
		}
		case Skill::HORSEMAN:
		{
			info->SetHealth(info->GetHealth() + 20);
			info->SetMaxHealth(info->GetMaxHealth() + 20);

			info->SetDefense(info->GetDefense() + 7);

			info->SetAttackDamage(info->GetAttackDamage() + 5);

			info->SetAttackRange(info->GetAttackRange() + 3.0f);
			return;
		}
	}
}
void Item::SetItemEffect_BonusInfo(int type, UnitInfoBonus* info)
{
	if (!info)
		return;

	switch (type)
	{
		case Skill::HEAL:
		{
			info->SetRegeneration(info->GetRegeneration() + 15);
			return;
		}
	}
}
int Item::GetItemEffect_Damage(int type)
{
	switch (type)
	{
		case Skill::ARROW:		{ return 10; }
		case Skill::LIGHTNING:	{ return 10; }
		case Skill::METEOR:		{ return 5; }
		case Skill::ICE_SPEAR:	{ return 10; }
		case Skill::HURRICANE:	{ return 10; }
		case Skill::FIREBALL:	{ return 5; }
	}
	return 0;
}

int Item::GetItemEffect_Heal(int type)
{
	switch (type)
	{
		case Skill::RECOVERY_POTION:{ return 100; }
	}
	return 0;
}

float Item::GetItemEffect_Cooldown(int type)
{
	switch (type)
	{
		case Skill::ARCHER			:{ return -2.0f; }
		case Skill::ARROW			:{ return -2.0f; }
		case Skill::BACKSTEP		:{ return -2.0f; }
		case Skill::SPEARMAN		:{ return -3.0f; }
		case Skill::SWORDSMAN		:{ return -1.0f; }
		case Skill::LIGHTNING		:{ return -2.0f; }
		case Skill::SHIELD			:{ return -3.0f; }
		case Skill::METEOR			:{ return -2.0f; }
		case Skill::WIZARD			:{ return -4.0f; }
		case Skill::ICE_SPEAR		:{ return -2.0f; }
		case Skill::HURRICANE		:{ return -2.0f; }
		case Skill::HORSEMAN		:{ return -2.0f; }
		case Skill::HEAL			:{ return -5.0f; }
		case Skill::RECOVERY_POTION	:{ return -5.0f; }
		case Skill::FIREBALL		:{ return -3.0f; }
	}
	return 0.0f;
}
float Item::GetItemEffect_Gauge(int type)
{
	switch (type)
	{
		case Skill::ARCHER			:{ return 0.0f; }
		case Skill::ARROW			:{ return -10.0f; }
		case Skill::BACKSTEP		:{ return 0.0f; }
		case Skill::SPEARMAN		:{ return 0.0f; }
		case Skill::SWORDSMAN		:{ return 0.0f; }
		case Skill::LIGHTNING		:{ return 0.0f; }
		case Skill::SHIELD			:{ return 0.0f; }
		case Skill::METEOR			:{ return 0.0f; }
		case Skill::WIZARD			:{ return 0.0f; }
		case Skill::ICE_SPEAR		:{ return 0.0f; }
		case Skill::HURRICANE		:{ return 0.0f; }
		case Skill::HORSEMAN		:{ return 0.0f; }
		case Skill::HEAL			:{ return 0.0f; }
		case Skill::RECOVERY_POTION	:{ return 0.0f; }
		case Skill::FIREBALL		:{ return 0.0f; }
	}
	return 0.0f;
}
int Item::GetPrice(int type)
{
	switch (type)
	{
		case Skill::ARCHER			:{ return 1300; }
		case Skill::ARROW			:{ return 1200; }
		case Skill::BACKSTEP		:{ return 500; }
		case Skill::SPEARMAN		:{ return 1800; }
		case Skill::SWORDSMAN		:{ return 800; }
		case Skill::LIGHTNING		:{ return 3000; }
		case Skill::SHIELD			:{ return 2200; }
		case Skill::METEOR			:{ return 1800; }
		case Skill::WIZARD			:{ return 2400; }
		case Skill::ICE_SPEAR		:{ return 2700; }
		case Skill::HURRICANE		:{ return 1600; }
		case Skill::HORSEMAN		:{ return 1600; }
		case Skill::HEAL			:{ return 500; }
		case Skill::RECOVERY_POTION	:{ return 500; }
		case Skill::FIREBALL		:{ return 1500; }
	}
	return 0;
}