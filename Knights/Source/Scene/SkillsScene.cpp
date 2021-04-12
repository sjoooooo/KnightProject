#include "SkillsScene.h"
#include "MercenaryShopScene.h"
#include "IntroScene.h"
#include "StatScene.h"
#include "GameCompleteScene.h"
#include "BossSelectionScene.h"
#include "SimpleAudioEngine.h"
#include "GameObject\Units\Hero.h"
#include "GameObject\Skills\SkillBar.h"
#include "GameObject\Spawner\AISpawner.h"
#include "GameObject\WaveInfo\IncomingWavesInfo.h"
#include "..\Managers\AnimationManager.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* SkillsScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SkillsScene::create();
	scene->addChild(layer);

	return scene;
}

SkillsScene::~SkillsScene()
{
	if (unitManager)
	{
		delete unitManager;
	}
	if (collisionManager)
	{
		delete collisionManager;
	}
	if (skillEffectManager)
	{
		delete skillEffectManager;
	}
	removeAllChildrenWithCleanup(true);
}

bool SkillsScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	touchedSkillbar = false;
	movedSkillbar = false;
	bossSpawnTime = 30.f;
	bossCastSpellTime = bossSpawnTime + 5.0f;
	bossCastedSpell = false;

	InitBackGround();
	InitGameObject();
	InitGUI();

	for (int i = Unit::SWORDSMAN; i < Unit::NUM_OF_TYPES; i++)
	{
		Animate* temp = AnimationManager::GetWalkingAnimation(i, 0);
		temp = AnimationManager::GetAttackingAnimation(i, 0);
		temp = AnimationManager::GetAttackingEffectAnimation(i, 0);
		temp = AnimationManager::GetIdlingAnimation(i, 0);
	}

	Animate* temp = AnimationManager::GetSkillAnimation(Skill::ARROW, 0);
	temp = AnimationManager::GetSkillAnimation(Skill::LIGHTNING, 0);
	temp = AnimationManager::GetSkillAnimation(Skill::METEOR, 0);

	this->schedule(schedule_selector(SkillsScene::update));

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SkillsScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(SkillsScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(SkillsScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);
	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(SkillsScene::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(SkillsScene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);


	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		Unit* boss = (Unit*)this->getChildByTag(BOSS);
		SimpleAudioEngine::getInstance()->playBackgroundMusic(("Sounds/mp3/Music/" + StringUtils::format("%d", boss->GetType() - Unit::BOSS_RICH + 1) + "_stage.mp3").c_str(), true);
	}

	return true;
}
void SkillsScene::update(float ft)
{
	auto gui = (Node*)this->getChildByTag(GUI);
	gui->setPositionX((Camera::getDefaultCamera()->getPositionX() - cameraMinX));
	switch (currentState)
	{
		case PLAYING:
		{
			/* Skill Guage*/
			auto skillguage = (Label*)this->getChildByTag(GUI)->getChildByTag(SKILLGUAGE);
			auto Skill_Guage = (ProgressTimer*)this->getChildByTag(GUI)->getChildByTag(SKILLGUAGEBAR);

			skillguage->setString(StringUtils::format("%d / %d", (int)(maxSkillGuage * (Skill_Guage->getPercentage() / 100)), maxSkillGuage));

			Unit* boss = (Unit*)this->getChildByTag(BOSS);

			/* Timer */
			auto timer = (Label*)this->getChildByTag(GUI)->getChildByTag(TIMER);
			auto countdown = (DelayTime*)timer->getActionByTag(TIMER);
			if (countdown)
			{
				timer->setString(StringUtils::format("Time Left: %ds", (int)(totalDuration - countdown->getElapsed())));

				// Time Elapsed more than boss Spawn Time
				if (countdown->getElapsed() >= bossSpawnTime)
				{
					if (!bossSpawned)
					{
						bossSpawned = true;
						// Boss spawn
						boss->SetActive(true);
						// Move boss to target position
						boss->MoveToTarget();
						// Resize HPBar
						boss->ResizeHPBar();
					}
					if (!bossCastedSpell)
					{
						if (countdown->getElapsed() >= bossCastSpellTime)
						{
							if (boss->GetType() == Unit::BOSS_QUEEN)
							{
								boss->CastSpell(0.5f);
								bossCastedSpell = true;
							}
						}
					}
				}
			}
			else
			{
				GameEnd(false);
			}
			// Enemy AI Spawner
			AISpawner* enemyAISpawner = (AISpawner*)this->getChildByTag(AI_SPAWNER_ENEMY);
			/* Units & Collision */
			// Update unit manager
			if (unitManager)
			{
				unitManager->Update(ft);
				if (boss->GetType() == Unit::BOSS_QUEEN)
				{
					if (boss->GetState() == Unit::CASTING_SPELL && !boss->getActionByTag(Unit::CASTING_SPELL))
					{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
							SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/Mass_unit_spawn.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
							SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/Mass_unit_spawn.wav");
#endif
						enemyAISpawner->GetSpawner()->Spawn(&unitSize, &spawnPositionLeft, Unit::ENEMY_MASS_UNIT, 1);
						boss->Attack();
					}
				}
				if (skillEffectManager)
				{
					skillEffectManager->Update(ft);
					if (collisionManager)
						collisionManager->CheckCollision(unitManager, skillEffectManager);
				}
			}
			/*********************/

			/* GUI */
			enemyAISpawner->Update(ft);
			// Enemy incoming waves info
			IncomingWavesInfo* iwi = (IncomingWavesInfo*)this->getChildByTag(GUI)->getChildByTag(INCOMING_WAVE_INFO);
			if (iwi)
				iwi->Update(ft);
			/******/

			/* Winning/Losing Condition */
			// If player character died
			if (!((Unit*)this->getChildByTag(PLAYER_CHARACTER))->IfActive())
			{
				GameEnd(false);
			}
			// If enemy castle destroyed
			if (!((Unit*)this->getChildByTag(ENEMY_CASTLE))->IfActive())
			{
				GameEnd(true);
			}
			/****************************/
			break;
		}
		case PRESTART:
		{
			auto countDown = (Label*)this->getChildByTag(GUI)->getChildByTag(COUNTDOWN);
			if (countDown->getNumberOfRunningActions() == 0)
			{
				if (--currentCountdownNumber == 0)
				{
					countDown->setString("Start!!!");
					countDown->runAction(Sequence::create(	ScaleTo::create(0.5f,	visibleSize.height / countDown->getContentSize().width,
																					visibleSize.height / countDown->getContentSize().height),
															FadeOut::create(1.5f),
															RemoveSelf::create(),
															NULL));

					// Timer
					Label* timer = ((Label*)this->getChildByTag(GUI)->getChildByTag(TIMER));

					auto countdown = DelayTime::create(totalDuration);
					countdown->setTag(TIMER);
					timer->runAction(countdown);

					// skill guage
					auto Skill_Guage = (ProgressTimer*)this->getChildByTag(GUI)->getChildByTag(SKILLGUAGEBAR);
					float duration = maxSkillGuage / skillGuageRecoverySpeed;
					Skill_Guage->runAction(ProgressTo::create(duration, 100));
					// Gameplay Menu
					((Menu*)this->getChildByTag(GUI)->getChildByTag(GAMEPLAY_MENU))->setEnabled(true);

					// Move Player to target position
					((Unit*)this->getChildByTag(PLAYER_CHARACTER))->MoveToTarget();

					// Allies
					SpawnAllies();

					currentState = PLAYING;
				}
				else
				{
					countDown->setString(StringUtils::format("%d", currentCountdownNumber));
					countDown->runAction(Sequence::create(	ScaleTo::create(0.5f,	visibleSize.height / countDown->getContentSize().width,
																					visibleSize.height / countDown->getContentSize().height),
															ScaleTo::create(0.5f, 0),
															NULL));
				}
				
			}
			break;
		}
	}
}

void SkillsScene::InitBackGround()
{
	// Camera
	cameraMaxX = visibleSize.width * 1.2f + origin.x;
	cameraMinX = visibleSize.width * 0.5f + origin.x;
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
		case Unit::BOSS_RICH:
		{
			background = Sprite::create("Background/1_stage.png");
			break;
		}
		case Unit::BOSS_FALLEN_ALCHEMIST:
		{
			background = Sprite::create("Background/2_stage.png");
			break;
		}
		case Unit::BOSS_ESCAPED_PRISONER:
		{
			background = Sprite::create("Background/3_stage.png");
			break;
		}
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			background = Sprite::create("Background/4_stage.png");
			break;
		}
		case Unit::BOSS_COMMANDER:
		{
			background = Sprite::create("Background/5_stage_traitor.png");
			break;
		}
		case Unit::BOSS_QUEEN:
		{
			background = Sprite::create("Background/6_stage_traitor.png");
			break;
		}
		case Unit::BOSS_WIZARD:
		{
			background = Sprite::create("Background/5_stage_loyalist.png");
			break;
		}
		case Unit::BOSS_KING:
		{
			background = Sprite::create("Background/6_stage_loyalist.png");
			break;
		}
	}

	background->setAnchorPoint(Vec2(0, 0));
	background->setScale(	(visibleSize.width * 1.7f / background->getContentSize().width),
							(visibleSize.height * 0.7 / background->getContentSize().height));
	background->setPositionY(visibleSize.height * 0.3 + origin.y);
	this->addChild(background, 0);

}
void SkillsScene::InitGUI()
{
	auto gui = Node::create();
	gui->setTag(GUI);
	this->addChild(gui, 500);

	Vector<MenuItem*> MenuItems;

	// Skill bar
	float skillSize = visibleSize.height * 0.15f;

	auto skillbar = new Skillbar;
	// skillbar Init
	Size skillbarSize(visibleSize.width, skillSize);
	Vec2 skillbarPosition(visibleSize.width * 0.5f + origin.x,
		skillSize * 0.5f + origin.y);
	skillbar->Init(	&skillbarSize,
					&skillbarPosition,
					skillSize * 0.3225f);
	skillbar->setTag(SKILLBAR);
	gui->addChild(skillbar, 0);

	// Incoming WaveInfo
	/*IncomingWavesInfo* iwi = new IncomingWavesInfo;
	iwi->Init((AISpawner*)this->getChildByTag(AI_SPAWNER_ENEMY), &Size(visibleSize.width * 0.3f, visibleSize.height * 0.15f));
	iwi->setPosition(	visibleSize.width + origin.x - iwi->GetSize().width * 0.55f,
	visibleSize.height + origin.y - iwi->GetSize().height * 0.55f);
	iwi->setTag(INCOMING_WAVE_INFO);
	gui->addChild(iwi, 0);*/

	// HP BAR
	Size hpBarSize(visibleSize.width * 0.4f, visibleSize.height * 0.03f);
	// HP BAR: PLAYER
	Hero* player = (Hero*)this->getChildByTag(PLAYER_CHARACTER);
	player->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png", false);
	HPBar* hpbar = player->GetHPBar();
	hpbar->Resize(hpBarSize);
	hpbar->setPosition(visibleSize.width * 0.3f + origin.x,
		visibleSize.height * 0.95f + origin.y);
	gui->addChild(hpbar, 0);

	// HP BAR: Castle
	Unit* enemyCastle = (Unit*)this->getChildByTag(ENEMY_CASTLE);
	enemyCastle->InitHPBar("HPBar/hp_bar_background.png", "HPBar/enemy_hp_bar.png", "HPBar/hp_bar_outline.png", false);
	HPBar* hpbarEnemyCastle = enemyCastle->GetHPBar();
	hpbarEnemyCastle->Resize(hpBarSize, false);
	hpbarEnemyCastle->setPosition(visibleSize.width * 0.7f + origin.x,
		visibleSize.height * 0.95f + origin.y);
	enemyCastle->SetType(Unit::CASTLE);
	gui->addChild(hpbarEnemyCastle, 0);


	// HP BAR : PLYAER HEART
	Sprite* player_heart = Sprite::create("KnightResources/player_hp_bar_heart.png");
	player_heart->setPosition(visibleSize.width * 0.1f + origin.x,
		visibleSize.height * 0.95f + origin.y);
	gui->addChild(player_heart, 1);

	//HP BAR : CASTLE HEART
	Sprite* enemy_heart = Sprite::create("KnightResources/enemy_castle_hp_bar_heart.png");
	enemy_heart->setPosition(visibleSize.width * 0.9f + origin.x,
		visibleSize.height * 0.95f + origin.y);
	gui->addChild(enemy_heart, 1);

	// Pause
	Size pauseButtonSize(visibleSize.height * 0.1f, visibleSize.height * 0.1f);
	auto pause = MenuItemImage::create(	"PauseWindow/button.png", "PauseWindow/push_button.png",
										CC_CALLBACK_1(SkillsScene::Pause, this));
	pause->setScale(pauseButtonSize.width / pause->getContentSize().width,
					pauseButtonSize.height / pause->getContentSize().height);
	pause->setPosition(	visibleSize.width * 0.5f - pauseButtonSize.width * 0.55f,
						visibleSize.height * 0.5f - pauseButtonSize.height * 0.55f);
	pause->setTag(PAUSE_BUTTON);
	MenuItems.pushBack(pause);


	// Timer
	totalDuration = 120.f;
	Label* timer = Label::createWithTTF(StringUtils::format("Time Left: %ds", (int)totalDuration), "MiniGameResources/CuteFont.ttf", visibleSize.height * 0.04f);
	timer->setPosition(visibleSize.width * 0.5f + origin.x,
		visibleSize.height * 0.9f + origin.y);
	timer->setColor(Color3B::WHITE);
	timer->setAnchorPoint(Point(0.5f, 0.5f));

	timer->setTag(TIMER);
	gui->addChild(timer, 0);

	//Skill_Guage
	CCSprite *timer_gaze = CCSprite::create("HPBar/skillbar.png");
	ProgressTimer *Skill_Guage = ProgressTimer::create(timer_gaze);
	Skill_Guage->setPosition(	visibleSize.width * 0.2f + origin.x,
								visibleSize.height * 0.25f + origin.y);
	Skill_Guage->setPercentage(0.f);
	Skill_Guage->setMidpoint(ccp(0, 0.5));
	Skill_Guage->setBarChangeRate(ccp(1, 0));
	Skill_Guage->setType(kCCProgressTimerTypeBar);
	Skill_Guage->setTag(SKILLGUAGEBAR);
	gui->addChild(Skill_Guage, 2);

	//Skill_Guage Number
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	int intPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::INT_PLUS_BUTTON)).c_str(), 0);
	int prcPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::PRC_PLUS_BUTTON)).c_str(), 0);
	maxSkillGuage = 100 + prcPoints * 5;
	skillGuageRecoverySpeed = 2 + intPoints;
	Label* skillguage = Label::createWithTTF(StringUtils::format("0 / %d", maxSkillGuage), "MiniGameResources/CuteFont.ttf", visibleSize.height * 0.04f);
	skillguage->setPosition(visibleSize.width * 0.4f + origin.x,
	visibleSize.height * 0.25f + origin.y);
	skillguage->setColor(Color3B::WHITE);
	skillguage->setAnchorPoint(Point(0.0f, 0.5f));
	skillguage->setTag(SKILLGUAGE);
	gui->addChild(skillguage, 0);

	// gameplay menu
	auto gameplayMenu = Menu::createWithArray(MenuItems);
	gameplayMenu->setTag(GAMEPLAY_MENU);
	gui->addChild(gameplayMenu, 1);

	gameplayMenu->setEnabled(false);

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
		CC_CALLBACK_1(SkillsScene::Resume, this));
	resume->setScale(PauseWindowButtonSize.width / resume->getContentSize().width,
		PauseWindowButtonSize.height / resume->getContentSize().height);
	resume->setPosition(-visibleSize.width * 0.3f, 0);
	resume->setTag(RESUME_BUTTON);
	PauseItems.pushBack(resume);
	// Restart Button
	auto restart = MenuItemImage::create("PauseWindow/retry.png", "PauseWindow/push_retry.png",
		CC_CALLBACK_1(SkillsScene::Restart, this));
	restart->setScale(PauseWindowButtonSize.width / restart->getContentSize().width,
		PauseWindowButtonSize.height / restart->getContentSize().height);
	restart->setTag(RESTART_BUTTON);
	PauseItems.pushBack(restart);
	// Quit button
	auto quit = MenuItemImage::create("PauseWindow/mainmenu.png", "PauseWindow/push_menu.png",
		CC_CALLBACK_1(SkillsScene::Quit, this));
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
										CC_CALLBACK_1(SkillsScene::WinNext, this));
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
									CC_CALLBACK_1(SkillsScene::Restart, this));
	restart->setScale(0);
	restart->setPosition(-visibleSize.width * 0.2f, -visibleSize.height * 0.25f);
	restart->setTag(RESTART_BUTTON);
	LostItems.pushBack(restart);
	// Quit button
	quit = MenuItemImage::create(	"PauseWindow/mainmenu.png", "PauseWindow/push_menu.png",
									CC_CALLBACK_1(SkillsScene::Quit, this));
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


	/* CountDown */
	currentState = PRESTART;
	currentCountdownNumber = 3;

	Label* countDown = Label::createWithTTF(StringUtils::format("%d", currentCountdownNumber), "MiniGameResources/CuteFont.ttf", visibleSize.height * 0.04f);
	countDown->setPosition(	visibleSize.width * 0.5f + origin.x,
							visibleSize.height * 0.5f + origin.y);
	countDown->setScale(0);
	countDown->setColor(Color3B::RED);
	countDown->setAnchorPoint(Vec2(0.5f, 0.5f));

	countDown->setTag(COUNTDOWN);
	gui->addChild(countDown, 1);

	countDown->runAction(Sequence::create(	ScaleTo::create(0.5f,	visibleSize.height  / countDown->getContentSize().width,
																	visibleSize.height / countDown->getContentSize().height),
											ScaleTo::create(0.5f, 0),
											NULL));
	/*************/

}
void SkillsScene::InitGameObject()
{
	/* Create new unit manager */
	unitManager = new UnitManager;
	/* Create new collision manager */
	collisionManager = new CollisionManager;
	/* Create new skill effect manager */
	skillEffectManager = new SkillEffectManager;

	/* Set Sizing */
	heroSize.setSize(visibleSize.width * 0.1f, visibleSize.height * 0.25f);
	unitSize.setSize(visibleSize.width * 0.1f, visibleSize.height * 0.2f);
	spawnPositionLeft.set(	origin.x,
							origin.y + visibleSize.height * 0.3f);
	spawnPositionRight.set(	origin.x + visibleSize.width * 1.7f,
							origin.y + visibleSize.height * 0.3f);
	heroOffset.set(0, visibleSize.height * 0.05f);
	Size castleSize(visibleSize.width * 0.3f, visibleSize.height * 0.6f);


	/* Player */
	auto player = new Hero();
	player->m_totalDistanceToMove = (spawnPositionRight - spawnPositionLeft).length();
	player->SetType(Unit::HERO);
	player->setTag(PLAYER_CHARACTER);
	// Set Player's Sprite
	player->SetSprite(Sprite::create("CharacterAnimations/stop_sd.png"));
	Sprite* playerSpr = player->GetSprite();
	playerSpr->setAnchorPoint(Vec2(0.5f, 0));
	playerSpr->setScale(heroSize.width / playerSpr->getContentSize().width,
						heroSize.height / playerSpr->getContentSize().height);

	// Get Stat points
	player->SelfSetInfoAccordingToTypeAndLocations(spawnPositionRight + heroOffset, spawnPositionLeft + heroOffset);

	// Set Position to the Left
	player->setPosition(spawnPositionLeft + heroOffset);

	// Add player to unit manager
	player->SetActive(true);
	this->addChild(player, 2);
	unitManager->SetPlayer(player);

	/* Player Unit's Spawner */
	Spawner* playerSpawner = new Spawner();
	playerSpawner->setTag(SPAWNER_PLAYER);
	playerSpawner->SetSpawnLocation(spawnPositionLeft, heroOffset.y);
	this->addChild(playerSpawner, 0);

	/* Player Units */
	for (int i = 0; i < 100; i++)
	{
		Unit* unit = new Unit();
		unit->m_totalDistanceToMove = (spawnPositionRight - spawnPositionLeft).length();
		unit->SetSprite(Sprite::create());
		unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
		unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/hp_bar.png", "HPBar/hp_bar_outline.png");
		unit->SetActive(false);
		unit->setTag(PLAYER_UNITS + i);
		this->addChild(unit, 3);

		playerSpawner->AddUnit(unit);
		unitManager->AddPlayerUnit(unit);
	}

	InitEnemies(castleSize);

	
	/* Skill Effects */
	for (int i = 0; i < 100; i++)
	{
		SkillEffect* skillEffect = new SkillEffect();
		skillEffect->Init();
		skillEffect->SetActive(false);
		this->addChild(skillEffect, 499);

		skillEffectManager->AddSkillEffect(skillEffect);
	}
	skillEffectManager;
}

void SkillsScene::InitEnemies(Size castleSize)
{
	/* Boss */
	SpawnBoss();

	/* Enemy Spawner */
	Spawner* enemySpawner = new Spawner();
	enemySpawner->setTag(SPAWNER_ENEMY);
	enemySpawner->SetSpawnLocation(spawnPositionRight, heroOffset.y);
	this->addChild(enemySpawner, 0);

	/* Enemy Units */
	for (int i = 0; i < 100; i++)
	{
		Unit* unit = new Unit();
		unit->m_totalDistanceToMove = (spawnPositionRight - spawnPositionLeft).length();
		unit->SetSprite(Sprite::create());
		unit->GetSprite()->setAnchorPoint(Vec2(0.5f, 0));
		unit->InitHPBar("HPBar/hp_bar_background.png", "HPBar/enemy_hp_bar.png", "HPBar/hp_bar_outline.png");
		unit->SetActive(false);
		unit->setTag(ENEMY_UNITS + i);
		this->addChild(unit, 3);

		enemySpawner->AddUnit(unit);
		unitManager->AddEnemyUnit(unit);
	}

	/* Enemy AI Spawner */
	AISpawner* enemyAISpawner = new AISpawner();
	enemyAISpawner->setTag(AI_SPAWNER_ENEMY);
	enemyAISpawner->SetSpawner(enemySpawner);
	this->addChild(enemyAISpawner, 0);

	InitEnemySpawnPatterns(enemyAISpawner);

	/* Enemy Castle*/
	Unit* enemyCastle = new Unit();
	enemyCastle->m_totalDistanceToMove = (spawnPositionRight - spawnPositionLeft).length();

	auto boss = (Unit*)this->getChildByTag(BOSS);
	switch (boss->GetType())
	{
		case Unit::BOSS_RICH:
		{
			enemyCastle->SetSprite(Sprite::create("Castles/1-2_build.png"));
			break;
		}
		case Unit::BOSS_FALLEN_ALCHEMIST:
		{
			enemyCastle->SetSprite(Sprite::create("Castles/1-2_build.png"));
			break;
		}
		case Unit::BOSS_ESCAPED_PRISONER:
		{
			enemyCastle->SetSprite(Sprite::create("Castles/3-4_build.png"));
			break;
		}
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			enemyCastle->SetSprite(Sprite::create("Castles/3-4_build.png"));
			break;
		}
		default:
		{
			enemyCastle->SetSprite(Sprite::create("Castles/5-6_build.png"));
			break;
		}
	}
	enemyCastle->setTag(ENEMY_CASTLE);
	enemyCastle->SetInfo(	250, 250, 0,
							0, 0, 0,
							0,
							0, spawnPositionRight, spawnPositionRight);
	Sprite* enemyCastleSprite = enemyCastle->GetSprite();
	enemyCastleSprite->setAnchorPoint(Vec2(0, 0));
	enemyCastleSprite->setScale(castleSize.width / enemyCastleSprite->getContentSize().width,
								castleSize.height / enemyCastleSprite->getContentSize().height);
	enemyCastle->setPosition(spawnPositionRight - Vec2(castleSize.width * 0.5f, 0));
	enemyCastle->SetActive(true);
	unitManager->SetEnemyCastle(enemyCastle);
	this->addChild(enemyCastle, 1);

}

void SkillsScene::InitEnemySpawnPatterns(AISpawner* enemyAISpawner)
{
	/* Enemy Pattern */

	auto boss = (Unit*)this->getChildByTag(BOSS);
	switch (boss->GetType())
	{
		case Unit::BOSS_RICH:
		{
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	2,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(6.f,	2,	Unit::ENEMY_SLAVE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	1,	Unit::ENEMY_PEOPLE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_SLAVE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);

			enemyAISpawner->AddSpawnPattern(6.f,	1,	Unit::ENEMY_PEOPLE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	3,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);// Boss Spawned: 30s
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_SLAVE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);

			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_PEOPLE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	1,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(10.f,	4,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(15.f,	4,	Unit::ENEMY_SLAVE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);

			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_SLAVE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(10.f,	2,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	4,	Unit::ENEMY_SLAVE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	4,	Unit::ENEMY_SLAVE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);

			enemyAISpawner->AddSpawnPattern(5.f,	5,	Unit::ENEMY_FARMER, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	5,	Unit::ENEMY_SLAVE, unitSize, spawnPositionLeft);
			break;
		}
		case Unit::BOSS_FALLEN_ALCHEMIST:
		{
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_ALCHEMIST, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_BEGGAR, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	1,	Unit::ENEMY_SLAVE, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(1.f,	1,	Unit::ENEMY_BEGGAR, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_BEGGAR, unitSize, spawnPositionLeft);

			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_ALCHEMIST, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(1.f,	1,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(7.f,	1,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(7.f,	1,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);// Boss Spawned: 30s
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_BEGGAR, unitSize, spawnPositionLeft);

			enemyAISpawner->AddSpawnPattern(5.f,	1,	Unit::ENEMY_BEGGAR, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	3,	Unit::ENEMY_ALCHEMIST, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(10.f,	2,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(10.f,	2,	Unit::ENEMY_ALCHEMIST, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	3,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);

			enemyAISpawner->AddSpawnPattern(10.f,	1,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_ALCHEMIST, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(10.f,	1,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_ALCHEMIST, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(3.f,	2,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);

			enemyAISpawner->AddSpawnPattern(3.f,	2,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(9.f,	3,	Unit::ENEMY_ALCHEMIST, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	3,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(15.f,	2,	Unit::ENEMY_ALCHEMIST, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);

			enemyAISpawner->AddSpawnPattern(0.f,	3,	Unit::ENEMY_BEGGAR, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	1,	Unit::ENEMY_BARON, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_BEGGAR, unitSize, spawnPositionLeft);
			break;
		}
		case Unit::BOSS_ESCAPED_PRISONER:
		{
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	1,	Unit::ENEMY_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	2,	Unit::ENEMY_SPEARMAN, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(3.f,	1,	Unit::ENEMY_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_FIRE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	1,	Unit::ENEMY_SHIELDMAN, unitSize, spawnPositionLeft); // Boss Spawned: 30s
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_SPEARMAN, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(6.f,	2,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(3.f,	1,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_FIRE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(6.f,	2,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_SHIELDMAN, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(10.f,	2,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(10.f,	2,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	4,	Unit::ENEMY_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(15.f,	3,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_FIRE_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(15.f,	3,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_SPEARMAN, unitSize, spawnPositionLeft);
			break;
		}
		case Unit::BOSS_ABANDONDED_KNIGHT:
		{
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	2,	Unit::ENEMY_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	2,	Unit::ENEMY_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(3.f,	1,	Unit::ENEMY_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	1,	Unit::ENEMY_LIGHTNING_WIZARD, unitSize, spawnPositionLeft); // Boss Spawned: 30s
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_ICE_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(6.f,	4,	Unit::ENEMY_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	3,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	5,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(3.f,	2,	Unit::ENEMY_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(6.f,	2,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(10.f,	3,	Unit::ENEMY_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(10.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(15.f,	3,	Unit::ENEMY_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_ICE_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(15.f,	2,	Unit::ENEMY_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_ICE_WIZARD, unitSize, spawnPositionLeft);
			break;
		}
		case Unit::BOSS_COMMANDER:
		{
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(4.f,	1,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	2,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	3,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft); // Boss Spawned: 30s
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(3.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			break;
		}
		case Unit::BOSS_QUEEN:
		{
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(4.f,	1,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	2,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	3,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft); // Boss Spawned: 30s
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(3.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			break;
		}
		case Unit::BOSS_WIZARD:
		{
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(4.f,	1,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	2,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	3,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft); // Boss Spawned: 30s
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(3.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			break;
		}
		case Unit::BOSS_KING:
		{
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(4.f,	1,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	2,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(4.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(2.f,	3,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(2.f,	1,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft); // Boss Spawned: 30s
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(5.f,	3,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_STRENGTHEN_ICE_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(5.f,	2,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(3.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_LIGHTNING_WIZARD, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_STRENGTHEN_PLATE_KNIGHT, unitSize, spawnPositionLeft);
			
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	3,	Unit::ENEMY_HEAVY_ARMOR_KNIGHT, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(0.f,	2,	Unit::ENEMY_HEAVY_ARMOR_SPEARMAN, unitSize, spawnPositionLeft);
			enemyAISpawner->AddSpawnPattern(8.f,	2,	Unit::ENEMY_STRENGTHEN_SHIELDMAN, unitSize, spawnPositionLeft);
			break;
		}
	}
}


bool SkillsScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	Point location = touch->getLocation();
	prevTouchLocation = location;

	switch (currentState)
	{
		case PLAYING:
		{
			Skillbar* skillbar = (Skillbar*)this->getChildByTag(GUI)->getChildByTag(SKILLBAR);
			// if touched skillbar
			if (skillbar->GetBackground()->getBoundingBox().containsPoint(location))
			{
				touchedSkillbar = true;
			}
			break;
		}
	}

	return true;
}
void SkillsScene::onTouchMoved(Touch *touch, Event *unused_event)
{
	Point location = touch->getLocation();

	switch (currentState)
	{
	case PLAYING:
	{
		float movebyX = location.x - prevTouchLocation.x;
		if (touchedSkillbar)
		{
			/*movedSkillbar = true;
			Skillbar* skillbar = (Skillbar*)this->getChildByTag(GUI)->getChildByTag(SKILLBAR);
			skillbar->MoveSkillsX(movebyX);*/
		}
		else
		{
			MoveCameraX(movebyX);

		}
		break;
	}
	}
	prevTouchLocation = location;
}
void SkillsScene::onTouchEnded(Touch *touch, Event *unused_event)
{
	Point location = touch->getLocation();
	switch (currentState)
	{
	case PLAYING:
	{
		if (touchedSkillbar)
		{
			touchedSkillbar = false;

			if (movedSkillbar)
			{
				movedSkillbar = false;
				Skillbar* skillbar = (Skillbar*)this->getChildByTag(GUI)->getChildByTag(SKILLBAR);
				skillbar->ContraintsMovedSkillsX();
			}
			else
			{
				// Skills
				Skillbar* skillbar = (Skillbar*)this->getChildByTag(GUI)->getChildByTag(SKILLBAR);
				Skill* skill = skillbar->TouchedSkill(&location);
				// if touched skill
				if (skill)
				{
					if (Skillusage(skill->GetCost()))
					{
						switch (skill->getTag())
						{
							// AOE
							case Skill::ARROW:
							{
								Hero* player = (Hero*)this->getChildByTag(PLAYER_CHARACTER);
								float totalDistance = spawnPositionRight.x - spawnPositionLeft.x;
								Size size(totalDistance * (10.0f / 100.0f), unitSize.height * 2.f);
								Vec2 position(player->getPosition() + Vec2(totalDistance * (1.0f / 100.0f) + size.width * 0.5f + player->getContentSize().width * player->getScaleX() * 0.5f, 0.0f));
								position.y = spawnPositionRight.y;
								skill->Cast(player,
									skillEffectManager->GetInactiveSkillEffect(),
									&position,
									&size);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/arrows.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/arrows.wav");
#endif
								break;
							}
							case Skill::LIGHTNING:
							{
								Hero* player = (Hero*)this->getChildByTag(PLAYER_CHARACTER);
								float totalDistance = spawnPositionRight.x - spawnPositionLeft.x;
								Size size(totalDistance * (20.0f / 100.0f), unitSize.height * 4.f);
								Vec2 position(player->getPosition() + Vec2(totalDistance * (3.0f / 100.0f) + size.width * 0.5f + player->getContentSize().width * player->getScaleX() * 0.5f, 0.0f));
								position.y = spawnPositionRight.y;
								skill->Cast(player,
									skillEffectManager->GetInactiveSkillEffect(),
									&position,
									&size);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/lightning.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/lightning.wav");
#endif
								break;
							}
							case Skill::METEOR:
							{
								Hero* player = (Hero*)this->getChildByTag(PLAYER_CHARACTER);
								float totalDistance = spawnPositionRight.x - spawnPositionLeft.x;
								Size size(totalDistance * (15.0f / 100.0f), unitSize.height * 4.f);
								Vec2 position(player->getPosition() + Vec2(totalDistance * (2.0f / 100.0f) + size.width * 0.5f + player->getContentSize().width * player->getScaleX() * 0.5f, 0.0f));
								position.y = spawnPositionRight.y;
								skill->Cast(player,
									skillEffectManager->GetInactiveSkillEffect(),
									&position,
									&size);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/metor.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/metor.wav");
#endif
								break;
							}
							case Skill::ICE_SPEAR:
							{
								Hero* player = (Hero*)this->getChildByTag(PLAYER_CHARACTER);
								float totalDistance = spawnPositionRight.x - spawnPositionLeft.x;
								Size size(totalDistance * (5.0f / 100.0f), unitSize.height * 0.5);
								Vec2 position(player->getPosition() + Vec2(totalDistance * (10.0f / 100.0f) + player->getContentSize().width * player->getScaleX() * 0.5f, 0.0f));
								skill->Cast(player,
									skillEffectManager->GetInactiveSkillEffect(),
									&position,
									&size);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/ice_spear.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/ice_spear.wav");
#endif
								break;
							}
							case Skill::HURRICANE:
							{
								Hero* player = (Hero*)this->getChildByTag(PLAYER_CHARACTER);
								float totalDistance = spawnPositionRight.x - spawnPositionLeft.x;
								Size size(totalDistance * (13.0f / 100.0f), unitSize.height * 2.f);
								Vec2 position(player->getPosition() + Vec2(totalDistance * (0.0f / 100.0f) + size.width * 0.5f + player->getContentSize().width * player->getScaleX() * 0.5f, 0.0f));
								position.y = spawnPositionRight.y;
								skill->Cast(player,
									skillEffectManager->GetInactiveSkillEffect(),
									&position,
									&size);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/hurricane.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/hurricane.wav");
#endif
								break;
							}
							case Skill::FIREBALL:
							{
								Hero* player = (Hero*)this->getChildByTag(PLAYER_CHARACTER);
								float totalDistance = spawnPositionRight.x - spawnPositionLeft.x;
								Size size(totalDistance * (5.0f / 100.0f), unitSize.height * 0.5);
								Vec2 position(player->getPosition() + Vec2(totalDistance * (10.0f / 100.0f) + player->getContentSize().width * player->getScaleX() * 0.5f, 0.0f));
								skill->Cast(player,
									skillEffectManager->GetInactiveSkillEffect(),
									&position,
									&size);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/fire_ball.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
								SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/fire_ball.wav");
#endif
								break;
							}
							// HEALING
							case Skill::HEAL:
							{
								skill->Cast((Hero*)this->getChildByTag(PLAYER_CHARACTER));
							}
							break;
							case Skill::RECOVERY_POTION:
							{
								skill->Cast((Hero*)this->getChildByTag(PLAYER_CHARACTER));
							}
							break;
							// Others
							case Skill::BACKSTEP:
							{
								Hero* player = (Hero*)this->getChildByTag(PLAYER_CHARACTER);
								// Not moving
								if (player->IfMovingRight() &&
									player->GetState() != Unit::IDLING &&
									player->getPositionX() != spawnPositionLeft.x)
								{
									player->SetMovingRight(false);
									Vec2 distanceToMove((2.0f * player->GetMovementSpeed()) * 0.01f * (spawnPositionLeft.x - spawnPositionRight.x), 0.0f);
									if ((player->getPosition() + distanceToMove).x < spawnPositionLeft.x)
									{
										distanceToMove.x = spawnPositionLeft.x - player->getPositionX();
									}
									player->MoveBy(distanceToMove);
									player->GetSprite()->setFlipX(false);
									unitManager->PlayerBackstep();
									skill->runAction(ProgressFromTo::create(5.0f, 0.0f, 100.0f));
								}
							}
							break;
							// Summon
							default:
							{
								skill->Summon(&unitSize, &spawnPositionRight, (Spawner*)this->getChildByTag(SPAWNER_PLAYER));
							}
							break;
						}
					}
				}
				else
				{
					
				}
			}
		}
	}
	}
}


// Movement
void SkillsScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	{
		//MovePlayerCharacter(false);
		break;
	}
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	{
		//MovePlayerCharacter(true);
		break;
	}
	}
}
void SkillsScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		//StopMovingPlayerCharacter();
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		//StopMovingPlayerCharacter();
		break;
	}
}

// Camera
void SkillsScene::MoveCameraX(float moveByX)
{
	Camera* cam = Camera::getDefaultCamera();
	cam->setPositionX(cam->getPositionX() - moveByX);

	if (cam->getPositionX() > cameraMaxX)
	{
		cam->setPositionX(cameraMaxX);
	}
	else if (cam->getPositionX() < cameraMinX)
	{
		cam->setPositionX(cameraMinX);
	}
}

void SkillsScene::Pause(Ref* pSender)
{
	Director::getInstance()->pause();
	Menu* pauseWindow = (Menu*)this->getChildByTag(GUI)->getChildByTag(PAUSE_WINDOW);
	pauseWindow->setVisible(true);
	currentState = PAUSED;

	auto gameplayMenu = (Menu*) this->getChildByTag(GUI)->getChildByTag(GAMEPLAY_MENU);
	gameplayMenu->setEnabled(false);
}
void SkillsScene::Resume(Ref* pSender)
{
	Director::getInstance()->resume();
	Menu* pauseWindow = (Menu*)this->getChildByTag(GUI)->getChildByTag(PAUSE_WINDOW);
	pauseWindow->setVisible(false);
	currentState = PLAYING;

	auto gameplayMenu = (Menu*) this->getChildByTag(GUI)->getChildByTag(GAMEPLAY_MENU);
	gameplayMenu->setEnabled(true);
}
void SkillsScene::Restart(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->resume();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(0.2f, SkillsScene::createScene()));
}
void SkillsScene::Quit(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->resume();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(0.2f, IntroScene::createScene()));
}
void SkillsScene::WinNext(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->resume();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

	Unit* boss = (Unit*)this->getChildByTag(BOSS);
	if (boss->GetType() == Unit::BOSS_QUEEN ||
		boss->GetType() == Unit::BOSS_KING)
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, GameCompleteScene::createScene()));
	}
	else
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, MercenaryShopScene::createScene()));
	}
}

void SkillsScene::GameEnd(bool win)
{
	currentState = PAUSED;
	unitManager->StopMovingAllUnits();
	AISpawner* enemyAISpawner = (AISpawner*)this->getChildByTag(AI_SPAWNER_ENEMY);
	enemyAISpawner->StopSpawning();

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

		// Set add new points for stat to true
		UserDefault* def = UserDefault::getInstance();
		int currentData = def->getIntegerForKey("CurrentData", 1);

		def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "JustWon").c_str(), true);

		def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_AddNewPoints").c_str(), true);

		int totalGoldLeft = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentGold").c_str(), 0);
		// Add gold gained this stage
		totalGoldLeft += 1000;
		auto countdown = (DelayTime*)timer->getActionByTag(TIMER);
		float timeLeft = totalDuration - countdown->getElapsed();
		if (timeLeft > 60.0f)
			totalGoldLeft += 500;
		if (timeLeft > 90.0f)
			totalGoldLeft += 300;

		def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentGold").c_str(), totalGoldLeft);

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

bool SkillsScene::Skillusage(int usage)
{
	auto Skill_Guage = (ProgressTimer*)this->getChildByTag(GUI)->getChildByTag(SKILLGUAGEBAR);
	float newpercentage = (maxSkillGuage * (Skill_Guage->getPercentage() / 100.f) - usage) / maxSkillGuage * 100.f;
	if (newpercentage < 0)
		return false;
	Skill_Guage->stopAllActions();
	Skill_Guage->setPercentage(newpercentage);
	float duration = maxSkillGuage / skillGuageRecoverySpeed;
	duration *= ((100 - Skill_Guage->getPercentage()) / 100.f);
	Skill_Guage->runAction(ProgressTo::create(duration, 100));
	return true;
}
void SkillsScene::SpawnAllies()
{
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);

	/* Allies */
	Size allySize(heroSize);
	int offsetX = 0.0f;
	for (int i = 0; i <= Unit::BOSS_KING - Unit::BOSS_RICH; i++)
	{
		if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_AllySelected" + StringUtils::format("%d", i)).c_str(), false))
		{
			Spawner* player_spawner = (Spawner*)this->getChildByTag(SPAWNER_PLAYER);
			Vec2 spawnLocation(player_spawner->GetSpawnLocation() + Vec2(offsetX, 0.0f));
			if (i == 5)
			{
				player_spawner->Spawn(&allySize, &spawnPositionRight, Unit::BOSS_RICH + i + 1, 1, &spawnLocation);
			}
			else
			{
				player_spawner->Spawn(&allySize, &spawnPositionRight, Unit::BOSS_RICH + i, 1, &spawnLocation);
			}
			offsetX -= heroSize.width;
		}
	}
	/**********/
}

void SkillsScene::SpawnBoss()
{
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);

	/* Boss */
	Size bossSize(heroSize);
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
	boss->SelfSetInfoAccordingToTypeAndLocations(spawnPositionLeft + heroOffset, spawnPositionRight + heroOffset);
	boss->SetSprite(Sprite::create("CharacterAnimations/stop_sd.png")); 
	boss->SetStopTexture();
	// Set boss's Sprite
	Sprite* bossSpr = boss->GetSprite();
	bossSpr->setAnchorPoint(Vec2(0.5f, 0));
	bossSpr->setScale(	bossSize.width / bossSpr->getContentSize().width,
						bossSize.height / bossSpr->getContentSize().height);
	// HP Bar
	boss->InitHPBar("HPBar/hp_bar_background.png", "HPBar/enemy_boss_hp_bar.png", "HPBar/hp_bar_outline.png");
	// Set Position to the Right
	boss->setPosition(spawnPositionRight + heroOffset);
	// Add boss to unit manager
	boss->SetActive(false);
	this->addChild(boss, 2);
	unitManager->SetEnemyBoss(boss);
	bossSpawned = false;
	/********/
}