#ifndef __BOSSCAP_SCENE_H__
#define __BOSSCAP_SCENE_H__

#include "..\Managers\CollisionManager.h"


#include "cocos2d.h"
USING_NS_CC;

class BossCapScene : public cocos2d::Layer
{
private:
	// Timer
	float totalDuration;
	
	enum TAG
	{
		//Timer
		TIMER,

		// GUI
		GUI,
		CHANGE_WEAPON_BUTTON,
		SPECIAL_SKILL,

		// Joystick
		JOY_STICK_BACKGROUND_MOVEMENT,
		JOY_STICK_MOVEMENT,
		JOY_STICK_BACKGROUND_ATTACK,
		JOY_STICK_ATTACK,

		GAMEPLAY_MENU,
		HP_BAR,
		MP_BAR,
		CURRENT_EQUIPPED_WEAPON,


		// Pause Window
		PAUSE_BUTTON,
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

		PLAYER,
		SPAWNER_PLAYER,
		SPAWNER_ENEMY,
		BOSS,
		PLAYER_UNITS,
		ENEMY_UNITS = 1000,
	};
	enum GAME_STATE
	{
		PRESTART = 0,
		PLAYING,
		PAUSED
	};
	enum DIRECTION
	{
		NO_DIRECTION = -1,
		LEFT = 0,
		RIGHT,
		UP,
		DOWN,
		NUM_OF_DIRECTION
	};

	int currentState;
private:
	Size visibleSize;
	Vec2 origin;

	Vec2 spawnPositionLeft;
	Vec2 spawnPositionRight;
	float maxOffsetY;
	/* Player */
	Vec2 playerOffset;
	float prevPlayerAnimateSpeed;
	int currentFrameIndexForWalkingAnimation;
	Size playerSize;
	int playerMaxMP;
	int playerMPRecoverySpeed;

	void UpdatePlayer(float deltaTime);
	void WalkAnimate(float speed);
	void PlayerDamageEnemies();
	void Attack();
	void WaitingToAttack();
	void AttackAnimate(float speed);

	void UseMp(int usageAmount);
	/**********/

	// units
	UnitManager* unitManager;
	// Collision
	CollisionManager* collisionManager;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	/* Joystick */
	// Movement
	bool touchedMovementJoystick;
	float joystickMovementMaxOffset;
	Touch* touchedMovement;

	void MoveMovementJoystick(Point location);
	void ResetMovementJoystick();

	// Atttack
	bool touchedAttackJoystick;
	float joystickAttackMaxOffset;
	Touch* touchedAttack;

	void MoveAttackJoystick(Point location);
	void ResetAttackJoystick();
	/************/

	void SpecialSkill(Ref* pSender);
	void ChangeWeapon(Ref* pSender);
public:
	void onTouchesBegan(const std::vector<Touch*>& touches, Event* unused_event);
	void onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event);
	void onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event);
private:
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	int player_movement_direction;
	int player_attack_direction;
	enum TOTAL_NUM
	{
		NUM_OF_KEYS = (int)EventKeyboard::KeyCode::KEY_PLAY + 1,
	};
	bool keyONHold[NUM_OF_KEYS];

	void CheckMovementKeysOnHold();
	void CheckAttackKeysOnHold();

public:
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
private:
#endif
	void StopAttack();
	void SpecialSkill();
	void ChangeWeapon();
	/* Camera */
	float cameraOffsetXFromScene;
	float cameraMaxOffsetX;
	float cameraMinOffsetX;

	bool CheckCameraIfExceedContraint(bool right);
	void ConstraintCamera();
	void UpdateCamera(bool movingTowardRight);
	void MoveCameraX(float moveByX);
	/**********/
	void Pause(Ref* pSender);
	void Resume(Ref* pSender);
	void Restart(Ref* pSender);
	void Quit(Ref* pSender);
	void WinNext(Ref* pSender);
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float delta) override;

	void InitSound();
	void InitBackGround();
	void GameEnd(bool win);
	void InitGUI();
	void InitGameObjects();
    CREATE_FUNC(BossCapScene);
};

#endif 
