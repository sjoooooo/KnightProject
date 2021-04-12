#ifndef _UNIT_H_
#define _UNIT_H_

#include "UnitInfoBonus.h"
#include "HPBar.h"

// Forward Declaration
class Spawner;

#include "cocos2d.h"
USING_NS_CC;

class Unit : public Node
{
public:
	// State
	enum FSM
	{
		IDLING = 1,
		MOVING,
		ATTACKING,
		WAITING_TO_NEXT_ATTACK,
		KNOCKED_UP,
		TANKING,
		CASTING_SPELL,
		FROZEN,
		REGENEARTION,
		BURNING,
		TO_BURN,
		ELECTRIC_SHOCK,
		ROT,
		MINUS_HEALTH_EFFECT,
		NUM_OF_STATES
	};

	enum SPECIAL_TAG
	{
		ATTACK_SKILL_DURATION = 100,
	};

	enum TYPE
	{
		NIL = 0,
		// Player' Units
		SWORDSMAN = 1,
		ARCHER,
		HORSEMAN,
		SPEARMAN,
		WIZARD,
		SHIELDMAN,
		// Player' weapon : Boss Capture
		HERO_TWO_HANDED_SWORD,
		HERO_SPEAR,
		HERO_BOW,
		HERO,
		CASTLE,
		// Bosses
		BOSS_RICH,
		BOSS_FALLEN_ALCHEMIST,
		BOSS_ESCAPED_PRISONER,
		BOSS_ABANDONDED_KNIGHT,

		BOSS_COMMANDER,
		BOSS_QUEEN,

		BOSS_WIZARD,
		BOSS_KING,
		// Enemy' Units
		ENEMY_SLAVE,
		ENEMY_FARMER,
		ENEMY_PEOPLE,

		ENEMY_BEGGAR,
		ENEMY_BARON,
		ENEMY_ALCHEMIST,

		ENEMY_SHIELDMAN,
		ENEMY_KNIGHT,
		ENEMY_HEAVY_ARMOR_KNIGHT,
		ENEMY_SPEARMAN,
		ENEMY_PLATE_KNIGHT,

		ENEMY_ICE_WIZARD,
		ENEMY_LIGHTNING_WIZARD,
		ENEMY_FIRE_WIZARD,
		
		ENEMY_HEAVY_ARMOR_SPEARMAN,

		ENEMY_STRENGTHEN_ICE_WIZARD,
		ENEMY_STRENGTHEN_LIGHTNING_WIZARD,
		ENEMY_STRENGTHEN_PLATE_KNIGHT,
		ENEMY_STRENGTHEN_SHIELDMAN,

		ENEMY_MASS_UNIT,

		NUM_OF_TYPES,
	};

	enum ATTACK_TYPE
	{
		NORMAL = 0,
		CAST_SKILL,
		SHOOTING,
		NUM_OF_ATTACK_TYPES
	};

protected:
	// Unit sprite
	Sprite* m_sprite;
	// Unit Info
	UnitInfo m_info;
	std::vector<UnitInfoBonus> m_bonusInfoList;
	// Active
	bool m_active;
	// State
	int m_currentState;
	// Target location
	Point m_targetLocation;
	// Spawn location
	Point m_spawnLocation;
	// Timer for regenaration
	float m_timeLeftToNextRegenaration;
	// Hp Bar
	HPBar* m_hpbar;
	// Type
	int m_type;
	std::vector<Unit*> m_targetList;
	// Knocked up damaged by
	int m_knockedUpDamage;
	// Burn damage
	int m_burningDamage;
	// Electric shock damage
	int m_electricShockDamage;
	// Corrupted defense
	int m_corruptedDefense;
	// Attack effect
	Sprite* m_attackEffect;
	// For shooting
	bool m_hasShooted;
	// For casting spell
	bool m_hasHitted;
	// For type
	int m_attacktype;
public:
	Unit();
	virtual ~Unit();

	float m_totalDistanceToMove;
	// Chasing Target
	Unit* chasingTarget;

	// Setters
	void SetSprite(Sprite* sprite);
	void SetInfo(	int health, int maxHealth, int regeneration,
					int attackDamage, float attackSpeed, float attackRange,
					int defense,
					float movementSpeed,
					Point targetLocation,
					Point spawnLocation);
	void SelfSetInfoAccordingToTypeAndLocations(Point targetLocation, Point spawnLocation);
	void SetInfo(UnitInfo* info);
	void AddBonusInfoList(UnitInfoBonus* bonusInfo);
	void AddBonusInfoList(	int health, int maxHealth, int regeneration,
							int attackDamage, float attackSpeed, float attackRange,
							int defense,
							float movementSpeed,
							float timeLeft,
							bool gotWeapon = false);
	void SetActive(bool active);
	void SetState(int currentState);
	void SetTargetLocation(Point targetLocation);
	void SetSpawnLocation(Point spawnLocation);
	void InitHPBar(std::string background, std::string hpbar, std::string outline, bool fixedToUnit = true);
	void ResizeHPBar();
	void SetType(int type);

	// Getters
	Sprite* GetSprite();
	Sprite* GetAttackEffect();
	UnitInfo* GetInfo();
	std::vector<UnitInfoBonus>* GetBonusInfoList();
	bool IfActive();
	int GetState();
	Point GetTargetLocation();
	Point GetSpawnLocation();
	int GetType();
	HPBar* GetHPBar();
	// Get Total Info
	int GetHealth();
	int GetMaxHealth();
	int GetRegeneration();
	int GetAttackDamage();
	float GetAttackSpeed();
	float GetAttackRange();
	int GetDefense();
	float GetMovementSpeed();

	// Others
	void Update(float dt);
	void UpdateOthers(float dt);
	void UpdateBonusInfo(float dt);

	void Reset();

	void StopAction(int state);
	void StopAllActions(bool includingMinusHealthEffect = false);
	void SetStopTexture();

	void Idle(float duration);

	void MoveToTarget();
	void MoveToLocation(Vec2 location);
	void MoveBy(Vec2 distance);
	void MoveAnimate();

	void Attack();
	void AttackAnimate();

	void WaitToNextAttack();

	bool CheckIfTargeted(Unit* target);
	void CheckTargetWhetherToAttack(Unit* target, bool IgnoredY = true);
	void CheckTargetWhetherToRemove(Unit* target, bool IgnoredY = true);

	void MinusHeallth(int minusBy, bool ignoreDef = false);
	void Heal(int healAmount);

	void RemoveTarget(Unit* target, bool MoveIfNoTargets = true);
	void RemoveAllTargets();

	void KnockUp(float duration, float distanceY, int knockedUpDamage);

	void Burn(float duration, int damage, float chance = 100.f);

	void Freeze(float duration, float chance = 100.f);

	void ElectricShock(float duration, int damage, float chance = 100.f);

	void Rot(float duration, int corruptedDefense, float chance = 100.f);

	void DamageTargets();

	void ChangeWeapon(int type);

	void CastSpell(float duration);

	void CheckActivateAttackEffect();
	void CheckAttackEffect();
};

#endif 
