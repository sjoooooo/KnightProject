#include "SkillSelectionScene.h"
#include "AlliesSelectionScene.h"
#include "SimpleAudioEngine.h"
#include "SkillsScene.h"
#include "IntroScene.h"
#include "StatScene.h"
#include "StoreScene.h"
#include "..\GameObject\\Skills\Skill.h"
#include "..\GameObject\Units\Unit.h"

using namespace CocosDenshion;
USING_NS_CC;

void SkillSelectionScene::InitMenu()
{
	// GUI
	auto gui = Node::create();
	gui->setTag(GUI);
	this->addChild(gui, 3);

	currentbutton = NULL;
	currentimage = NULL;

	int k = 0;
	for (int i = 0; i <5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Sprite* button = Sprite::create("SkillSelection/skill/skill"+StringUtils::format("%d", k)+".png");
			button->setTag(BUTTON + k);
			button->setPosition(Point(innerVisibleSize.width*0.075f + i * innerVisibleSize.width*0.12f, innerVisibleSize.height * 0.8f + j*(-innerVisibleSize.height * 0.2f)) + innerMidpointOffset + outerOffset);
			button->setScale(	innerVisibleSize.width * 0.1f / button->getContentSize().width,
								innerVisibleSize.height * 0.1f / button->getContentSize().height);
			this->addChild(button, 2);
			
           Sprite* image = Sprite::create("SkillSelection/skillexplain/skill_explain"+StringUtils::format("%d", k)+".png");
		   image->setTag(BUTTON + k + TOTAL_NUM_OF_SKILLS);
		   image->setScale(	innerVisibleSize.width * 0.365f / image->getContentSize().width,
							innerVisibleSize.height / image->getContentSize().height);
		   image->setPosition(Point(origin.x + innerVisibleSize.width*0.819f, origin.y + innerVisibleSize.height*0.5f) + innerMidpointOffset + outerOffset);
		   image->setVisible(false);
		   this->addChild(image, 2);

			k++;
		}
	}

	numOfEquipedskills = 0;

	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = 0; i < TOTAL_NUM_OF_SKILLS; i++)
	{
		selected[i] = def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_SkillSelected" + StringUtils::format("%d", i)).c_str(), false);
		if (selected[i])
		{
			Sprite* button = (Sprite*)this->getChildByTag(BUTTON + i);
			button->setTexture("SkillSelection/skill/equip" + StringUtils::format("%d", button->getTag() - BUTTON) + ".png");
			numOfEquipedskills++;
		}
	}

	Sprite*equipbutton = Sprite::create("SkillSelection/skillequipbutton/skillequip.png");
	equipbutton->setTag(EQUIP_BUTTON);
	equipbutton->setPosition(Point(origin.x + innerVisibleSize.width*0.1f, origin.y + innerVisibleSize.height*0.1f) + innerMidpointOffset + outerOffset);
	equipbutton->setScale(0.5);
	this->addChild(equipbutton, 2);

	Sprite*clearbutton = Sprite::create("SkillSelection/skillequipbutton/skillnotequip.png");
	clearbutton->setTag(CLEAR_BUTTON);
	clearbutton->setScale(0.5);
	clearbutton->setPosition(Point(origin.x + innerVisibleSize.width*0.3f, origin.y + innerVisibleSize.height*0.1f) + innerMidpointOffset + outerOffset);
	this->addChild(clearbutton, 2);

	// Outer menu
	Vector<MenuItem*> MenuItems;
	Size PlayAndMainMenuButtonSize(	visibleSize.width * 0.15f,
									visibleSize.height * 0.1f);
	// Play Button
	auto playButton = MenuItemImage::create("Buttons/nextbton.png", "Buttons/nextbtoff.png", "Buttons/nextbtoff.png",
											CC_CALLBACK_1(SkillSelectionScene::ClickPlayButton, this));
	playButton->setTag(PLAY_BUTTON);
	playButton->setPosition(visibleSize.width * 0.5f - PlayAndMainMenuButtonSize.width * 0.5f,
							visibleSize.height * 0.275f);
	playButton->setScale(	PlayAndMainMenuButtonSize.width / playButton->getContentSize().width,
							PlayAndMainMenuButtonSize.height / playButton->getContentSize().height);
	// Lock play button
	playButton->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems.pushBack(playButton);

	// Mainmenu Button
	auto mainmenuButton = MenuItemImage::create("Buttons/backbton.png", "Buttons/backbtoff.png", "Buttons/backbtoff.png",
												CC_CALLBACK_1(SkillSelectionScene::ClickMainMenuButton, this));
	mainmenuButton->setTag(MAINMENU_BUTTON);
	mainmenuButton->setPosition(-visibleSize.width * 0.5f + PlayAndMainMenuButtonSize.width * 0.5f,
								visibleSize.height * 0.275f);
	mainmenuButton->setScale(	PlayAndMainMenuButtonSize.width / mainmenuButton->getContentSize().width,
								PlayAndMainMenuButtonSize.height / mainmenuButton->getContentSize().height);
	mainmenuButton->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems.pushBack(mainmenuButton);

	InitTabs(&MenuItems);

	auto menu = Menu::createWithArray(MenuItems);
	menu->setTag(MENU);
	gui->addChild(menu, 2);

	CheckAndUnLockPlayButton();
}
Scene* SkillSelectionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SkillSelectionScene::create();
    scene->addChild(layer);

    return scene;
}

bool SkillSelectionScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin      = Director::getInstance()->getVisibleOrigin();

	InitBackGround();
	InitMenu();
	InitLabel();

	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	// If First Stage
	if (def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_RICH) == Unit::BOSS_RICH)
	{
		InitFirstTimeTutorial();
	}
	
	mpData = cocos2d::Vector<cocos2d::Sprite*>(100);

	this->schedule(schedule_selector(SkillSelectionScene::update));
	
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(SkillSelectionScene::onTouchBegan, this); 
	listener->onTouchEnded = CC_CALLBACK_2(SkillSelectionScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

    return true;
}

void SkillSelectionScene::update(float ft)
{

}
	
void SkillSelectionScene::InitBackGround()
{
	auto background = Sprite::create("Tabs/UI_ver2.png");
	background->setPosition(Point(	visibleSize.width * 0.5f + origin.x, 
									visibleSize.height * 0.5f + origin.y));
	background->setScale(	(visibleSize.width / background->getContentSize().width), 
							(visibleSize.height / background->getContentSize().height));
	this->addChild(background, 2);

	innerVisibleSize.setSize(visibleSize.width * 0.87f, visibleSize.height * 0.655f);
	innerMidpointOffset.set(0.0f, -visibleSize.height * 0.1f);
	midPoint.set(visibleSize.width * 0.5f, visibleSize.height * 0.5f);
	outerOffset.set(visibleSize.width - innerVisibleSize.width, visibleSize.height - innerVisibleSize.height);
	outerOffset *= 0.5f;

	auto innerbackground = Sprite::create("SkillSelection/background.png");
	innerbackground->setPosition(innerMidpointOffset + midPoint + origin);
	innerbackground->setScale(	(innerVisibleSize.width / innerbackground->getContentSize().width),
								(innerVisibleSize.height / innerbackground->getContentSize().height));
	this->addChild(innerbackground, 0);	
}
void SkillSelectionScene::InitFirstTimeTutorial()
{
	// First Time Tutorial
	auto firstTimeTutorial = MenuItemImage::create("SkillSelection/tutorial.png", "SkillSelection/tutorial.png",
											CC_CALLBACK_1(SkillSelectionScene::ClickFirstTime, this));
	firstTimeTutorial->setScale((visibleSize.width / firstTimeTutorial->getContentSize().width),
								(visibleSize.height / firstTimeTutorial->getContentSize().height));
	auto firstTimeTutorialMenu = Menu::create(firstTimeTutorial, NULL);
	firstTimeTutorialMenu->setTag(FIRST_TIME_TUTORIAL);
	this->addChild(firstTimeTutorialMenu, FIRST_TIME_TUTORIAL);
}

void SkillSelectionScene::InitLabel()
{
	auto label = Label::createWithTTF("Equip number", "CocosResources/CuteFont.ttf", 26);
	label->setPosition(Vec2(origin.x + visibleSize.width*0.55f, origin.y + visibleSize.height*0.18f));
	this->addChild(label, 1);
	
	Label* label2 = Label::createWithTTF(StringUtils::format("%d/8", numOfEquipedskills), "MiniGameResources/CuteFont.ttf", 40);
	label2->setPosition(Vec2(origin.x + visibleSize.width*0.55f, origin.y + visibleSize.height*0.14f));
	label2->setColor(Color3B(255,255,255));
	label2->setTag(NUM_OF_EQUIPPED);	
	this->addChild(label2, 2);
}

void SkillSelectionScene::InitTabs(Vector<MenuItem*>* MenuItems)
{
	Size tabSize(visibleSize.width * 0.7f * 0.25f, visibleSize.height * 0.15f);
	// Stat Tab
	auto statTab = MenuItemImage::create(	"Tabs/stat_button.png", "Tabs/push_stat_button.png",
											CC_CALLBACK_1(SkillSelectionScene::ClickStatSceneSceneTab, this));
	statTab->setTag(STAT_SCENE_TAB);
	statTab->setPosition(	-tabSize.width * 1.5f,
							visibleSize.height * 0.255f);
	statTab->setScale(	tabSize.width / statTab->getContentSize().width,
						tabSize.height / statTab->getContentSize().height);
	statTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(statTab);
	// Store Tab
	auto storeTab = MenuItemImage::create(	"Tabs/store_button.png", "Tabs/push_store_button.png",
											CC_CALLBACK_1(SkillSelectionScene::ClickStoreSceneTab, this));
	storeTab->setTag(STORE_SCENE_TAB);
	storeTab->setPosition(	-tabSize.width * 0.5f,
							visibleSize.height * 0.255f);
	storeTab->setScale(	tabSize.width / storeTab->getContentSize().width,
						tabSize.height / storeTab->getContentSize().height);
	storeTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(storeTab);
	
	// Skill Selection Tab
	auto skillsSelectionTab = MenuItemImage::create("Tabs/push_skill_button.png", "Tabs/push_skill_button.png");
	skillsSelectionTab->setTag(SKILL_SELECTION_SCENE_TAB);
	skillsSelectionTab->setPosition(tabSize.width * 0.5f,
									visibleSize.height * 0.255f);
	skillsSelectionTab->setScale(	tabSize.width / skillsSelectionTab->getContentSize().width,
									tabSize.height / skillsSelectionTab->getContentSize().height);
	skillsSelectionTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(skillsSelectionTab);
	
	// Allies Selection Tab
	auto alliesSelectionTab = MenuItemImage::create("Tabs/colleague_button.png", "Tabs/push_colleague_button.png",
													CC_CALLBACK_1(SkillSelectionScene::ClickAlliesSelectionSceneTab, this));
	alliesSelectionTab->setTag(ALLIES_SELECTION_SCENE_TAB);
	alliesSelectionTab->setPosition(tabSize.width * 1.5f,
									visibleSize.height * 0.255f);
	alliesSelectionTab->setScale(tabSize.width / alliesSelectionTab->getContentSize().width,
									tabSize.height / alliesSelectionTab->getContentSize().height);
	alliesSelectionTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(alliesSelectionTab);
}

void SkillSelectionScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
}

void SkillSelectionScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	
}

bool SkillSelectionScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	//마우스가 누른 위치 받기
	Point location;
	location = touch->getLocation();

	if(((Sprite*)this->getChildByTag(CLEAR_BUTTON))->getBoundingBox().containsPoint(location))
	{
		if (currentbutton)
		{
			if (selected[currentbutton->getTag() - BUTTON] && numOfEquipedskills > 0)
			{
				Label* label1 = ((Label*) this->getChildByTag(NUM_OF_EQUIPPED));
				label1->setString(StringUtils::format("%d/8", --numOfEquipedskills));

				currentbutton->setTexture("SkillSelection/skill/skill" + StringUtils::format("%d", currentbutton->getTag() - BUTTON) + ".png");
				selected[currentbutton->getTag() - BUTTON] = false;
				currentbutton = NULL;
			}
		}
	}

	if (((Sprite*) this->getChildByTag(EQUIP_BUTTON))->getBoundingBox().containsPoint(location))
	{
		if (currentbutton)
		{
			if (!selected[currentbutton->getTag() - BUTTON] && numOfEquipedskills < 8)
			{
				Label* label = ((Label*) this->getChildByTag(NUM_OF_EQUIPPED));
				label->setString(StringUtils::format("%d/8", ++numOfEquipedskills));

				currentbutton->setTexture("SkillSelection/skill/equip" + StringUtils::format("%d", currentbutton->getTag() - BUTTON) + ".png");
				selected[currentbutton->getTag() - BUTTON] = true;
				currentbutton = NULL;
				CheckAndUnLockPlayButton();
			}
		}
	}
	else
	{
		if (currentbutton)
		{
			if (!selected[currentbutton->getTag() - BUTTON])
			{
				currentbutton->setTexture("SkillSelection/skill/skill" + StringUtils::format("%d", currentbutton->getTag() - BUTTON) + ".png");
			}
			else
			{
				currentbutton->setTexture("SkillSelection/skill/equip" + StringUtils::format("%d", currentbutton->getTag() - BUTTON) + ".png");
			}
			currentbutton = NULL;
		}

		if (currentimage)
		{
			currentimage->setVisible(false);
			currentimage = NULL;
		}

		for (int i = BUTTON; i < BUTTON + TOTAL_NUM_OF_SKILLS; i++)
		{
			Sprite* button = (Sprite*) this->getChildByTag(i);
			if (button->getBoundingBox().containsPoint(location))
			{
				if (!selected[button->getTag() - BUTTON])
				{
					button->setTexture("SkillSelection/skill/choice" + StringUtils::format("%d", i - BUTTON) + ".png");
				}
				else
				{
					button->setTexture("SkillSelection/skill/equip_choice" + StringUtils::format("%d", i - BUTTON) + ".png");
				}
				currentbutton = button;
				Sprite* image = (Sprite*) this->getChildByTag(i + TOTAL_NUM_OF_SKILLS);
				image->setVisible(true);
				currentimage = image;
				break;

			}
		}
	}


	return true;
}

void SkillSelectionScene::onTouchEnded(Touch* touch, Event* unused_event)
{
	//마우스가 누른 위치 받기
	Point location;
	location = touch->getLocation();
}

void SkillSelectionScene::SaveData()
{
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = 0; i < TOTAL_NUM_OF_SKILLS; i++)
	{
		def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_SkillSelected" + StringUtils::format("%d", i)).c_str(), selected[i]);
	}
}

void SkillSelectionScene::CheckAndUnLockPlayButton()
{
	CheckStatCompleted();
	// Unlock play button if locked
	MenuItemImage* playButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(MENU)->getChildByTag(PLAY_BUTTON);
	if (numOfEquipedskills == 8 && statSceneCompleted)
	{
		if (!playButton->isEnabled())
			playButton->setEnabled(true);
	}
	else
	{
		playButton->setEnabled(false);
	}
}
void SkillSelectionScene::CheckStatCompleted()
{
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	if (def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_UnsavedPoints").c_str(), 0) == 0 ||
		def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_TempUnsavedPoints").c_str(), 0) == 0)
	{
		statSceneCompleted = true;
	}
	else
	{
		statSceneCompleted = false;
	}
}


void SkillSelectionScene::ClickFirstTime(Ref* pSender)
{
	auto firstTimeTutorial = (Menu*)this->getChildByTag(FIRST_TIME_TUTORIAL);
	firstTimeTutorial->setVisible(false);
}

void SkillSelectionScene::ClickPlayButton(Ref* pSender)
{
	SaveData();
	StatScene::SaveStatPoints();

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	StatScene::DeleteTempSaveStatPoints(false);

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SkillsScene::createScene()));
}

void SkillSelectionScene::ClickMainMenuButton(Ref* pSender)
{
	StatScene::DeleteTempSaveStatPoints();

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, IntroScene::createScene()));
}

void SkillSelectionScene::ClickStoreSceneTab(Ref* pSender)
{
	SaveData();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(StoreScene::createScene());
}
void SkillSelectionScene::ClickAlliesSelectionSceneTab(Ref* pSender)
{
	SaveData();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(AlliesSelectionScene::createScene());
}
void SkillSelectionScene::ClickStatSceneSceneTab(Ref* pSender)
{
	SaveData();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(StatScene::createScene());
}