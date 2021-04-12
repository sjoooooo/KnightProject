#include "AlliesSelectionScene.h"
#include "SkillSelectionScene.h"
#include "SkillsScene.h"
#include "IntroScene.h"
#include "StatScene.h"
#include "StoreScene.h"
#include "SimpleAudioEngine.h"
#include "..\GameObject\\Skills\Skill.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* AlliesSelectionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = AlliesSelectionScene::create();
    scene->addChild(layer);

    return scene;
}

bool AlliesSelectionScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin      = Director::getInstance()->getVisibleOrigin();

	InitBackGround();
	InitAllies();
	InitMenu();

	
	mpData = cocos2d::Vector<cocos2d::Sprite*>(100);
	
	this->schedule(schedule_selector(AlliesSelectionScene::update));
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(AlliesSelectionScene::onTouchBegan, this); 
	listener->onTouchEnded = CC_CALLBACK_2(AlliesSelectionScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

    return true;
}

void AlliesSelectionScene::update(float ft)
{

}
	
void AlliesSelectionScene::InitBackGround()
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
	outerOffset  *= 0.5f;

	auto innerbackground = Sprite::create("Background/HalfAlphaBlack.png");
	innerbackground->setPosition(innerMidpointOffset + midPoint + origin);
	innerbackground->setScale(	(innerVisibleSize.width / innerbackground->getContentSize().width),
								(innerVisibleSize.height / innerbackground->getContentSize().height));
	this->addChild(innerbackground, 0);
}

void AlliesSelectionScene::InitAllies()
{
	currentAlly = NULL;

	int maxShown = 3;
	numOfPages = 2;
	currentPage = 1;
	int currentIndex = 0;
	for (int i = 0; i < numOfPages; i++)
	{
		for (int j = 0; j < maxShown; j++)
		{
			Sprite* ally = Sprite::create("CharacterAnimations/stop_sd.png");
			ally->setTag(ALLY + currentIndex);
			ally->setPosition(	Point(visibleSize.width * 0.35f + j * visibleSize.width * 0.15f + origin.x + i * visibleSize.width,
								visibleSize.height * 0.5f + origin.y) + innerMidpointOffset);
			ally->setScale(	visibleSize.width * 0.1f / ally->getContentSize().width,
							visibleSize.height * 0.3f / ally->getContentSize().height);
			this->addChild(ally, 1);

			if (++currentIndex >= totalNumOfAllies)
			{
				break;
			}
		}
		if (currentIndex >= totalNumOfAllies)
		{
			break;
		}
	}

	numOfEquippedAllies = 0;
	numOfAvailableAllies = 0;

	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = 0; i < totalNumOfAllies; i++)
	{
		Sprite* ally = (Sprite*)this->getChildByTag(ALLY + i);
		selectedType[i] = def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_AllySelected" + StringUtils::format("%d", i)).c_str(), false);
		selectable[i] = def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_AllySelectable" + StringUtils::format("%d", i)).c_str(), false);
		if (selectedType[i])
		{
			SetTexture(ally, CHOICE);
			numOfEquippedAllies++;
			numOfAvailableAllies++;
		}
		else
		{
			if (!selectable[i])
			{
				SetTexture(ally, LOCKED);
			}
			else
			{
				SetTexture(ally, UNLOCK);
				numOfAvailableAllies++;
			}
		}
	}
}

void AlliesSelectionScene::InitMenu()
{
	// GUI
	auto gui = Node::create();
	gui->setTag(GUI);
	this->addChild(gui, 3);

	// Inner menu
	Size buttonSize(innerVisibleSize.width * 0.1f, innerVisibleSize.height * 0.1f);

	Vector<MenuItem*> menuItemList;

	// Equip
	auto equipButton = MenuItemImage::create(	"AlliesSelection/Buttons/equip.png", "AlliesSelection/Buttons/push_equip.png", "AlliesSelection/Buttons/equip.png",
												CC_CALLBACK_1(AlliesSelectionScene::Equip, this));
	equipButton->setScale(	buttonSize.width / equipButton->getContentSize().width,
							buttonSize.height / equipButton->getContentSize().height);
	equipButton->setPosition(Point(-innerVisibleSize.width * 0.5f + buttonSize.width * 0.6f, -innerVisibleSize.height * 0.5f + buttonSize.height * 0.6f) + innerMidpointOffset);
	equipButton->setTag(EQUIP);
	equipButton->setEnabled(false);
	menuItemList.pushBack(equipButton);

	// Unequip
	auto unequipButton = MenuItemImage::create("AlliesSelection/Buttons/equip_n.png", "AlliesSelection/Buttons/push_equip_n.png", "AlliesSelection/Buttons/equip_n.png",
												CC_CALLBACK_1(AlliesSelectionScene::Unequip, this));
	unequipButton->setScale(buttonSize.width / equipButton->getContentSize().width,
							buttonSize.height / equipButton->getContentSize().height);
	unequipButton->setPosition(Point(-innerVisibleSize.width * 0.5f + buttonSize.width * 1.7f, -innerVisibleSize.height * 0.5f + buttonSize.height * 0.6f) + innerMidpointOffset);
	unequipButton->setTag(UNEQUIP);
	unequipButton->setEnabled(false);
	menuItemList.pushBack(unequipButton);
	
	// Next Set
	auto nextSetButton = MenuItemImage::create("AlliesSelection/right.png", "AlliesSelection/push_right.png",
												CC_CALLBACK_1(AlliesSelectionScene::NextSet, this));
	nextSetButton->setScale(buttonSize.width / nextSetButton->getContentSize().width,
							buttonSize.height / nextSetButton->getContentSize().height);
	nextSetButton->setPosition(Point(innerVisibleSize.width * 0.5f - buttonSize.width * 0.6f, 0.0f) + innerMidpointOffset);
	nextSetButton->setTag(NEXT_SET);
	menuItemList.pushBack(nextSetButton);

	// Prev Set
	auto prevSetButton = MenuItemImage::create("AlliesSelection/left.png", "AlliesSelection/push_left.png",
												CC_CALLBACK_1(AlliesSelectionScene::PrevSet, this));
	prevSetButton->setScale(buttonSize.width / prevSetButton->getContentSize().width,
							buttonSize.height / prevSetButton->getContentSize().height);
	prevSetButton->setPosition(Point(-innerVisibleSize.width * 0.5f + buttonSize.width * 0.6f, 0.0f) + innerMidpointOffset);
	prevSetButton->setTag(PREV_SET);
	menuItemList.pushBack(prevSetButton);

	auto inner_menu = Menu::createWithArray(menuItemList);
	inner_menu->setTag(INNER_MENU);
	gui->addChild(inner_menu, 2);

	// Outer menu
	Vector<MenuItem*> MenuItems;
	Size PlayAndMainMenuButtonSize(	visibleSize.width * 0.15f,
									visibleSize.height * 0.1f);
	// Play Button
	auto playButton = MenuItemImage::create("Buttons/nextbton.png", "Buttons/nextbtoff.png", "Buttons/nextbtoff.png",
												CC_CALLBACK_1(AlliesSelectionScene::ClickPlayButton, this));
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
												CC_CALLBACK_1(AlliesSelectionScene::ClickMainMenuButton, this));
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


	// Labels
	float labelSize = visibleSize.height * 0.05f;
	auto label = Label::createWithTTF("ALLIES:" + StringUtils::format("%d/3", numOfEquippedAllies), "CocosResources/CuteFont.ttf", labelSize);
	label->setPosition(Vec2(origin.x + buttonSize.width * 2.3f, origin.y + buttonSize.height * 0.6f) + innerMidpointOffset + outerOffset);
	label->setTag(NUM_OF_EQUIPPED_LABEL);
	label->setAnchorPoint(Point(0.0f, 0.5f));
	gui->addChild(label, 3);


	CheckAndUnLockPlayButton();
}
void AlliesSelectionScene::InitTabs(Vector<MenuItem*>* MenuItems)
{
	Size tabSize(visibleSize.width * 0.7f * 0.25f, visibleSize.height * 0.15f);
	// Stat Tab
	auto statTab = MenuItemImage::create(	"Tabs/stat_button.png", "Tabs/push_stat_button.png",
											CC_CALLBACK_1(AlliesSelectionScene::ClickStatSceneSceneTab, this));
	statTab->setTag(STAT_SCENE_TAB);
	statTab->setPosition(	-tabSize.width * 1.5f,
							visibleSize.height * 0.255f);
	statTab->setScale(	tabSize.width / statTab->getContentSize().width,
						tabSize.height / statTab->getContentSize().height);
	statTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(statTab);
	// Store Tab
	auto storeTab = MenuItemImage::create(	"Tabs/store_button.png", "Tabs/push_store_button.png",
											CC_CALLBACK_1(AlliesSelectionScene::ClickStoreSceneTab, this));
	storeTab->setTag(STORE_SCENE_TAB);
	storeTab->setPosition(	-tabSize.width * 0.5f,
							visibleSize.height * 0.255f);
	storeTab->setScale(	tabSize.width / storeTab->getContentSize().width,
						tabSize.height / storeTab->getContentSize().height);
	storeTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(storeTab);
	
	// Skill Selection Tab
	auto skillsSelectionTab = MenuItemImage::create("Tabs/skill_button.png", "Tabs/push_skill_button.png",
													CC_CALLBACK_1(AlliesSelectionScene::ClickSkillSelectionSceneTab, this));
	skillsSelectionTab->setTag(SKILL_SELECTION_SCENE_TAB);
	skillsSelectionTab->setPosition(tabSize.width * 0.5f,
									visibleSize.height * 0.255f);
	skillsSelectionTab->setScale(	tabSize.width / skillsSelectionTab->getContentSize().width,
									tabSize.height / skillsSelectionTab->getContentSize().height);
	skillsSelectionTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(skillsSelectionTab);
	
	// Allies Selection Tab
	auto alliesSelectionTab = MenuItemImage::create("Tabs/push_colleague_button.png", "Tabs/push_colleague_button.png");
	alliesSelectionTab->setTag(ALLIES_SELECTION_SCENE_TAB);
	alliesSelectionTab->setPosition(tabSize.width * 1.5f,
									visibleSize.height * 0.255f);
	alliesSelectionTab->setScale(tabSize.width / alliesSelectionTab->getContentSize().width,
									tabSize.height / alliesSelectionTab->getContentSize().height);
	alliesSelectionTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(alliesSelectionTab);
}

bool AlliesSelectionScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	//마우스가 누른 위치 받기
	Point location;
	location = touch->getLocation();

	UnselectCurrentAlly();

	for (int i = ALLY; i < ALLY + totalNumOfAllies; i++)
	{
		if (selectable[i - ALLY])
		{
			Sprite* ally = (Sprite*) this->getChildByTag(i);
			if (ally->getBoundingBox().containsPoint(location))
			{
				if (selectedType[ally->getTag() - ALLY])
				{
					SetTexture(ally, SELECT);
					if (numOfEquippedAllies > 0)
					{
						MenuItemImage* unequipButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(INNER_MENU)->getChildByTag(UNEQUIP);
						unequipButton->setEnabled(true);
					}
				}
				else
				{
					SetTexture(ally, SELECT);
					if (numOfEquippedAllies < maxNumOfSelectedAllies && numOfEquippedAllies < numOfAvailableAllies)
					{
						MenuItemImage* equipButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(INNER_MENU)->getChildByTag(EQUIP);
						equipButton->setEnabled(true);
					}
				}
				currentAlly = ally;
				break;

			}
		}
	}


	return true;
}

void AlliesSelectionScene::onTouchEnded(Touch* touch, Event* unused_event)
{
	//마우스가 누른 위치 받기
	Point location;
	location = touch->getLocation();
}

void AlliesSelectionScene::CheckAndUnLockPlayButton()
{
	CheckSkillsSelectionCompleted();
	CheckStatCompleted();
	// Unlock play button if locked
	MenuItemImage* playButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(MENU)->getChildByTag(PLAY_BUTTON);
	if (skillsSelectionSceneCompleted && statSceneCompleted)
	{
		if (!playButton->isEnabled())
			playButton->setEnabled(true);
	}
	else
	{
		playButton->setEnabled(false);
	}
}
void AlliesSelectionScene::CheckSkillsSelectionCompleted()
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
void AlliesSelectionScene::CheckStatCompleted()
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

void AlliesSelectionScene::SaveData()
{
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = 0; i < totalNumOfAllies; i++)
	{
		def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_AllySelected" + StringUtils::format("%d", i)).c_str(), selectedType[i]);
	}
}
void AlliesSelectionScene::UnselectCurrentAlly()
{
	if (currentAlly)
	{
		if (selectedType[currentAlly->getTag() - ALLY])
		{
			SetTexture(currentAlly, CHOICE);

			MenuItemImage* unequipButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(INNER_MENU)->getChildByTag(UNEQUIP);
			unequipButton->setEnabled(false);
		}
		else
		{
			SetTexture(currentAlly, UNLOCK);

			MenuItemImage* equipButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(INNER_MENU)->getChildByTag(EQUIP);
			equipButton->setEnabled(false);
		}
		currentAlly = NULL;
	}
}

void AlliesSelectionScene::Equip(Ref* pSender)
{
	if (currentAlly)
	{
		if (!selectedType[currentAlly->getTag() - ALLY])
		{
			SetTexture(currentAlly, CHOICE);
			selectedType[currentAlly->getTag() - ALLY] = true;
			// Labels
			auto label = (Label*)this->getChildByTag(GUI)->getChildByTag(NUM_OF_EQUIPPED_LABEL);
			label->setString("ALLIES:" + StringUtils::format("%d/3", ++numOfEquippedAllies));
			MenuItemImage* equipButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(INNER_MENU)->getChildByTag(EQUIP);
			equipButton->setEnabled(false);
		}
		currentAlly = NULL;
	}
}
void AlliesSelectionScene::Unequip(Ref* pSender)
{
	if (currentAlly)
	{
		if (selectedType[currentAlly->getTag() - ALLY])
		{
			SetTexture(currentAlly, UNLOCK);
			selectedType[currentAlly->getTag() - ALLY] = false;
			// Labels
			auto label = (Label*)this->getChildByTag(GUI)->getChildByTag(NUM_OF_EQUIPPED_LABEL);
			label->setString("ALLIES:" + StringUtils::format("%d/3", --numOfEquippedAllies));

			MenuItemImage* unequipButton = (MenuItemImage*)this->getChildByTag(GUI)->getChildByTag(INNER_MENU)->getChildByTag(UNEQUIP);
			unequipButton->setEnabled(false);
		}
		currentAlly = NULL;
	}
}

void AlliesSelectionScene::NextSet(Ref* pSender)
{
	UnselectCurrentAlly();

	if (currentPage == numOfPages)
	{
		for (int i = ALLY; i < ALLY + totalNumOfAllies; i++)
		{
			Sprite* ally = (Sprite*) this->getChildByTag(i);
			ally->runAction(MoveBy::create(0.5f, Vec2(visibleSize.width * (numOfPages - 1), 0)));
		}
		currentPage = 1;
	}
	else
	{
		for (int i = ALLY; i < ALLY + totalNumOfAllies; i++)
		{
			Sprite* ally = (Sprite*) this->getChildByTag(i);
			ally->runAction(MoveBy::create(0.5f, Vec2(-visibleSize.width, 0)));
		}
		++currentPage;
	}
}

void AlliesSelectionScene::PrevSet(Ref* pSender)
{
	UnselectCurrentAlly();

	if (currentPage == 1)
	{
		for (int i = ALLY; i < ALLY + totalNumOfAllies; i++)
		{
			Sprite* ally = (Sprite*) this->getChildByTag(i);
			ally->runAction(MoveBy::create(0.5f, Vec2(-visibleSize.width * (numOfPages - 1), 0)));
		}
		currentPage = numOfPages;
	}
	else
	{
		for (int i = ALLY; i < ALLY + totalNumOfAllies; i++)
		{
			Sprite* ally = (Sprite*) this->getChildByTag(i);
			ally->runAction(MoveBy::create(0.5f, Vec2(visibleSize.width, 0)));
		}
		--currentPage;
	}
}

void AlliesSelectionScene::ClickPlayButton(Ref* pSender)
{
	SaveData();
	StatScene::SaveStatPoints();

	StatScene::DeleteTempSaveStatPoints(false);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SkillsScene::createScene()));
}

void AlliesSelectionScene::ClickMainMenuButton(Ref* pSender)
{
	SaveData();
	StatScene::DeleteTempSaveStatPoints();

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, IntroScene::createScene()));
}

void AlliesSelectionScene::ClickStoreSceneTab(Ref* pSender)
{
	SaveData();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(StoreScene::createScene());
}
void AlliesSelectionScene::ClickSkillSelectionSceneTab(Ref* pSender)
{
	SaveData();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(SkillSelectionScene::createScene());
}
void AlliesSelectionScene::ClickStatSceneSceneTab(Ref* pSender)
{
	SaveData();
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(StatScene::createScene());
}

void AlliesSelectionScene::SetTexture(Sprite* spr, int state)
{
	switch (state)
	{
		case CHOICE:
		{
			spr->setTexture(("AlliesSelection/choice/choice-" + StringUtils::format("%d", spr->getTag() - ALLY + 1) + ".png").c_str());
			break;
		}
		case LOCKED:
		{
			spr->setTexture(("AlliesSelection/locked/locked-" + StringUtils::format("%d", spr->getTag() - ALLY + 1) + ".png").c_str());
			break;
		}
		case UNLOCK:
		{
			spr->setTexture(("AlliesSelection/unlock/unlock-" + StringUtils::format("%d", spr->getTag() - ALLY + 1) + ".png").c_str());
			break;
		}
		case SELECT:
		{
			spr->setTexture(("AlliesSelection/select/select-" + StringUtils::format("%d", spr->getTag() - ALLY + 1) + ".png").c_str());
			break;
		}
	}
}