#include "AnimationManager.h"
#include "..\GameObject\Units\Unit.h"
#include "..\GameObject\\Skills\Skill.h"

AnimationManager::AnimationManager()
{
}

AnimationManager::~AnimationManager()
{
}

// Getters
Animate* AnimationManager::GetWalkingAnimation(int tag, float speed)
{
	Animation* walkingAnimation = Animation::create();
	switch (tag)
	{
		case Unit::SWORDSMAN:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/swordsman/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ARCHER:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/archer/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::HORSEMAN:
		{
			int numOfSprites = 11;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/horseman/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::SPEARMAN:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/spearman/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/wizard/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::SHIELDMAN:
		{
			int numOfSprites = 7;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/shieldman/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::HERO_TWO_HANDED_SWORD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/walk_sd/walk_sd-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::HERO_SPEAR:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/spear/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::HERO_BOW:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/bow/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::HERO:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/walk_sd/walk_sd-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		// Bosses
		case Unit::BOSS_RICH:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/1stage/walk/walk%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_FALLEN_ALCHEMIST:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/2stage/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_ESCAPED_PRISONER:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/3stage/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/4stage/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_COMMANDER:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/5stage/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_QUEEN:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/6stage/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/7stage/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_KING:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/8stage/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		// Enemy units
		case Unit::ENEMY_SLAVE:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/slave/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_FARMER:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/farmer/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_PEOPLE:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/people/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_BEGGAR:
		{
			int numOfSprites = 4;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/beggar/walk/walk-%d.png", i));
			}	
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_BARON:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/baron/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_ALCHEMIST:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/enemy_alchemist/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_SHIELDMAN:
		{
			int numOfSprites = 7;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/enemy_shieldman/shieldman/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_KNIGHT:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/knight/knight/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_HEAVY_ARMOR_KNIGHT:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/knight/heavy_armor_knight/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_SPEARMAN:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/spearman/spearman/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_PLATE_KNIGHT:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/knight/knight/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_ICE_WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/wizard/ice/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_LIGHTNING_WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/wizard/thunder/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_FIRE_WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/wizard/fire/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_HEAVY_ARMOR_SPEARMAN:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/spearman/heavy_armor_spearman/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_STRENGTHEN_ICE_WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/strengthen_wizard/ice/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/strengthen_wizard/thunder/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/knight/strengthen_plate_knight/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_STRENGTHEN_SHIELDMAN:
		{
			int numOfSprites = 7;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/enemy_shieldman/strengthen_shieldman/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_MASS_UNIT:
		{
			int numOfSprites = 4;
			for (int i = 1; i <= numOfSprites; i++) {
				walkingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/crowd/walk/walk-%d.png", i));
			}
			walkingAnimation->setDelayPerUnit((float)numOfSprites / speed / (float)numOfSprites);
			break;
			break;
		}
	}
	Animate*		walkingAnimate = Animate::create(walkingAnimation);
	walkingAnimate->setTag(Unit::MOVING);

	return walkingAnimate;
}

Animate* AnimationManager::GetAttackingAnimation(int tag, float speed)
{
	Animation* attackAnimation = Animation::create();
	switch (tag)
	{
		case Unit::SWORDSMAN:
		{
			int numOfSprites = 9;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/swordsman/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ARCHER:
		{
			int numOfSprites = 22;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/archer/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::HORSEMAN:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/horseman/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::SPEARMAN:
		{
			int numOfSprites = 12;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/spearman/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::WIZARD:
		{
			int numOfSprites = 14;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/wizard/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::SHIELDMAN:
		{
			int numOfSprites = 9;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Unit/shieldman/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::HERO_TWO_HANDED_SWORD:
		{
			int numOfSprites = 9;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/attack_sd/attack_sd-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::HERO_SPEAR:
		{
			int numOfSprites = 7;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/spear/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::HERO_BOW:
		{
			int numOfSprites = 7;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/bow/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::HERO:
		{
			int numOfSprites = 9;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/attack_sd/attack_sd-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_RICH:
		{
			return NULL;
			break;
		}
		case Unit::BOSS_FALLEN_ALCHEMIST:
		{
			int numOfSprites = 31;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/2stage/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_ESCAPED_PRISONER:
		{
			int numOfSprites = 12;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/3stage/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			int numOfSprites = 12;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/4stage/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_COMMANDER:
		{
			int numOfSprites = 24;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/5stage/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_QUEEN:
		{
			int numOfSprites = 18;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/6stage/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_WIZARD:
		{
			int numOfSprites = 25;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/7stage/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_KING:
		{
			int numOfSprites = 12;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/8stage/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		// Enemy Units
		case Unit::ENEMY_SLAVE:
		{
			int numOfSprites = 13;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/slave/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_FARMER:
		{
			int numOfSprites = 9;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/farmer/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_PEOPLE:
		{
			int numOfSprites = 17;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/people/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_BEGGAR:
		{
			int numOfSprites = 9;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/beggar/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_BARON:
		{
			int numOfSprites = 18;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/baron/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_ALCHEMIST:
		{
			int numOfSprites = 20;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/enemy_alchemist/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_SHIELDMAN:
		{
			int numOfSprites = 9;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/enemy_shieldman/shieldman/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_KNIGHT:
		{
			int numOfSprites = 10;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/knight/knight/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_HEAVY_ARMOR_KNIGHT:
		{
			int numOfSprites = 10;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/knight/heavy_armor_knight/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_SPEARMAN:
		{
			int numOfSprites = 12;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/spearman/spearman/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_PLATE_KNIGHT:
		{
			int numOfSprites = 10;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/knight/knight/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_ICE_WIZARD:
		{
			int numOfSprites = 5;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/wizard/ice/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_LIGHTNING_WIZARD:
		{
			int numOfSprites = 5;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/wizard/thunder/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_FIRE_WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/wizard/fire/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_HEAVY_ARMOR_SPEARMAN:
		{
			int numOfSprites = 12;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/spearman/heavy_armor_spearman/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}

		case Unit::ENEMY_STRENGTHEN_ICE_WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/strengthen_wizard/ice/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD:
		{
			int numOfSprites = 5;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/strengthen_wizard/thunder/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT:
		{
			int numOfSprites = 10;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/knight/strengthen_plate_knight/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_STRENGTHEN_SHIELDMAN:
		{
			int numOfSprites = 9;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/enemy_shieldman/strengthen_shieldman/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_MASS_UNIT:
		{
			int numOfSprites = 5;
			for (int i = 1; i <= numOfSprites; i++) {
				attackAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/crowd/attack/attack-%d.png", i));
			}
			attackAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
	}
	Animate*		attackAnimate = Animate::create(attackAnimation);
	attackAnimate->setTag(Unit::ATTACKING);

	return attackAnimate;
}

Animate* AnimationManager::GetAttackingEffectAnimation(int tag, float speed)
{
	Animation* attackEffectAnimation = Animation::create();
	switch (tag)
	{
		case Unit::BOSS_WIZARD:
		{
			int numOfSprites = 25;
			for (int i = 1; i <= numOfSprites; i++) {
				attackEffectAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/7stage/skill/skill-%d.png", i));
			}
			attackEffectAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::BOSS_KING:
		{
			int numOfSprites = 4;
			for (int i = 1; i <= numOfSprites; i++) {
				attackEffectAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/8stage/arrow/arrow-%d.png", i));
			}
			attackEffectAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_ICE_WIZARD:
		{
			int numOfSprites = 1;
			for (int i = 1; i <= numOfSprites; i++) {
				attackEffectAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/wizard/ice/skill.png", i));
			}
			attackEffectAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_LIGHTNING_WIZARD:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				attackEffectAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/wizard/thunder/skill/skill-%d.png", i));
			}
			attackEffectAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_FIRE_WIZARD:
		{
			int numOfSprites = 4;
			for (int i = 1; i <= numOfSprites; i++) {
				attackEffectAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/wizard/fire/skill/skill-%d.png", i));
			}
			attackEffectAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_STRENGTHEN_ICE_WIZARD:
		{
			int numOfSprites = 4;
			for (int i = 1; i <= numOfSprites; i++) {
				attackEffectAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/strengthen_wizard/ice/skill/skill-%d.png", i));
			}
			attackEffectAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD:
		{
			int numOfSprites = 9;
			for (int i = 1; i <= numOfSprites; i++) {
				attackEffectAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/strengthen_wizard/thunder/skill/skill-%d.png", i));
			}
			attackEffectAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Unit::HERO_BOW:
		{
			int numOfSprites = 1;
			for (int i = 1; i <= numOfSprites; i++) {
				attackEffectAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/bow/attack/arrow.png", i));
			}
			attackEffectAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		default:
		{
			return NULL;
		}
	}
	Animate*		attackEffectAnimate = Animate::create(attackEffectAnimation);
	attackEffectAnimate->setTag(Unit::ATTACKING);

	return attackEffectAnimate;
}

Animate* AnimationManager::GetIdlingAnimation(int tag, float speed)
{
	Animation* idlingAnimation = Animation::create();
	switch (tag)
	{
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			int numOfSprites = 4;
			for (int i = 1; i <= numOfSprites; i++) {
				idlingAnimation->addSpriteFrameWithFile(StringUtils::format("CharacterAnimations/Enemy/boss/4stage/stop/stop-%d.png", i));
			}
			idlingAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		default:
		{
			return NULL;
		}
	}
	Animate*		idlingAnimate = Animate::create(idlingAnimation);
	idlingAnimate->setTag(Unit::IDLING);

	return idlingAnimate;
}

Animate* AnimationManager::GetSkillAnimation(int tag, float speed)
{
	Animation* skillAnimation = Animation::create();
	switch (tag)
	{
		case Skill::ARROW:
		{
			int numOfSprites = 13;
			for (int i = 1; i <= numOfSprites; i++) {
				skillAnimation->addSpriteFrameWithFile(StringUtils::format("SkillImage/arrow/arrow-%d.png", i));
			}
			skillAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Skill::LIGHTNING:
		{
			int numOfSprites = 28;
			for (int i = 1; i <= numOfSprites; i++) {
				skillAnimation->addSpriteFrameWithFile(StringUtils::format("SkillImage/lightning/lightning-%d.png", i));
			}
			skillAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Skill::METEOR:
		{
			int numOfSprites = 8;
			for (int i = 1; i <= numOfSprites; i++) {
				skillAnimation->addSpriteFrameWithFile(StringUtils::format("SkillImage/meteor/meteor-%d.png", i));
			}
			skillAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Skill::ICE_SPEAR:
		{
			int numOfSprites = 3;
			for (int i = 1; i <= numOfSprites; i++) {
				skillAnimation->addSpriteFrameWithFile(StringUtils::format("SkillImage/ice_spear/ice_spear-%d.png", i));
			}
			skillAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Skill::FIREBALL:
		{
			int numOfSprites = 5;
			for (int i = 1; i <= numOfSprites; i++) {
				skillAnimation->addSpriteFrameWithFile(StringUtils::format("SkillImage/fire_ball/fire_ball-%d.png", i));
			}
			skillAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
		case Skill::HURRICANE:
		{
			int numOfSprites = 12;
			for (int i = 1; i <= numOfSprites; i++) {
				skillAnimation->addSpriteFrameWithFile(StringUtils::format("SkillImage/hurricane/hurricane-%d.png", i));
			}
			skillAnimation->setDelayPerUnit(speed / (float)numOfSprites);
			break;
		}
	}
	Animate* askillAnimate = Animate::create(skillAnimation);

	return askillAnimate;
}