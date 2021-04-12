#include "StatScene.h"
#include "SimpleAudioEngine.h"
#include "IntroScene.h"
#include "SkillSelectionScene.h"
#include "AlliesSelectionScene.h"
#include "SkillsScene.h"
#include "StoreScene.h"
#include "..\GameObject\Units\Unit.h"
#include "..\GameObject\\Skills\Skill.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* StatScene::createScene()
{
    auto scene = Scene::create();
    auto layer = StatScene::create();
    scene->addChild(layer);

    return scene;
}

bool StatScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	fontSize = visibleSize.height * 0.075f;

	CheckSkillsSelectionCompleted();

	// Add number of luck here
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	// If no temp saved points
	int tempUnsavedPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_TempUnsavedPoints").c_str(), -1);
	if (tempUnsavedPoints == -1)
	{
		// Get saved unused points
		pointsLeft = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_UnsavedPoints").c_str(), 0);
		if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_AddNewPoints").c_str(), true))
		{
			pointsLeft += 5 + (def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", LUCK_PLUS_BUTTON)).c_str(), 0) / 3);

			def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_AddNewPoints").c_str(), false);
		}
		def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_UnsavedPoints").c_str(), pointsLeft);
	}
	else
	{
		// Get saved unused points
		pointsLeft = tempUnsavedPoints;
	}
	unsavedPointsLeft = pointsLeft;

	InitBackGround();
	InitGUI();

	// If First Stage
	if (def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_RICH) == Unit::BOSS_RICH)
	{
		InitFirstTimeTutorial();
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(StatScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/mp3/Music/Skill_stat.mp3", true);
	}
    return true;
}
void StatScene::InitBackGround()
{
	auto background = Sprite::create("Tabs/UI_ver2.png");
	background->setPosition(Point(	visibleSize.width * 0.5f + origin.x, 
									visibleSize.height * 0.5f + origin.y));
	background->setScale(	(visibleSize.width / background->getContentSize().width), 
							(visibleSize.height / background->getContentSize().height));
	this->addChild(background, 0);
	
	auto innerbackground = Sprite::create("StatScene/background.png");
	innerbackground->setPosition(Point(visibleSize.width * 0.5f + origin.x,
							visibleSize.height * 0.4f + origin.y));
	innerbackground->setScale(	(visibleSize.width * 0.87f / innerbackground->getContentSize().width),
								(visibleSize.height * 0.655f / innerbackground->getContentSize().height));
	this->addChild(innerbackground, 0);
}
void StatScene::InitGUI()
{
	// GUI
	auto gui = Node::create();
	gui->setTag(GUI);
	this->addChild(gui, 1);

	/* Menu */
	Vector<MenuItem*> MenuItems;
	Size buttonSize(visibleSize.width * 0.15f,
					visibleSize.height * 0.1f);
	// Play Button
	auto playButton = MenuItemImage::create("Buttons/nextbton.png", "Buttons/nextbtoff.png", "Buttons/nextbtoff.png",
												CC_CALLBACK_1(StatScene::ClickPlayButton, this));
	playButton->setTag(PLAY_BUTTON);
	playButton->setPosition(visibleSize.width * 0.5f - buttonSize.width * 0.5f,
							visibleSize.height * 0.275f);
	playButton->setScale(	buttonSize.width / playButton->getContentSize().width,
							buttonSize.height / playButton->getContentSize().height);
	// Lock play button
	playButton->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems.pushBack(playButton);

	// Mainmenu Button
	auto mainmenuButton = MenuItemImage::create("Buttons/backbton.png", "Buttons/backbtoff.png", "Buttons/backbtoff.png",
												CC_CALLBACK_1(StatScene::ClickMainMenuButton, this));
	mainmenuButton->setTag(MAINMENU_BUTTON);
	mainmenuButton->setPosition(-visibleSize.width * 0.5f + buttonSize.width * 0.5f,
								visibleSize.height * 0.275f);
	mainmenuButton->setScale(	buttonSize.width / mainmenuButton->getContentSize().width,
								buttonSize.height / mainmenuButton->getContentSize().height);
	mainmenuButton->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems.pushBack(mainmenuButton);

	// Init tabs
	InitTabs(&MenuItems);

	// Points Left
	auto pointsLeftLabel = Label::create(StringUtils::format("%d", pointsLeft), "MiniGameResources/CuteFont.ttf", fontSize);
	pointsLeftLabel->setTextColor(Color4B::WHITE);
	pointsLeftLabel->setPosition(	Vec2(visibleSize.width * 0.75f + origin.x, 
									visibleSize.height * 0.115f + origin.y));
	pointsLeftLabel->setTag(POINTS_LEFT_LABEL);
	pointsLeftLabel->setAnchorPoint(Point(0.0f, 0.5f));
	gui->addChild(pointsLeftLabel, 1);


	// Plus button position
	Vec2 midpoint(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y);
	std::vector<Vec2> statPositionList;
	// STR
	statPositionList.push_back(Vec2(-visibleSize.width * 0.15f, visibleSize.height * 0.01f));
	// HP
	statPositionList.push_back(Vec2(visibleSize.width * 0.225f, -visibleSize.height * 0.11f));
	// Def
	statPositionList.push_back(Vec2(-visibleSize.width * 0.15f, -visibleSize.height * 0.11f));
	// Recovery
	statPositionList.push_back(Vec2(-visibleSize.width * 0.15f, -visibleSize.height * 0.23f));
	// Luck
	statPositionList.push_back(Vec2(visibleSize.width * 0.225f, -visibleSize.height * 0.23f));
	// P.R.C
	statPositionList.push_back(Vec2(visibleSize.width * 0.225f, visibleSize.height * 0.01f));
	// Int
	statPositionList.push_back(Vec2(-visibleSize.width * 0.15f, -visibleSize.height * 0.35f));

	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = STR_PLUS_BUTTON; i < NUM_OF_PLUS_BUTTONS; i++)
	{
		stat[i - STR_PLUS_BUTTON] = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", i)).c_str(), 0);
		int tempStat = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_TempStat" + StringUtils::format("%d", i)).c_str(), -1);
		if (tempStat != -1)
		{
			stat[i - STR_PLUS_BUTTON] += tempStat;
			unsavedPointsLeft += tempStat;
		}
		// Plus Button
		MenuItemImage* plusButton = MenuItemImage::create(	"StatScene/plus_button.png", "StatScene/plus_button.png", "StatScene/plus_button_disabled.png",
															CC_CALLBACK_1(StatScene::ClickStrPlusButton, this));
		plusButton->setTag(i);
		plusButton->setPosition(statPositionList[i - STR_PLUS_BUTTON]);
		plusButton->setScale(	fontSize / plusButton->getContentSize().width, 
								fontSize / plusButton->getContentSize().height);
		switch (i)
		{
			case STR_PLUS_BUTTON:
			{
				plusButton->setCallback(CC_CALLBACK_1(StatScene::ClickStrPlusButton, this));
				break;
			}
			case HP_PLUS_BUTTON:
			{
				plusButton->setCallback(CC_CALLBACK_1(StatScene::ClickHpPlusButton, this));
				break;
			}
			case DEF_PLUS_BUTTON:
			{
				plusButton->setCallback(CC_CALLBACK_1(StatScene::ClickDefPlusButton, this));
				break;
			}
			case RECOVERY_PLUS_BUTTON:
			{
				plusButton->setCallback(CC_CALLBACK_1(StatScene::ClickRecoveryPlusButton, this));
				break;
			}
			case LUCK_PLUS_BUTTON:
			{
				plusButton->setCallback(CC_CALLBACK_1(StatScene::ClickLuckPlusButton, this));
				break;
			}
			case PRC_PLUS_BUTTON:
			{
				plusButton->setCallback(CC_CALLBACK_1(StatScene::ClickPrcPlusButton, this));
				break;
			}
			case INT_PLUS_BUTTON:
			{
				plusButton->setCallback(CC_CALLBACK_1(StatScene::ClickIntPlusButton, this));
				break;
			}
		}
		MenuItems.pushBack(plusButton);

		// Points Label
		auto points = Label::create(StringUtils::format("%d", stat[i - STR_PLUS_BUTTON]), "MiniGameResources/CuteFont.ttf", fontSize);
		points->setTextColor(Color4B::WHITE);
		points->setPosition(plusButton->getPosition() + midpoint +
							Vec2(plusButton->getContentSize().width * plusButton->getScaleX() * 0.75f, 0));
		points->setTag(i + NUM_OF_POINTS - NUM_OF_PLUS_BUTTONS);
		points->setAnchorPoint(Point(0.0f, 0.5f));
		gui->addChild(points, 1);
	}

	auto menu = Menu::createWithArray(MenuItems);
	menu->setTag(MENU);
	gui->addChild(menu, 1);

	CheckAndLockAllPlusButtons();
}
void StatScene::InitTabs(Vector<MenuItem*>* MenuItems)
{
	Size tabSize(visibleSize.width * 0.7f * 0.25f, visibleSize.height * 0.15f);
	// Stat Tab
	auto statTab = MenuItemImage::create("Tabs/push_stat_button.png", "Tabs/push_stat_button.png");
	statTab->setTag(STAT_SCENE_TAB);
	statTab->setPosition(	-tabSize.width * 1.5f,
							visibleSize.height * 0.255f);
	statTab->setScale(	tabSize.width / statTab->getContentSize().width,
						tabSize.height / statTab->getContentSize().height);
	statTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(statTab);
	// Store Tab
	auto storeTab = MenuItemImage::create("Tabs/store_button.png", "Tabs/push_store_button.png",
												CC_CALLBACK_1(StatScene::ClickStoreSceneTab, this));
	storeTab->setTag(STORE_SCENE_TAB);
	storeTab->setPosition(	-tabSize.width * 0.5f,
							visibleSize.height * 0.255f);
	storeTab->setScale(	tabSize.width / storeTab->getContentSize().width,
						tabSize.height / storeTab->getContentSize().height);
	storeTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(storeTab);
	
	// Skill Selection Tab
	auto skillsSelectionTab = MenuItemImage::create("Tabs/skill_button.png", "Tabs/push_skill_button.png",
												CC_CALLBACK_1(StatScene::ClickSkillSelectionSceneTab, this));
	skillsSelectionTab->setTag(SKILL_SELECTION_SCENE_TAB);
	skillsSelectionTab->setPosition(tabSize.width * 0.5f,
									visibleSize.height * 0.255f);
	skillsSelectionTab->setScale(	tabSize.width / skillsSelectionTab->getContentSize().width,
									tabSize.height / skillsSelectionTab->getContentSize().height);
	skillsSelectionTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(skillsSelectionTab);
	
	// Allies Selection Tab
	auto alliesSelectionTab = MenuItemImage::create("Tabs/colleague_button.png", "Tabs/push_colleague_button.png",
												CC_CALLBACK_1(StatScene::ClickAlliesSelectionSceneTab, this));
	alliesSelectionTab->setTag(ALLIES_SELECTION_SCENE_TAB);
	alliesSelectionTab->setPosition(tabSize.width * 1.5f,
									visibleSize.height * 0.255f);
	alliesSelectionTab->setScale(tabSize.width / alliesSelectionTab->getContentSize().width,
									tabSize.height / alliesSelectionTab->getContentSize().height);
	alliesSelectionTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(alliesSelectionTab);
}
void StatScene::InitFirstTimeTutorial()
{
	// First Time Tutorial
	auto firstTimeTutorial = MenuItemImage::create("StatScene/tutorial.png", "StatScene/tutorial.png",
											CC_CALLBACK_1(StatScene::ClickFirstTime, this));
	firstTimeTutorial->setScale((visibleSize.width / firstTimeTutorial->getContentSize().width),
								(visibleSize.height / firstTimeTutorial->getContentSize().height));
	auto firstTimeTutorialMenu = Menu::create(firstTimeTutorial, NULL);
	firstTimeTutorialMenu->setTag(FIRST_TIME_TUTORIAL);
	this->addChild(firstTimeTutorialMenu, 100);
}
void StatScene::IncreasePoint(int plusButtonTag)
{
	// Increse this stat points
	Label* points = (Label*)this->getChildByTag(GUI)->getChildByTag(plusButtonTag + NUM_OF_POINTS - NUM_OF_PLUS_BUTTONS);
	points->setString(StringUtils::format("%d", ++stat[plusButtonTag - STR_PLUS_BUTTON]));
	// Reduce points left
	Label* pointsLeftLabel = (Label*)this->getChildByTag(GUI)->getChildByTag(POINTS_LEFT_LABEL);
	pointsLeftLabel->setString(StringUtils::format("%d", --pointsLeft));
	CheckAndLockAllPlusButtons();
}
void StatScene::CheckAndLockAllPlusButtons()
{
	if (pointsLeft == 0)
	{
		// Lock all of the plus_buttons
		for (int i = STR_PLUS_BUTTON; i < NUM_OF_PLUS_BUTTONS; i++)
		{
			MenuItemImage* plusButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(MENU)->getChildByTag(i);
			if (!plusButton->isEnabled())
				return;
			plusButton->setEnabled(false);
		}
	}
	CheckAndUnLockPlayButton();
}

void StatScene::CheckAndUnLockPlayButton()
{
	// Unlock play button if locked
	MenuItemImage* playButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(MENU)->getChildByTag(PLAY_BUTTON);
	if (skillsSelectionSceneCompleted && pointsLeft == 0)
	{
		if (!playButton->isEnabled())
			playButton->setEnabled(true);
	}
	else
	{
		playButton->setEnabled(false);
	}
}
void StatScene::CheckSkillsSelectionCompleted()
{
	int numOfSkillsEquipped = 0;
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = 0; i < Skill::NUM_OF_TYPES; i++)
	{
		if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_SkillSelected" + StringUtils::format("%d", i)).c_str(), false))
			numOfSkillsEquipped++;
	}
	if (numOfSkillsEquipped == 8)
	{
		skillsSelectionSceneCompleted = true;
	}
	else
	{
		skillsSelectionSceneCompleted = false;
	}
}
bool StatScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	Point location = touch->getLocation();
	return false;
}

void StatScene::ClickFirstTime(Ref* pSender)
{
	auto firstTimeTutorial = (Menu*)this->getChildByTag(FIRST_TIME_TUTORIAL);
	firstTimeTutorial->setVisible(false);
}

void StatScene::SaveStatPoints()
{
	// Save points
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = STR_PLUS_BUTTON; i < NUM_OF_PLUS_BUTTONS; i++)
	{
		std::string keyName = "Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", i);
		std::string tempKeyName = "Data" + StringUtils::format("%d", currentData) + "_TempStat" + StringUtils::format("%d", i);
		def->setIntegerForKey(keyName.c_str(), def->getIntegerForKey(tempKeyName.c_str(), 0) + def->getIntegerForKey(keyName.c_str(), 0));
	}
	def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_UnsavedPoints").c_str(), 0);
}

void StatScene::ClickPlayButton(Ref* pSender)
{
	TempSaveStatPoints();
	SaveStatPoints();

	DeleteTempSaveStatPoints(false);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SkillsScene::createScene()));
}

void StatScene::ClickMainMenuButton(Ref* pSender)
{
	TempSaveStatPoints();
	DeleteTempSaveStatPoints();

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, IntroScene::createScene()));
}


void StatScene::ClickStrPlusButton(Ref* pSender)
{
	IncreasePoint(STR_PLUS_BUTTON);
}
void StatScene::ClickHpPlusButton(Ref* pSender)
{
	IncreasePoint(HP_PLUS_BUTTON);
}
void StatScene::ClickDefPlusButton(Ref* pSender)
{
	IncreasePoint(DEF_PLUS_BUTTON);
}
void StatScene::ClickRecoveryPlusButton(Ref* pSender)
{
	IncreasePoint(RECOVERY_PLUS_BUTTON);
}
void StatScene::ClickLuckPlusButton(Ref* pSender)
{
	IncreasePoint(LUCK_PLUS_BUTTON);
}
void StatScene::ClickPrcPlusButton(Ref* pSender)
{
	IncreasePoint(PRC_PLUS_BUTTON);
}
void StatScene::ClickIntPlusButton(Ref* pSender)
{
	IncreasePoint(INT_PLUS_BUTTON);
}


void StatScene::DeleteTempSaveStatPoints(bool saveUnsavedpoints)
{
	// Save unsaved points
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	if (saveUnsavedpoints)
	{
		int pointsUnsaved = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_TempUnsavedPoints").c_str(), 0);
		for (int i = STR_PLUS_BUTTON; i < NUM_OF_PLUS_BUTTONS; i++)
		{
			std::string keyName = "Data" + StringUtils::format("%d", currentData) + "_TempStat" + StringUtils::format("%d", i);
			pointsUnsaved += def->getIntegerForKey(keyName.c_str(), 0);
		}
		def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_UnsavedPoints").c_str(), pointsUnsaved);
	}
	// Delete Temp Save points
	for (int i = STR_PLUS_BUTTON; i < NUM_OF_PLUS_BUTTONS; i++)
	{
		std::string keyName = "Data" + StringUtils::format("%d", currentData) + "_TempStat" + StringUtils::format("%d", i);
		def->deleteValueForKey(keyName.c_str());
	}
	def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_TempUnsavedPoints").c_str());
}
void StatScene::TempSaveStatPoints()
{
	// Temp Save points
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = STR_PLUS_BUTTON; i < NUM_OF_PLUS_BUTTONS; i++)
	{
		std::string keyName = "Data" + StringUtils::format("%d", currentData) + "_TempStat" + StringUtils::format("%d", i);
		std::string keyName2 = "Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", i);
		def->setIntegerForKey(keyName.c_str(), stat[i - STR_PLUS_BUTTON] - def->getIntegerForKey(keyName2.c_str(), 0));
	}
	def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_TempUnsavedPoints").c_str(), pointsLeft);
}

void StatScene::ClickStoreSceneTab(Ref* pSender)
{
	TempSaveStatPoints();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(StoreScene::createScene());
}
void StatScene::ClickSkillSelectionSceneTab(Ref* pSender)
{
	TempSaveStatPoints();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(SkillSelectionScene::createScene());
}
void StatScene::ClickAlliesSelectionSceneTab(Ref* pSender)
{
	TempSaveStatPoints();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(AlliesSelectionScene::createScene());
}