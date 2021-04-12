#include "BossCapScene.h"
#include "SkillsScene.h"
#include "IntroScene.h"
#include "StatScene.h"
#include "BossSelectionScene.h"
#include "LoyaltySelectionScene.h"
#include "MercenaryShopScene.h"
#include "SimpleAudioEngine.h"
#include "GameObject\Units\Hero.h"
#include "GameObject\Skills\SkillBar.h"
#include "GameObject\Spawner\AISpawner.h"
#include "GameObject\WaveInfo\IncomingWavesInfo.h"
#include "..\Managers\AnimationManager.h"
//사운드 할라면 꼭 필요
using namespace CocosDenshion;
USING_NS_CC;

Scene* BossCapScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BossCapScene::create();
    scene->addChild(layer);

    return scene;
}

void BossCapScene::InitBackGround()
{
	// Camera
	cameraOffsetXFromScene = visibleSize.width * 0.5f;
	cameraMaxOffsetX = visibleSize.width * 1.f + origin.x + cameraOffsetXFromScene;
	cameraMinOffsetX = origin.x + cameraOffsetXFromScene;
	// Background
	Sprite* background;
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);

	/* Boss */
	int tag = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_RICH);
	if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_remainedLoyal").c_str(), false))
	{
		tag += Unit::BOSS_KING - Unit::BOSS_QUEEN;
	}
	if (tag > Unit::BOSS_KING)
	{
		tag = Unit::BOSS_RICH;
		def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_RICH);
	}
	switch (tag)
	{
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			background = Sprite::create("Background/4_1_stage.png");
			break;
		}
		case Unit::BOSS_COMMANDER:
		{
			background = Sprite::create("Background/5_1__6_1_stage.png");
			break;
		}
		case Unit::BOSS_QUEEN:
		{
			background = Sprite::create("Background/5_1__6_1_stage.png");
			break;
		}
		case Unit::BOSS_WIZARD:
		{
			background = Sprite::create("Background/5_1__6_1_stage.png");
			break;
		}
		case Unit::BOSS_KING:
		{
			background = Sprite::create("Background/5_1__6_1_stage.png");
			break;
		}
		default:
		{
			background = Sprite::create("Background/1_1__2_1__3_1_stage.png");
			break;
		}
	}
	background->setAnchorPoint(Vec2(0, 0));
	background->setScale(	(visibleSize.width * 2.0f / background->getContentSize().width),
							(visibleSize.height / background->getContentSize().height));
	background->setPosition(origin);
	this->addChild(background, 0);

}

void BossCapScene::InitSound()
{
	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		Unit* boss = (Unit*)this->getChildByTag(BOSS);
		SimpleAudioEngine::getInstance()->playBackgroundMusic(("Sounds/mp3/Music/" + StringUtils::format("%d", boss->GetType() - Unit::BOSS_RICH + 1) + "_stage.mp3").c_str(), true);
	}
}

void BossCapScene::InitGUI()
{
	auto gui = Node::create();
	gui->setTag(GUI);
	this->addChild(gui, 1000);

	totalDuration = 90.f;
	Label* timer = Label::createWithTTF(StringUtils::format("Time Left: %ds", (int)totalDuration), "MiniGameResources/CuteFont.ttf", visibleSize.height * 0.04f);
	timer->setPosition(visibleSize.width * 0.5f + origin.x,
		visibleSize.height * 0.9f + origin.y);
	timer->setColor(Color3B::WHITE);
	timer->setAnchorPoint(Point(0.5f, 0.5f));

	auto countdown = DelayTime::create(totalDuration);
	countdown->setTag(TIMER);
	timer->runAction(countdown);

	timer->setTag(TIMER);
	gui->addChild(timer, 2);	

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	/* Joystick*/
	/* Movement */
	touchedMovement = NULL;
	touchedMovementJoystick = false;
	Size joystickButtonSize(visibleSize.height * 0.1f, visibleSize.height * 0.1f);
	Size movementJoystickBackgroundSize(joystickButtonSize * 2.f);
	joystickMovementMaxOffset = (movementJoystickBackgroundSize.width - joystickButtonSize.width) * 0.5f;
	// Joystick Background
	auto movementJoystickBackground = Sprite::create("bossCap/joystick_background.png");
	movementJoystickBackground->setScale(	movementJoystickBackgroundSize.width / movementJoystickBackground->getContentSize().width,
											movementJoystickBackgroundSize.height / movementJoystickBackground->getContentSize().height);
	movementJoystickBackground->setPosition(movementJoystickBackgroundSize.width * 1.f + origin.x,
											movementJoystickBackgroundSize.height * 1.f + origin.y);
	movementJoystickBackground->setTag(JOY_STICK_BACKGROUND_MOVEMENT);
	gui->addChild(movementJoystickBackground, 0);
	// Joystick
	auto movementJoystick = Sprite::create("bossCap/joystick.png");
	movementJoystick->setScale( joystickButtonSize.width / movementJoystick->getContentSize().width,
								joystickButtonSize.height / movementJoystick->getContentSize().height);
	movementJoystick->setPosition(movementJoystickBackground->getPosition());
	movementJoystick->setTag(JOY_STICK_MOVEMENT);
	gui->addChild(movementJoystick, 1);
	/***********/
	/* Attack */
	touchedAttack = NULL;
	touchedAttackJoystick = false;
	Size attackJoystickBackgroundSize(joystickButtonSize.width* 3.f, joystickButtonSize.height);
	joystickAttackMaxOffset = (attackJoystickBackgroundSize.width - joystickButtonSize.width) * 0.5f;
	// Joystick Background
	auto attackJoystickBackground = Sprite::create("bossCap/joystick_background_attack.png");
	attackJoystickBackground->setScale(	attackJoystickBackgroundSize.width / attackJoystickBackground->getContentSize().width,
										attackJoystickBackgroundSize.height / attackJoystickBackground->getContentSize().height);
	attackJoystickBackground->setPosition(	visibleSize.width - attackJoystickBackgroundSize.width * 1.f + origin.x,
											movementJoystickBackground->getPositionY());
	attackJoystickBackground->setTag(JOY_STICK_BACKGROUND_ATTACK);
	gui->addChild(attackJoystickBackground, 0);
	// Joystick
	auto attackJoystick = Sprite::create("bossCap/joystick_attack.png");
	attackJoystick->setScale(	joystickButtonSize.width / attackJoystick->getContentSize().width,
								joystickButtonSize.height / attackJoystick->getContentSize().height);
	attackJoystick->setPosition(attackJoystickBackground->getPosition());
	attackJoystick->setTag(JOY_STICK_ATTACK);
	gui->addChild(attackJoystick, 1);
	/***********/
	/***********/
#endif

	// Hp bar
	Size hpBarSize(visibleSize.width * 0.4f, visibleSize.height * 0.03f);
	// HP BAR: PLAYER
	Unit* player = (Unit*)this->getChildByTag(PLAYER);
	player->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png", false);
	HPBar* hpbar = player->GetHPBar();
	hpbar->Resize(hpBarSize);
	hpbar->setPosition(	visibleSize.width * 0.5f + origin.x,
						visibleSize.height + origin.y - hpBarSize.height);
	hpbar->setTag(HP_BAR);
	gui->addChild(hpbar, 0);

	// mp bar
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	int intPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::INT_PLUS_BUTTON)).c_str(), 0);
	playerMaxMP = 100;
	playerMPRecoverySpeed = intPoints;

	Size mpBarSize(hpBarSize * 0.8f);
	Sprite *mpBarSprite = Sprite::create("HPBar/skillbar.png");
	/*mpBarSprite->setScale(	mpBarSize.width / mpBarSprite->getContentSize().width,
							mpBarSize.height / mpBarSprite->getContentSize().height);*/
	ProgressTimer *mpBar = ProgressTimer::create(mpBarSprite);
	mpBar->setScale(mpBarSize.width / mpBar->getContentSize().width,
					mpBarSize.height / mpBar->getContentSize().height);
	mpBar->setAnchorPoint(Point(0.5f, 0.5f));
	mpBar->setPosition(	visibleSize.width * 0.5f + origin.x,
						hpbar->getPositionY() - hpBarSize.height * 0.5f - mpBarSize.height);
	mpBar->setPercentage(0.f);
	mpBar->setMidpoint(ccp(0, 0.5));
	mpBar->setBarChangeRate(ccp(1, 0));
	mpBar->setType(kCCProgressTimerTypeBar);

	mpBar->setTag(MP_BAR);
	gui->addChild(mpBar, 2);

	if (playerMPRecoverySpeed > 0)
	{
		float duration = playerMaxMP / playerMPRecoverySpeed;
		mpBar->runAction(ProgressTo::create(duration, 100));
	}

	Sprite *mpBarBackgroundSprite = Sprite::create("HPBar/hp_bar_background.png");
	mpBarBackgroundSprite->setScale(mpBarSize.width / mpBarBackgroundSprite->getContentSize().width,
									mpBarSize.height / mpBarBackgroundSprite->getContentSize().height);
	mpBarBackgroundSprite->setPosition(	visibleSize.width * 0.5f + origin.x,
										hpbar->getPositionY() - hpBarSize.height * 0.5f - mpBarSize.height);
	gui->addChild(mpBarBackgroundSprite, 1);
	
	Sprite *mpBarOutlineSprite = Sprite::create("HPBar/hp_bar_outline.png");
	float outlineSize = 5.f;
	mpBarOutlineSprite->setScale((mpBarSize.width + outlineSize) / mpBarOutlineSprite->getContentSize().width,
								 (mpBarSize.height + outlineSize) / mpBarOutlineSprite->getContentSize().height);
	mpBarOutlineSprite->setPosition(visibleSize.width * 0.5f + origin.x,
									hpbar->getPositionY() - hpBarSize.height * 0.5f - mpBarSize.height);
	gui->addChild(mpBarOutlineSprite, 0);

	// Current Equipped weapon
	float currentEquippedWeaponSprSize = visibleSize.height * 0.15f;
	auto currentEquippedWeaponSpr = Sprite::create("CharacterAnimations/stop_sd.png");
	currentEquippedWeaponSpr->setScale(	currentEquippedWeaponSprSize / currentEquippedWeaponSpr->getContentSize().width,
										currentEquippedWeaponSprSize / currentEquippedWeaponSpr->getContentSize().height);
	currentEquippedWeaponSpr->setPosition(	origin.x + currentEquippedWeaponSprSize,
											visibleSize.height + origin.y - currentEquippedWeaponSprSize);
	currentEquippedWeaponSpr->setTag(CURRENT_EQUIPPED_WEAPON);
	gui->addChild(currentEquippedWeaponSpr, 0);

	Vector<MenuItem*> MenuItems;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	// Change Weapon
	Size changeWeaponButtonSize(visibleSize.height * 0.1f, visibleSize.height * 0.1f);
	auto changeWeaponButton = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
													CC_CALLBACK_1(BossCapScene::ChangeWeapon, this));
	changeWeaponButton->setScale(	changeWeaponButtonSize.width / changeWeaponButton->getContentSize().width,
									changeWeaponButtonSize.height / changeWeaponButton->getContentSize().height);
	changeWeaponButton->setPosition(changeWeaponButtonSize.width ,
									-visibleSize.height * 0.3f);
	changeWeaponButton->setTag(CHANGE_WEAPON_BUTTON);
	MenuItems.pushBack(changeWeaponButton);

	// Special skill
	Size specialSkilluttonSize(visibleSize.height * 0.1f, visibleSize.height * 0.1f);
	auto specialSkillButton = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
													CC_CALLBACK_1(BossCapScene::SpecialSkill, this));
	specialSkillButton->setScale(	specialSkilluttonSize.width / specialSkillButton->getContentSize().width,
									specialSkilluttonSize.height / specialSkillButton->getContentSize().height);
	specialSkillButton->setPosition(visibleSize.width * 0.25f,
									-visibleSize.height * 0.15f);
	specialSkillButton->setTag(SPECIAL_SKILL);
	MenuItems.pushBack(specialSkillButton);
#endif

	// Pause
	Size pauseButtonSize(visibleSize.height * 0.1f, visibleSize.height * 0.1f);
	auto pause = MenuItemImage::create(	"PauseWindow/button.png", "PauseWindow/push_button.png",
										CC_CALLBACK_1(BossCapScene::Pause, this));
	pause->setScale(pauseButtonSize.width / pause->getContentSize().width,
					pauseButtonSize.height / pause->getContentSize().height);
	pause->setPosition(	visibleSize.width * 0.5f - pauseButtonSize.width * 0.55f,
						visibleSize.height * 0.5f - pauseButtonSize.height * 0.55f);
	pause->setTag(PAUSE_BUTTON);
	MenuItems.pushBack(pause);

	// gameplay menu
	auto gameplayMenu = Menu::createWithArray(MenuItems);
	gameplayMenu->setTag(GAMEPLAY_MENU);
	gui->addChild(gameplayMenu, 1);

	/* Pause Window */
	Vector<MenuItem*> PauseItems;
	//	Pause Window Background
	auto pauseWindowBackground = MenuItemImage::create("Background/HalfAlphaBlack.png", "Background/HalfAlphaBlack.png");
	pauseWindowBackground->setScale(visibleSize.width / pauseWindowBackground->getContentSize().width,
		visibleSize.height / pauseWindowBackground->getContentSize().height);
	PauseItems.pushBack(pauseWindowBackground);
	// Resume Button
	Size PauseWindowButtonSize(visibleSize.width * 0.2f, visibleSize.height * 0.2f);
	auto resume = MenuItemImage::create("victory/continue.png", "victory/push_continue.png",
		CC_CALLBACK_1(BossCapScene::Resume, this));
	resume->setScale(PauseWindowButtonSize.width / resume->getContentSize().width,
		PauseWindowButtonSize.height / resume->getContentSize().height);
	resume->setPosition(-visibleSize.width * 0.3f, 0);
	resume->setTag(RESUME_BUTTON);
	PauseItems.pushBack(resume);
	// Restart Button
	auto restart = MenuItemImage::create("PauseWindow/retry.png", "PauseWindow/push_retry.png",
		CC_CALLBACK_1(BossCapScene::Restart, this));
	restart->setScale(PauseWindowButtonSize.width / restart->getContentSize().width,
		PauseWindowButtonSize.height / restart->getContentSize().height);
	restart->setTag(RESTART_BUTTON);
	PauseItems.pushBack(restart);
	// Quit button
	auto quit = MenuItemImage::create("PauseWindow/mainmenu.png", "PauseWindow/push_menu.png",
		CC_CALLBACK_1(BossCapScene::Quit, this));
	quit->setScale(PauseWindowButtonSize.width / quit->getContentSize().width,
		PauseWindowButtonSize.height / quit->getContentSize().height);
	quit->setPosition(visibleSize.width * 0.3f, 0);
	quit->setTag(QUIT_BUTTON);
	PauseItems.pushBack(quit);

	auto pauseWindow = Menu::createWithArray(PauseItems);
	pauseWindow->setTag(PAUSE_WINDOW);
	gui->addChild(pauseWindow, 2);
	// Hide Pause Window
	pauseWindow->setVisible(false);
	/****************/

	Size foregroundSize(visibleSize.width * 0.5f, visibleSize.height * 0.5f);

	/* Won Window */
	Vector<MenuItem*> WonItems;
	//	Won Window Background
	auto wonWindowBackground = MenuItemImage::create("Background/HalfAlphaBlack.png", "Background/HalfAlphaBlack.png");
	wonWindowBackground->setScale(	visibleSize.width / wonWindowBackground->getContentSize().width,
									visibleSize.height / wonWindowBackground->getContentSize().height);
	wonWindowBackground->setOpacity(0);
	wonWindowBackground->setTag(BACKGROUND);
	WonItems.pushBack(wonWindowBackground);
	//	Won Window Foreground
	auto wonWindowForeground = MenuItemImage::create("victory/victory.png", "victory/victory.png");
	wonWindowForeground->setScale(	foregroundSize.width / wonWindowForeground->getContentSize().width,
									foregroundSize.height / wonWindowForeground->getContentSize().height);
	wonWindowForeground->setOpacity(0);
	wonWindowForeground->setPosition(0, visibleSize.height * 0.1f);
	wonWindowForeground->setTag(FOREGROUND);
	WonItems.pushBack(wonWindowForeground);
	// Next Button
	auto next = MenuItemImage::create(	"victory/continue.png", "victory/push_continue.png",
										CC_CALLBACK_1(BossCapScene::WinNext, this));
	next->setScale(0);
	next->setPosition(0, -visibleSize.height * 0.25f);
	next->setTag(NEXT_BUTTON);
	WonItems.pushBack(next);

	auto wonWindow = Menu::createWithArray(WonItems);
	wonWindow->setTag(WON_WINDOW);
	gui->addChild(wonWindow, 2);
	// Hide Won Window
	wonWindow->setVisible(false);
	/****************/

	/* Lost Window */
	Vector<MenuItem*> LostItems;
	//	Lost Window Background
	auto lostWindowBackground = MenuItemImage::create("Background/HalfAlphaBlack.png", "Background/HalfAlphaBlack.png");
	lostWindowBackground->setScale(	visibleSize.width / lostWindowBackground->getContentSize().width,
									visibleSize.height / lostWindowBackground->getContentSize().height);
	lostWindowBackground->setOpacity(0);
	lostWindowBackground->setTag(BACKGROUND);
	LostItems.pushBack(lostWindowBackground);
	//	Lost Window Foreground
	auto lostWindowForeground = MenuItemImage::create("defeat/defeat.png", "defeat/defeat.png");
	lostWindowForeground->setScale(	foregroundSize.width / lostWindowForeground->getContentSize().width,
									foregroundSize.height / lostWindowForeground->getContentSize().height);
	lostWindowForeground->setOpacity(0);
	lostWindowForeground->setPosition(0, visibleSize.height * 0.1f);
	lostWindowForeground->setTag(FOREGROUND);
	LostItems.pushBack(lostWindowForeground);
	// Restart Button
	restart = MenuItemImage::create("defeat/try.png", "defeat/push_try.png",
									CC_CALLBACK_1(BossCapScene::Restart, this));
	restart->setScale(0);
	restart->setPosition(-visibleSize.width * 0.2f, -visibleSize.height * 0.25f);
	restart->setTag(RESTART_BUTTON);
	LostItems.pushBack(restart);
	// Quit button
	quit = MenuItemImage::create(	"PauseWindow/mainmenu.png", "PauseWindow/push_menu.png",
									CC_CALLBACK_1(BossCapScene::Quit, this));
	quit->setScale(0);
	quit->setPosition(visibleSize.width * 0.2f, -visibleSize.height * 0.25f);
	quit->setTag(QUIT_BUTTON);
	LostItems.pushBack(quit);

	auto lostWindow = Menu::createWithArray(LostItems);
	lostWindow->setTag(LOST_WINDOW);
	gui->addChild(lostWindow, 2);
	// Hide Lost Window
	lostWindow->setVisible(false);
	/****************/
}

void BossCapScene::InitGameObjects()
{
	playerSize.setSize(visibleSize.height * 0.2f, visibleSize.height * 0.25f);
	Size unitSize(	playerSize.width * 0.65f,
					playerSize.height * 0.65f);
	Size bossSize(	playerSize.width * 0.9f,
					playerSize.height * 0.9f);
	spawnPositionLeft.set(	origin.x + visibleSize.height * 0.25f,
							origin.y);
	spawnPositionRight.set(	visibleSize.width * 2.0f + origin.x - visibleSize.height * 0.25f,
							origin.y);
	maxOffsetY = visibleSize.height * 0.6f - playerSize.height;
	playerOffset.set(0.0f, visibleSize.height * 0.3f);

	/* Create new unit manager */
	unitManager = new UnitManager;
	/* Create new collision manager */
	collisionManager = new CollisionManager;

	/* PLayer */
	auto player = new Unit();
	player->m_totalDistanceToMove = (spawnPositionRight - spawnPositionLeft).length();
	player->SetType(Unit::HERO_TWO_HANDED_SWORD);
	player->setTag(PLAYER);
	player->SetSprite(Sprite::create("CharacterAnimations/stop_sd.png"));
	Sprite* playerSpr = player->GetSprite();
	playerSpr->setAnchorPoint(Vec2(0.5f, 0));
	playerSpr->setScale(playerSize.width / playerSpr->getContentSize().width,
						playerSize.height / playerSpr->getContentSize().height);

	// Set Position to the Left
	player->setPosition(spawnPositionLeft + playerOffset);

	// Add player to unit manager
	player->SetActive(true);
	this->addChild(player, 1);
	unitManager->SetPlayer(player);
	player->SelfSetInfoAccordingToTypeAndLocations(spawnPositionRight + playerOffset, player->getPosition());
	player->SetActive(true);
	/**********/

	/* Player Unit's Spawner */
	Spawner* playerSpawner = new Spawner();
	playerSpawner->setTag(SPAWNER_PLAYER);
	playerSpawner->SetSpawnLocation(spawnPositionLeft, maxOffsetY);
	this->addChild(playerSpawner, 0);
	/*************************/

	// Get Current Data
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	/* Player Units */
	int amountOfMercenaries[MercenaryShopScene::NUM_OF_MERCENARIES - MercenaryShopScene::SWORDSMAN];
	int totalAmountOfMercenaries = 0;
	for (int i = MercenaryShopScene::SWORDSMAN; i < MercenaryShopScene::NUM_OF_MERCENARIES; i++)
	{
		amountOfMercenaries[i - MercenaryShopScene::SWORDSMAN] = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_MercaneryBought" + StringUtils::format("%d", i)).c_str(), 0);
		totalAmountOfMercenaries += amountOfMercenaries[i - MercenaryShopScene::SWORDSMAN];
	}
	for (int i = 0; i < totalAmountOfMercenaries; i++)
	{
		Unit* unit = new Unit();
		unit->m_totalDistanceToMove = (spawnPositionRight - spawnPositionLeft).length();
		unit->SetSprite(Sprite::create());
		unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
		unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
		unit->SetActive(false);
		unit->setTag(PLAYER_UNITS + i);
		this->addChild(unit, 1);

		playerSpawner->AddUnit(unit);
		unitManager->AddPlayerUnit(unit);
	}
	for (int i = MercenaryShopScene::SWORDSMAN; i < MercenaryShopScene::NUM_OF_MERCENARIES; i++)
	{
		for (int j = 0; j < amountOfMercenaries[i - MercenaryShopScene::SWORDSMAN]; j++)
		{
			switch (i)
			{
				case MercenaryShopScene::SWORDSMAN:
				{
					playerSpawner->Spawn(&unitSize, &spawnPositionRight, Unit::SWORDSMAN);
					break;
				}
				case MercenaryShopScene::ARCHER:
				{
					playerSpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ARCHER);
					break;
				}
				case MercenaryShopScene::SPEARMAN:
				{
					playerSpawner->Spawn(&unitSize, &spawnPositionRight, Unit::SPEARMAN);
					break;
				}
				case MercenaryShopScene::HORSEMAN:
				{
					playerSpawner->Spawn(&unitSize, &spawnPositionRight, Unit::HORSEMAN);
					break;
				}
				case MercenaryShopScene::SHIELDMAN:
				{
					playerSpawner->Spawn(&unitSize, &spawnPositionRight, Unit::SHIELDMAN);
					break;
				}
				case MercenaryShopScene::WIZARD:
				{
					playerSpawner->Spawn(&unitSize, &spawnPositionRight, Unit::WIZARD);
					break;
				}
			}
		}
	}
	/****************/

	/* Boss */
	auto boss = new Unit();
	boss->m_totalDistanceToMove = (spawnPositionRight - spawnPositionLeft).length();
	int tag = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_RICH);
	if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_remainedLoyal").c_str(), false))
	{
		tag += Unit::BOSS_KING - Unit::BOSS_QUEEN;
	}
	if (tag > Unit::BOSS_KING)
	{
		tag = Unit::BOSS_RICH;
		def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_RICH);
	}
	boss->SetType(tag);
	boss->setTag(BOSS);
	boss->SelfSetInfoAccordingToTypeAndLocations(spawnPositionLeft + playerOffset, spawnPositionRight + playerOffset);
	boss->SetSprite(Sprite::create("CharacterAnimations/stop_sd.png"));
	boss->SetStopTexture();
	// Set boss's Sprite
	Sprite* bossSpr = boss->GetSprite();
	bossSpr->setAnchorPoint(Vec2(0.5f, 0));
	bossSpr->setScale(	bossSize.width / bossSpr->getContentSize().width,
						bossSize.height / bossSpr->getContentSize().height);
	// HP Bar
	boss->InitHPBar("HPBar/hp_bar_background.png", "HPBar/enemy_boss_hp_bar.png", "HPBar/hp_bar_outline.png");
	boss->ResizeHPBar();
	// Set Position to the Right
	boss->setPosition(spawnPositionRight + playerOffset);
	this->addChild(boss, 1);
	unitManager->SetEnemyBoss(boss);
	boss->setLocalZOrder((int)(maxOffsetY)-((int)boss->getPositionY() - spawnPositionLeft.y));
	boss->MoveToTarget();
	boss->SetActive(true);
	/********/

	/* Enemy spawner */
	Spawner* enemySpawner = new Spawner();
	enemySpawner->setTag(SPAWNER_ENEMY);
	enemySpawner->SetSpawnLocation(spawnPositionRight, maxOffsetY);
	this->addChild(enemySpawner, 0);
	/*************************/

	/* Enemy Units */
	switch (tag)
	{
		case Unit::BOSS_RICH:
		{
			int numOfEnemies = 5 + 4 + 3;
			for (int i = 0; i < numOfEnemies; i++)
			{
				Unit* unit = new Unit();
				unit->m_totalDistanceToMove = (spawnPositionLeft - spawnPositionRight).length();
				unit->SetSprite(Sprite::create());
				unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
				unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
				unit->SetActive(false);
				unit->setTag(ENEMY_UNITS + i);
				this->addChild(unit, 1);

				enemySpawner->AddUnit(unit);
				unitManager->AddEnemyUnit(unit);
			}
			for (int i = 0; i < 5; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_FARMER);
			}
			for (int i = 0; i < 4; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_SLAVE);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_PEOPLE);
			}
			break;
		}
		case Unit::BOSS_FALLEN_ALCHEMIST:
		{
			int numOfEnemies = 3 + 2 + 3 + 2;
			for (int i = 0; i < numOfEnemies; i++)
			{
				Unit* unit = new Unit();
				unit->m_totalDistanceToMove = (spawnPositionLeft - spawnPositionRight).length();
				unit->SetSprite(Sprite::create());
				unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
				unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
				unit->SetActive(false);
				unit->setTag(ENEMY_UNITS + i);
				this->addChild(unit, 1);

				enemySpawner->AddUnit(unit);
				unitManager->AddEnemyUnit(unit);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_SLAVE);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_BEGGAR);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_BARON);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_ALCHEMIST);
			}
			break;
		}
		case Unit::BOSS_ESCAPED_PRISONER:
		{
			int numOfEnemies = 2 + 2 + 2 + 3 + 2;
			for (int i = 0; i < numOfEnemies; i++)
			{
				Unit* unit = new Unit();
				unit->m_totalDistanceToMove = (spawnPositionLeft - spawnPositionRight).length();
				unit->SetSprite(Sprite::create());
				unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
				unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
				unit->SetActive(false);
				unit->setTag(ENEMY_UNITS + i);
				this->addChild(unit, 1);

				enemySpawner->AddUnit(unit);
				unitManager->AddEnemyUnit(unit);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_SHIELDMAN);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_FIRE_WIZARD);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_HEAVY_ARMOR_KNIGHT);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_KNIGHT);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_SPEARMAN);
			}
			break;
		}
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			int numOfEnemies = 2 + 3 + 3 + 2;
			for (int i = 0; i < numOfEnemies; i++)
			{
				Unit* unit = new Unit();
				unit->m_totalDistanceToMove = (spawnPositionLeft - spawnPositionRight).length();
				unit->SetSprite(Sprite::create());
				unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
				unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
				unit->SetActive(false);
				unit->setTag(ENEMY_UNITS + i);
				this->addChild(unit, 1);

				enemySpawner->AddUnit(unit);
				unitManager->AddEnemyUnit(unit);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_KNIGHT);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_HEAVY_ARMOR_KNIGHT);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_PLATE_KNIGHT);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_LIGHTNING_WIZARD);
			}
			break;
		}
		case Unit::BOSS_COMMANDER:
		{
			int numOfEnemies = 2 + 3 + 2 + 2 + 1 + 1;
			for (int i = 0; i < numOfEnemies; i++)
			{
				Unit* unit = new Unit();
				unit->m_totalDistanceToMove = (spawnPositionLeft - spawnPositionRight).length();
				unit->SetSprite(Sprite::create());
				unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
				unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
				unit->SetActive(false);
				unit->setTag(ENEMY_UNITS + i);
				this->addChild(unit, 1);

				enemySpawner->AddUnit(unit);
				unitManager->AddEnemyUnit(unit);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_KNIGHT);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_HEAVY_ARMOR_SPEARMAN);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_SHIELDMAN);
			}
			for (int i = 0; i < 1; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_ICE_WIZARD);
			}
			for (int i = 0; i < 1; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD);
			}
			break;
		}
		case Unit::BOSS_QUEEN:
		{
			int numOfEnemies = 1 + 3 + 2 + 2 + 3 + 3;
			for (int i = 0; i < numOfEnemies; i++)
			{
				Unit* unit = new Unit();
				unit->m_totalDistanceToMove = (spawnPositionLeft - spawnPositionRight).length();
				unit->SetSprite(Sprite::create());
				unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
				unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
				unit->SetActive(false);
				unit->setTag(ENEMY_UNITS + i);
				this->addChild(unit, 1);

				enemySpawner->AddUnit(unit);
				unitManager->AddEnemyUnit(unit);
			}
			for (int i = 0; i < 1; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_KNIGHT);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_HEAVY_ARMOR_SPEARMAN);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_SHIELDMAN);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_ICE_WIZARD);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD);
			}
			break;
		}
		case Unit::BOSS_WIZARD:
		{
			int numOfEnemies = 2 + 2 + 2 + 2 + 1 + 2;
			for (int i = 0; i < numOfEnemies; i++)
			{
				Unit* unit = new Unit();
				unit->m_totalDistanceToMove = (spawnPositionLeft - spawnPositionRight).length();
				unit->SetSprite(Sprite::create());
				unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
				unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
				unit->SetActive(false);
				unit->setTag(ENEMY_UNITS + i);
				this->addChild(unit, 1);

				enemySpawner->AddUnit(unit);
				unitManager->AddEnemyUnit(unit);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_HEAVY_ARMOR_KNIGHT);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_ICE_WIZARD);
			}
			for (int i = 0; i < 1; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_SHIELDMAN);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_HEAVY_ARMOR_SPEARMAN);
			}
			break;
		}
		case Unit::BOSS_KING:
		{
			int numOfEnemies = 2 + 2 + 1 + 2 + 1 + 3;
			for (int i = 0; i < numOfEnemies; i++)
			{
				Unit* unit = new Unit();
				unit->m_totalDistanceToMove = (spawnPositionLeft - spawnPositionRight).length();
				unit->SetSprite(Sprite::create());
				unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
				unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
				unit->SetActive(false);
				unit->setTag(ENEMY_UNITS + i);
				this->addChild(unit, 1);

				enemySpawner->AddUnit(unit);
				unitManager->AddEnemyUnit(unit);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_HEAVY_ARMOR_KNIGHT);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT);
			}
			for (int i = 0; i < 1; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD);
			}
			for (int i = 0; i < 2; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_ICE_WIZARD);
			}
			for (int i = 0; i < 1; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_STRENGTHEN_SHIELDMAN);
			}
			for (int i = 0; i < 3; i++)
			{
				enemySpawner->Spawn(&unitSize, &spawnPositionRight, Unit::ENEMY_HEAVY_ARMOR_SPEARMAN);
			}
			break;
		}
	}
	/****************/
}

bool BossCapScene::init()
{
	Director::getInstance()->getTextureCache()->removeUnusedTextures();

    if ( !Layer::init() )
    {
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin      = Director::getInstance()->getVisibleOrigin();
	
	InitBackGround();
	InitGameObjects();
	InitGUI();
	InitSound();

	currentState = PLAYING;
	

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//터치 리스너 등록
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(BossCapScene::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(BossCapScene::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(BossCapScene::onTouchesEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(BossCapScene::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(BossCapScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);

	player_movement_direction = NO_DIRECTION;
	player_attack_direction = NO_DIRECTION;

	for (int i = 0; i < NUM_OF_KEYS; i++)
	{
		keyONHold[i] = false;
	}
#endif
	//Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

	//반복시간 안정한 스케쥴은 지정한 fps 속도로 반복 
	this->schedule(schedule_selector(BossCapScene::update));
	//반복시간 정한 스케쥴은 그 시간마다 호출
	
	//딜레이 타임 후 한번만	불러지게 하기
	//this->scheduleOnce(schedule_selector(HelloWorld::Tick), 10.0f);

    return true;
}


#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
void BossCapScene::onTouchesBegan(const std::vector<Touch*>& touches, Event* unused_event)
{
	//마우스가 누른 위치 받기
	if (!touchedMovementJoystick && !touchedAttackJoystick)
	{
		Sprite* movementJoystick = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_MOVEMENT);
		Sprite* attackJoystick = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_ATTACK);
		for (int i = 0; i < touches.size(); i++)
		{
			Point location;
			location = touches.at(i)->getLocation();
			if (movementJoystick->getBoundingBox().containsPoint(location))
			{
				touchedMovement = touches.at(i);
				touchedMovementJoystick = true;
			}
			else if (attackJoystick->getBoundingBox().containsPoint(location))
			{
				touchedAttack = touches.at(i);
				touchedAttackJoystick = true;
				Attack();
			}
		}
	}
}
void BossCapScene::onTouchesMoved(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (touchedMovementJoystick)
	{
		MoveMovementJoystick(touchedMovement->getLocation());
	}
	if (touchedAttackJoystick)
	{
		MoveAttackJoystick(touchedAttack->getLocation());
	}
}

void BossCapScene::onTouchesEnded(const std::vector<Touch*>& touches, Event *unused_event)
{
	if (touchedMovementJoystick)
	{
		ResetMovementJoystick();
	}
	if (touchedAttackJoystick)
	{
		ResetAttackJoystick();
	}
}

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void BossCapScene::CheckMovementKeysOnHold()
{
	if (keyONHold[(int)EventKeyboard::KeyCode::KEY_W])
	{
		player_movement_direction = UP;
	}
	else if (keyONHold[(int)EventKeyboard::KeyCode::KEY_S])
	{
		player_movement_direction = DOWN;
	}
	else if (keyONHold[(int)EventKeyboard::KeyCode::KEY_A])
	{
		player_movement_direction = LEFT;
	}
	else if (keyONHold[(int)EventKeyboard::KeyCode::KEY_D])
	{
		player_movement_direction = RIGHT;
	}
	else
	{
		player_movement_direction = NO_DIRECTION;
	}
}
void BossCapScene::CheckAttackKeysOnHold()
{
	if (keyONHold[(int)EventKeyboard::KeyCode::KEY_J])
	{
		player_attack_direction = LEFT;
		auto playerSpr = ((Unit*)this->getChildByTag(PLAYER))->GetSprite();
		playerSpr->setFlippedX(true);
	}
	else if (keyONHold[(int)EventKeyboard::KeyCode::KEY_K])
	{
		player_attack_direction = RIGHT;
		auto playerSpr = ((Unit*)this->getChildByTag(PLAYER))->GetSprite();
		playerSpr->setFlippedX(false);
	}
	else
	{
		player_attack_direction = NO_DIRECTION;
		StopAttack();
	}
}

void BossCapScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (currentState == PLAYING)
	{
		if (player_attack_direction == NO_DIRECTION)
		{
			switch (keyCode)
			{
				case EventKeyboard::KeyCode::KEY_W:
				{
					keyONHold[(int)EventKeyboard::KeyCode::KEY_W] = true;
					player_movement_direction = UP;
					break;
				}
				case EventKeyboard::KeyCode::KEY_S:
				{
					keyONHold[(int)EventKeyboard::KeyCode::KEY_S] = true;
					player_movement_direction = DOWN;
					break;
				}
				case EventKeyboard::KeyCode::KEY_A:
				{
					keyONHold[(int)EventKeyboard::KeyCode::KEY_A] = true;
					player_movement_direction = LEFT;
					break;
				}
				case EventKeyboard::KeyCode::KEY_D:
				{
					keyONHold[(int)EventKeyboard::KeyCode::KEY_D] = true;
					player_movement_direction = RIGHT;
					break;
				}
				case EventKeyboard::KeyCode::KEY_SPACE:
				{
					ChangeWeapon();
					break;
				}
				case EventKeyboard::KeyCode::KEY_F:
				{
					SpecialSkill();
					break;
				}
			}
		}
		if (player_movement_direction == NO_DIRECTION)
		{
			switch (keyCode)
			{
				case EventKeyboard::KeyCode::KEY_J:
				{
					keyONHold[(int)EventKeyboard::KeyCode::KEY_J] = true;
					if (player_attack_direction == NO_DIRECTION)
					{
						Attack();
					}
					player_attack_direction = LEFT;
					auto playerSpr = ((Unit*)this->getChildByTag(PLAYER))->GetSprite();
					playerSpr->setFlippedX(true);
					break;
				}
				case EventKeyboard::KeyCode::KEY_K:
				{
					keyONHold[(int)EventKeyboard::KeyCode::KEY_K] = true;
					if (player_attack_direction == NO_DIRECTION)
					{
						Attack();
					}
					player_attack_direction = RIGHT;
					auto playerSpr = ((Unit*)this->getChildByTag(PLAYER))->GetSprite();
					playerSpr->setFlippedX(false);
					break;
				}
			}
		}
	}
}

void BossCapScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_W:
		{
			keyONHold[(int)EventKeyboard::KeyCode::KEY_W] = false;
			if (player_movement_direction == UP)
			{
				CheckMovementKeysOnHold();
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_S:
		{
			keyONHold[(int)EventKeyboard::KeyCode::KEY_S] = false;
			if (player_movement_direction == DOWN)
			{
				CheckMovementKeysOnHold();
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_A:
		{
			keyONHold[(int)EventKeyboard::KeyCode::KEY_A] = false;
			if (player_movement_direction == LEFT)
			{
				CheckMovementKeysOnHold();
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_D:
		{
			keyONHold[(int)EventKeyboard::KeyCode::KEY_D] = false;
			if (player_movement_direction == RIGHT)
			{
				CheckMovementKeysOnHold();
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_J:
		{
			keyONHold[(int)EventKeyboard::KeyCode::KEY_J] = false;
			if (player_attack_direction == LEFT)
			{
				CheckAttackKeysOnHold();
			}
			break;
		}
		case EventKeyboard::KeyCode::KEY_K:
		{
			keyONHold[(int)EventKeyboard::KeyCode::KEY_K] = false;
			if (player_attack_direction == RIGHT)
			{
				CheckAttackKeysOnHold();
			}
			break;
		}
	}
}

#endif

void BossCapScene::StopAttack()
{
	// Stop animation
	Unit* player = (Unit*)this->getChildByTag(PLAYER);
	player->StopAction(Unit::ATTACKING);
	player->StopAction(Unit::WAITING_TO_NEXT_ATTACK);
	player->SetStopTexture();
}

void BossCapScene::update(float ft)
{
	if (currentState == PLAYING)
	{
		/* Timer */
		auto timer = (Label*)this->getChildByTag(GUI)->getChildByTag(TIMER);
		auto countdown = (DelayTime*)timer->getActionByTag(TIMER);
		if (countdown)
		{
			timer->setString(StringUtils::format("Time Left: %ds", (int)(totalDuration - countdown->getElapsed())));
		}
		else
		{
			GameEnd(false);
		}

		// Player
		UpdatePlayer(ft);
		/* Units & Collision */
		// Update unit manager
		if (unitManager)
		{
			unitManager->Update(ft, true);
			if (collisionManager)
				collisionManager->CheckCollision(unitManager, NULL, true);
		}
		// Camera
		ConstraintCamera();
		// Check game condition
		Unit* player = (Unit*) this->getChildByTag(PLAYER);
		Unit* boss = (Unit*) this->getChildByTag(BOSS);
		if (!player->IfActive())
		{
			GameEnd(false);
		}
		else if (!boss->IfActive())
		{
			GameEnd(true);
		}
	}
	auto gui = (Node*)this->getChildByTag(GUI);
	gui->setPositionX((Camera::getDefaultCamera()->getPositionX() - visibleSize.width * 0.5f + origin.x));
}

void BossCapScene::UpdatePlayer(float deltaTime)
{
	Unit* player = (Unit*) this->getChildByTag(PLAYER);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (touchedMovementJoystick)
	{
		// Move Player
		Sprite* joystick = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_MOVEMENT);
		Sprite* joystick_background = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_BACKGROUND_MOVEMENT);
		Vec2 moveBy = (joystick->getPosition() - joystick_background->getPosition()) * deltaTime * player->GetMovementSpeed();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if (player_movement_direction != NO_DIRECTION)
	{
		Vec2 moveBy(0.0f, 0.0f);
		switch (player_movement_direction)
		{
			case LEFT:
			{
				moveBy.set((-visibleSize.height * 0.075f) * deltaTime * player->GetMovementSpeed(), 0.0f);
				break;
			}
			case RIGHT:
			{
				moveBy.set((visibleSize.height * 0.075f) * deltaTime * player->GetMovementSpeed(), 0.0f);
				break;
			}
			case UP:
			{
				moveBy.set(0.0f, (visibleSize.height * 0.075f) * deltaTime * player->GetMovementSpeed());
				break;
			}
			case DOWN:
			{
				moveBy.set(0.0f, (-visibleSize.height * 0.075f) * deltaTime * player->GetMovementSpeed());
				break;
			}
		}
#endif
		
		// Check moveByif exceed boundary
		Vec2 newPosition = player->getPosition() + moveBy;
		// Check X: Left
		if (newPosition.x < spawnPositionLeft.x)
		{
			newPosition.x = spawnPositionLeft.x;
		}
		// Check X: Right
		else if (newPosition.x > spawnPositionRight.x)
		{
			newPosition.x = spawnPositionRight.x;
		}
		// Check Y: Bottom
		if (newPosition.y < origin.y)
		{
			newPosition.y = origin.y;
		}
		// Check Y: Top
		else if (newPosition.y > maxOffsetY + origin.y)
		{
			newPosition.y = maxOffsetY + origin.y;
		}
		// Walk animation
		if (prevPlayerAnimateSpeed != moveBy.length())
		{
			prevPlayerAnimateSpeed = moveBy.length();
			// Run animation if speed is not zero
			if (prevPlayerAnimateSpeed != 0.0f)
				WalkAnimate(prevPlayerAnimateSpeed);
		}
		// Move player after checking
		player->setPosition(newPosition);

		// Check direction and update camera
		if (moveBy.x < 0.0f)
		{
			player->GetSprite()->setFlippedX(true);
			UpdateCamera(false);
		}
		else if (moveBy.x > 0.0f)
		{
			player->GetSprite()->setFlippedX(false);
			UpdateCamera(true);
		}
	}
	else
	{
		if (player->GetSprite()->getActionByTag(Unit::MOVING))
		{
			player->GetSprite()->stopActionByTag(Unit::MOVING);
			player->SetStopTexture();
		}
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (touchedAttackJoystick)
	{
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if(player_attack_direction != NO_DIRECTION)
	{
#endif
		if (player->GetState() == Unit::ATTACKING)
		{
			player->CheckActivateAttackEffect();
			if (!player->GetSprite()->getActionByTag(Unit::ATTACKING))
			{
				if (player->GetType() != Unit::HERO_BOW)
					PlayerDamageEnemies();
				WaitingToAttack();
			}
		}
	}
	player->CheckAttackEffect();
	player->setLocalZOrder((int)(maxOffsetY)-((int)player->getPositionY() - spawnPositionLeft.y));
}

void BossCapScene::WalkAnimate(float speed)
{
	Unit* player = (Unit*)this->getChildByTag(PLAYER);
	Sprite* PlayerSpr = player->GetSprite();
	Speed* animateSpeed = (Speed*)PlayerSpr->getActionByTag(Unit::MOVING);
	if (animateSpeed)
	{
		Animate* animate = (Animate*)animateSpeed->getInnerAction();
		Animation* animation = animate->getAnimation();
		if (animate->getCurrentFrameIndex() != animation->getFrames().size() - 1)
		{
			animateSpeed->setSpeed(speed);
		}
		else
		{
			PlayerSpr->stopActionByTag(Unit::MOVING);
			auto animateNew = Speed::create(AnimationManager::GetWalkingAnimation(player->GetType(), 3.f), speed);
			animateNew->setTag(Unit::MOVING);
			PlayerSpr->runAction(animateNew);
		}
	}
	else
	{
		auto animateNew = Speed::create(AnimationManager::GetWalkingAnimation(player->GetType(), 3.f), speed);
		animateNew->setTag(Unit::MOVING);
		PlayerSpr->runAction(animateNew);
	}
}

void BossCapScene::Attack()
{
	Unit* player = (Unit*)this->getChildByTag(PLAYER);
	player->SetState(Unit::ATTACKING);
	AttackAnimate(0.5f);

	auto attacktime = DelayTime::create(0.5f);
	attacktime->setTag(Unit::ATTACKING);
	player->runAction(attacktime);

	if (player->GetType() == Unit::HERO_BOW)
	{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
		//SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/archer_man_attack_sound.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/archer_man_attack_sound.wav");
#endif
	}
}

void BossCapScene::PlayerDamageEnemies()
{
	Unit* player = (Unit*)this->getChildByTag(PLAYER);
	player->DamageTargets();
}

void BossCapScene::WaitingToAttack()
{
	Unit* player = (Unit*)this->getChildByTag(PLAYER);
	player->SetState(Unit::WAITING_TO_NEXT_ATTACK);
	auto action = Sequence::create(DelayTime::create(player->GetAttackSpeed()), CallFuncN::create(CC_CALLBACK_0(BossCapScene::Attack, this)), NULL);
	action->setTag(Unit::WAITING_TO_NEXT_ATTACK);
	player->runAction(action);
}

void BossCapScene::AttackAnimate(float speed)
{
	Unit* player = (Unit*)this->getChildByTag(PLAYER);
	Sprite* PlayerSpr = player->GetSprite();
	auto animateNew = AnimationManager::GetAttackingAnimation(player->GetType(), speed);
	animateNew->setTag(Unit::ATTACKING);
	PlayerSpr->runAction(animateNew);
}


void BossCapScene::UseMp(int usageAmount)
{
	if (playerMPRecoverySpeed > 0)
	{
		auto mpBar = (ProgressTimer*)this->getChildByTag(GUI)->getChildByTag(MP_BAR);
		float newpercentage = (playerMaxMP * (mpBar->getPercentage() / 100.f) - usageAmount) / playerMaxMP * 100.f;
		mpBar->stopAllActions();
		mpBar->setPercentage(newpercentage);
		float duration = playerMaxMP / playerMPRecoverySpeed;
		duration *= ((100 - mpBar->getPercentage()) / 100.f);
		mpBar->runAction(ProgressTo::create(duration, 100));
	}
}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

void BossCapScene::MoveMovementJoystick(Point location)
{
	Sprite* joystick = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_MOVEMENT);
	Sprite* joystick_background = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_BACKGROUND_MOVEMENT);

	// Move joystick
	joystick->setPosition(location);
	// Contraint joystick
	if ((joystick->getPosition() - joystick_background->getPosition()).length() > joystickMovementMaxOffset)
	{
		Vec2 newPosition = (joystick->getPosition() - joystick_background->getPosition()).getNormalized() * joystickMovementMaxOffset + joystick_background->getPosition();
		joystick->setPosition(newPosition);
	}
}

void BossCapScene::ResetMovementJoystick()
{
	Sprite* joystick = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_MOVEMENT);
	Sprite* joystick_background = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_BACKGROUND_MOVEMENT);
	joystick->setPosition(joystick_background->getPosition());

	touchedMovementJoystick = false;
	touchedMovement = NULL;
}
void BossCapScene::MoveAttackJoystick(Point location)
{
	Sprite* joystick = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_ATTACK);
	Sprite* joystick_background = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_BACKGROUND_ATTACK);

	// Move joystick
	joystick->setPositionX(location.x);
	// Contraint joystick
	if ((joystick->getPosition() - joystick_background->getPosition()).length() > joystickAttackMaxOffset)
	{
		Vec2 newPosition = (joystick->getPosition() - joystick_background->getPosition()).getNormalized() * joystickAttackMaxOffset + joystick_background->getPosition();
		joystick->setPosition(newPosition);
	}
	// Face Right
	if (joystick->getPositionX() > joystick_background->getPositionX())
	{
		auto playerSpr = ((Unit*)this->getChildByTag(PLAYER))->GetSprite();
		playerSpr->setFlippedX(false);
	}
	// Face Left
	else if (joystick->getPositionX() < joystick_background->getPositionX())
	{
		auto playerSpr = ((Unit*)this->getChildByTag(PLAYER))->GetSprite();
		playerSpr->setFlippedX(true);
	}
}
void BossCapScene::ResetAttackJoystick()
{
	Sprite* joystick = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_ATTACK);
	Sprite* joystick_background = (Sprite*) this->getChildByTag(GUI)->getChildByTag(JOY_STICK_BACKGROUND_ATTACK);
	joystick->setPosition(joystick_background->getPosition());

	touchedAttackJoystick = false;
	touchedAttack = NULL;

	StopAttack();
}

void BossCapScene::SpecialSkill(Ref* pSender)
{
	SpecialSkill();
}
void BossCapScene::ChangeWeapon(Ref* pSender)
{
	ChangeWeapon();
}
#endif

bool BossCapScene::CheckCameraIfExceedContraint(bool right)
{
	Camera* cam = Camera::getDefaultCamera();
	// Check Right
	if (right)
	{
		return cam->getPositionX() > cameraMaxOffsetX;
	}
	// Check Left
	else
	{
		return cam->getPositionX() < cameraMinOffsetX;
	}
}

void BossCapScene::ConstraintCamera()
{
	Camera* cam = Camera::getDefaultCamera();
	// Check Right
	if(CheckCameraIfExceedContraint(true))
	{
		cam->setPositionX(cameraMaxOffsetX);
	}
	// Check Left
	else if (CheckCameraIfExceedContraint(false))
	{
		cam->setPositionX(cameraMinOffsetX);
	}
}

void BossCapScene::UpdateCamera(bool movingTowardRight)
{
	if (movingTowardRight)
	{
		// Check camera right contraint
		if (!CheckCameraIfExceedContraint(true))
		{
			Camera* cam = Camera::getDefaultCamera();
			Unit* player = (Unit*) this->getChildByTag(PLAYER);

			float currentPlayerPosXInTheScreen = player->getPositionX() - (cam->getPositionX() - cameraOffsetXFromScene);

			if (currentPlayerPosXInTheScreen > visibleSize.width * 0.5f)
			{
				MoveCameraX(currentPlayerPosXInTheScreen - visibleSize.width * 0.5f);
			}
		}
	}
	else
	{
		// Check camera left contraint
		if (!CheckCameraIfExceedContraint(false))
		{
			Camera* cam = Camera::getDefaultCamera();
			Unit* player = (Unit*) this->getChildByTag(PLAYER);

			float currentPlayerPosXInTheScreen = player->getPositionX() - (cam->getPositionX() - cameraOffsetXFromScene);

			if (currentPlayerPosXInTheScreen < visibleSize.width * 0.5f)
			{
				MoveCameraX(currentPlayerPosXInTheScreen - visibleSize.width * 0.5f);
			}
		}
	}
}

void BossCapScene::MoveCameraX(float moveByX)
{
	Camera* cam = Camera::getDefaultCamera();
	cam->setPositionX(cam->getPositionX() + moveByX);

	ConstraintCamera();
}

void BossCapScene::GameEnd(bool win)
{
	currentState = PAUSED;
	unitManager->StopMovingAllUnits();

	player_movement_direction = NO_DIRECTION;
	StopAttack();

	Label* timer = ((Label*)this->getChildByTag(GUI)->getChildByTag(TIMER));
	timer->pause();

	Size foregroundSize(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	Size buttonSize(visibleSize.width * 0.225f, visibleSize.height * 0.125f);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->stopAllEffects();

	if (win)
	{
		if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/mp3/Music/game_win.mp3", true);
		}

		UserDefault* def = UserDefault::getInstance();
		int currentData = def->getIntegerForKey("CurrentData", 1);

		def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "JustWon").c_str(), false);

		int nextBossType = ((Unit*)this->getChildByTag(BOSS))->GetType() + 1;
		def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_RICH) + 1);

		// Menu item Animations
		Menu* wonWindow = (Menu*)this->getChildByTag(GUI)->getChildByTag(WON_WINDOW);
		wonWindow->setVisible(true);

		MenuItem* background = (MenuItem*)wonWindow->getChildByTag(BACKGROUND);
		background->runAction(FadeIn::create(1.0f));

		MenuItem* foreground = (MenuItem*)wonWindow->getChildByTag(FOREGROUND);
		foreground->runAction(Sequence::create(	DelayTime::create(1.0f),
												FadeIn::create(1.0f),
												NULL));

		MenuItem* nextButton = (MenuItem*)wonWindow->getChildByTag(NEXT_BUTTON);
		nextButton->runAction(Sequence::create(	DelayTime::create(2.0f),
												ScaleTo::create(0.25f,	buttonSize.width / nextButton->getContentSize().width,
																		buttonSize.height / nextButton->getContentSize().height),
												NULL));

	}
	else
	{
		if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		{
			SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/mp3/Music/game_lose.mp3", true);
		}

		// Menu item Animations
		Menu* lostWindow = (Menu*)this->getChildByTag(GUI)->getChildByTag(LOST_WINDOW);
		lostWindow->setVisible(true);

		MenuItem* background = (MenuItem*)lostWindow->getChildByTag(BACKGROUND);
		background->runAction(FadeIn::create(1.0f));

		MenuItem* foreground = (MenuItem*)lostWindow->getChildByTag(FOREGROUND);
		foreground->runAction(Sequence::create(	DelayTime::create(1.0f),
												FadeIn::create(1.0f),
												NULL));

		MenuItem* restartButton = (MenuItem*)lostWindow->getChildByTag(RESTART_BUTTON);
		restartButton->runAction(Sequence::create(	DelayTime::create(2.0f),
													ScaleTo::create(0.25f,	buttonSize.width / restartButton->getContentSize().width,
																			buttonSize.height / restartButton->getContentSize().height),
													NULL));

		MenuItem* quitButton = (MenuItem*)lostWindow->getChildByTag(QUIT_BUTTON);
		quitButton->runAction(Sequence::create(	DelayTime::create(2.125f), 
												ScaleTo::create(0.25f,	buttonSize.width / quitButton->getContentSize().width,
																		buttonSize.height / quitButton->getContentSize().height),
												NULL));

	}

	auto gameplayMenu = (Menu*) this->getChildByTag(GUI)->getChildByTag(GAMEPLAY_MENU);
	gameplayMenu->setEnabled(false);
}

void BossCapScene::SpecialSkill()
{
	auto mpBar = (ProgressTimer*)this->getChildByTag(GUI)->getChildByTag(MP_BAR);
	if (mpBar->getPercentage() > 0)
	{
		Unit* player = (Unit*)this->getChildByTag(PLAYER);
		player->Heal(mpBar->getPercentage());
		UseMp(mpBar->getPercentage());
	}
}
void BossCapScene::ChangeWeapon()
{
	Unit* player = (Unit*)this->getChildByTag(PLAYER);
	int type = player->GetType();
	if (type == Unit::HERO_BOW)
	{
		player->ChangeWeapon(Unit::HERO_TWO_HANDED_SWORD);
	}
	else
	{
		player->ChangeWeapon(++type);
	}
	Sprite* currentEquippedWeapon = (Sprite*)this->getChildByTag(GUI)->getChildByTag(CURRENT_EQUIPPED_WEAPON);
	currentEquippedWeapon;
	switch (player->GetType())
	{
		case Unit::HERO_TWO_HANDED_SWORD:
		{
			currentEquippedWeapon->setTexture("CharacterAnimations/stop_sd.png");
			break;
		}
		case Unit::HERO_SPEAR:
		{
			currentEquippedWeapon->setTexture("CharacterAnimations/spear/stop.png");
			break;
		}
		case Unit::HERO_BOW:
		{
			currentEquippedWeapon->setTexture("CharacterAnimations/bow/stop.png");
			break;
		}
	}
}

void BossCapScene::Pause(Ref* pSender)
{
	Director::getInstance()->pause();
	Menu* pauseWindow = (Menu*)this->getChildByTag(GUI)->getChildByTag(PAUSE_WINDOW);
	pauseWindow->setVisible(true);
	currentState = PAUSED;

	auto gameplayMenu = (Menu*) this->getChildByTag(GUI)->getChildByTag(GAMEPLAY_MENU);
	gameplayMenu->setEnabled(false);
}
void BossCapScene::Resume(Ref* pSender)
{
	Director::getInstance()->resume();
	Menu* pauseWindow = (Menu*)this->getChildByTag(GUI)->getChildByTag(PAUSE_WINDOW);
	pauseWindow->setVisible(false);
	currentState = PLAYING;

	auto gameplayMenu = (Menu*) this->getChildByTag(GUI)->getChildByTag(GAMEPLAY_MENU);
	gameplayMenu->setEnabled(true);
}
void BossCapScene::Restart(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->resume();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(0.2f, BossCapScene::createScene()));
}
void BossCapScene::Quit(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->resume();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(0.2f, IntroScene::createScene()));
}
void BossCapScene::WinNext(Ref* pSender)
{
	Director::getInstance()->resume();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	if (def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_RICH) - 1 == Unit::BOSS_ABANDONDED_KNIGHT)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, LoyaltySelectionScene::createScene()));
	}
	else
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, BossSelectionScene::createScene()));
	}
}