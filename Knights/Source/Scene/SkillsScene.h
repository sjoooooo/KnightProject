#ifndef __SKILL_SCENE_H__
#define __SKILL_SCENE_H__

#include "..\Managers\CollisionManager.h"
#include "..\GameObject\Spawner\AISpawner.h"

#include "cocos2d.h"
USING_NS_CC;

class SkillsScene : public cocos2d::Layer
{
private:
	Size visibleSize;
	Vec2 origin;

	// Skills
	Rect skillbarBackground;

	bool touchedSkillbar;
	bool movedSkillbar;
	Point prevTouchLocation;

	SkillEffectManager* skillEffectManager;

	// units
	UnitManager* unitManager;
	//Size heroSize;
	Size unitSize;
	Vec2 spawnPositionLeft;
	Vec2 spawnPositionRight;
	Vec2 heroOffset;
	Size heroSize;
	bool bossSpawned;
	float bossSpawnTime;

	bool bossCastedSpell;
	float bossCastSpellTime;

	// Collision
	CollisionManager* collisionManager;

	// Background
	float cameraMaxX;
	float cameraMinX;

	// Timer
	float totalDuration;

	// Skill guage
	int maxSkillGuage;
	float skillGuageRecoverySpeed;

	enum TAG
	{
		// GUI
		GUI = 1,
		GAMEPLAY_MENU,
		SKILLBAR,
		INCOMING_WAVE_INFO,
		PAUSE_BUTTON,
		// Pause Window
		PAUSE_WINDOW,
		RESUME_BUTTON,
		RESTART_BUTTON,
		QUIT_BUTTON,
		BACKGROUND,
		// Won Window
		WON_WINDOW,
		NEXT_BUTTON,
		FOREGROUND,
		// Lost Window
		LOST_WINDOW,

		// Timer
		TIMER,
		COUNTDOWN,

		// Skill Guage
		SKILLGUAGE,
		SKILLGUAGEBAR,

		// Units
		PLAYER_CHARACTER,
		BOSS,
		ENEMY_CASTLE,

		// SPAWNERS
		SPAWNER_PLAYER,
		SPAWNER_ENEMY,
		AI_SPAWNER_ENEMY,

		// Must Be Last
		PLAYER_UNITS = 100,
		ENEMY_UNITS = PLAYER_UNITS + 200
	};

	enum GAME_STATE
	{
		PRESTART = 0,
		PLAYING,
		PAUSED
	};

	int currentCountdownNumber;
	int currentState;

	// Movement
	/*void MovePlayerCharacter(bool towardLeft);
	void StopMovingPlayerCharacter();*/

	// Camera
	void MoveCameraX(float moveByX);

	void GameEnd(bool win);

	bool Skillusage(int usage);

	void SpawnAllies();
	void SpawnBoss();

	void Pause(Ref* pSender);
	void Resume(Ref* pSender);
	void Restart(Ref* pSender);
	void Quit(Ref* pSender);
	void WinNext(Ref* pSender);
public:
	~SkillsScene();
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();
	void InitGUI();
	void InitGameObject();
	void InitEnemies(Size castleSize);
	void InitEnemySpawnPatterns(AISpawner* enemyAISpawner);

	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onTouchMoved(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);

	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	CREATE_FUNC(SkillsScene);
};

#endif 
