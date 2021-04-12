#include "StoreScene.h"
#include "SkillSelectionScene.h"
#include "SkillsScene.h"
#include "IntroScene.h"
#include "StatScene.h"
#include "AlliesSelectionScene.h"
#include "SimpleAudioEngine.h"
#include "..\GameObject\\Skills\Skill.h"
#include "..\GameObject\Item\Item.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* StoreScene::createScene()
{
    auto scene = Scene::create();
    auto layer = StoreScene::create();
    scene->addChild(layer);

    return scene;
}

bool StoreScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin      = Director::getInstance()->getVisibleOrigin();

	InitBackGround();
	InitMenu();

	
	mpData = cocos2d::Vector<cocos2d::Sprite*>(100);

	
	this->schedule(schedule_selector(StoreScene::update));
	
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(StoreScene::onTouchBegan, this); 
	listener->onTouchEnded = CC_CALLBACK_2(StoreScene::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

    return true;
}

void StoreScene::update(float ft)
{

}
	
void StoreScene::InitBackGround()
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

	auto innerbackground = Sprite::create("ShopScene/shop_background.png");
	innerbackground->setPosition(innerMidpointOffset + midPoint + origin);
	innerbackground->setScale(	(innerVisibleSize.width / innerbackground->getContentSize().width),
								(innerVisibleSize.height / innerbackground->getContentSize().height));
	this->addChild(innerbackground, 0);
}

void StoreScene::InitMenu()
{
	// GUI
	auto gui = Node::create();
	gui->setTag(GUI);
	this->addChild(gui, 3);
	
	// Outer menu
	Vector<MenuItem*> MenuItems;
	Size PlayAndMainMenuButtonSize(	visibleSize.width * 0.15f,
									visibleSize.height * 0.1f);
	// Play Button
	auto playButton = MenuItemImage::create("Buttons/nextbton.png", "Buttons/nextbtoff.png", "Buttons/nextbtoff.png",
												CC_CALLBACK_1(StoreScene::ClickPlayButton, this));
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
												CC_CALLBACK_1(StoreScene::ClickMainMenuButton, this));
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

	// Inner menu
	currentbutton = NULL;
	currentimage = NULL;

	int k = 0;
	for (int i = 0; i <5; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Sprite* button = Sprite::create("SkillSelection/skill/skill" + StringUtils::format("%d", k) + ".png");
			button->setTag(BUTTON + k);
			button->setPosition(Point(innerVisibleSize.width*0.075f + i * innerVisibleSize.width*0.12f, innerVisibleSize.height * 0.8f + j*(-innerVisibleSize.height * 0.2f)) + innerMidpointOffset + outerOffset);
			button->setScale(innerVisibleSize.width * 0.1f / button->getContentSize().width,
				innerVisibleSize.height * 0.1f / button->getContentSize().height);
			this->addChild(button, 2);

			Sprite* image = Sprite::create("ShopScene/" + StringUtils::format("%d", k) + ".png");
			image->setTag(BUTTON + k + TOTAL_NUM_OF_ITEMS);
			image->setScale(innerVisibleSize.width * 0.365f / image->getContentSize().width,
				innerVisibleSize.height / image->getContentSize().height);
			image->setPosition(Point(origin.x + innerVisibleSize.width*0.819f, origin.y + innerVisibleSize.height*0.5f) + innerMidpointOffset + outerOffset);
			image->setVisible(false);
			this->addChild(image, 2);

			k++;
		}
	}

	numOfEquipedItems = 0;

	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = 0; i < TOTAL_NUM_OF_ITEMS; i++)
	{
		selected[i] = def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", i)).c_str(), false);
		if (selected[i])
		{
			Sprite* button = (Sprite*)this->getChildByTag(BUTTON + i);
			button->setTexture("SkillSelection/skill/equip" + StringUtils::format("%d", button->getTag() - BUTTON) + ".png");
			numOfEquipedItems++;
		}
	}

	Sprite* buy_button = Sprite::create("ShopScene/buy.png");
	buy_button->setTag(BUY_BUTTON);
	buy_button->setPosition(Point(origin.x + innerVisibleSize.width*0.1f, origin.y + innerVisibleSize.height*0.1f) + innerMidpointOffset + outerOffset);
	buy_button->setScale(0.5);
	this->addChild(buy_button, 2);


	// Gold
	goldLeft = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentGold").c_str(), 0);
	for (int i = Skill::ARCHER; i < Skill::NUM_OF_TYPES; i++)
	{
		price[i] = Item::GetPrice(i);

		float priceLabelFontSize = visibleSize.height * 0.025f;
		auto priceLabel = Label::createWithTTF(StringUtils::format("Price: %d", price[i]), "CocosResources/CuteFont.ttf", priceLabelFontSize);
		Sprite* button = (Sprite*)this->getChildByTag(BUTTON + i);
		priceLabel->setPosition(button->getPosition() + Vec2(0.0f, -button->getScaleY() * button->getContentSize().height * 0.5f - priceLabelFontSize));
		this->addChild(priceLabel, 1);
	}

	auto goldLabel = Label::createWithTTF("Gold", "CocosResources/CuteFont.ttf", 26);
	goldLabel->setPosition(Vec2(origin.x + visibleSize.width*0.55f, origin.y + visibleSize.height*0.18f));
	this->addChild(goldLabel, 1);

	Label* goldLeftLabel = Label::createWithTTF(StringUtils::format("%d", goldLeft), "MiniGameResources/CuteFont.ttf", 40);
	goldLeftLabel->setPosition(Vec2(origin.x + visibleSize.width*0.55f, origin.y + visibleSize.height*0.14f));
	goldLeftLabel->setColor(Color3B(255, 255, 255));
	goldLeftLabel->setTag(NUM_OF_GOLD);
	this->addChild(goldLeftLabel, 2);
	
	CheckAndUnLockPlayButton();
}
void StoreScene::InitTabs(Vector<MenuItem*>* MenuItems)
{
	Size tabSize(visibleSize.width * 0.7f * 0.25f, visibleSize.height * 0.15f);
	// Stat Tab
	auto statTab = MenuItemImage::create(	"Tabs/stat_button.png", "Tabs/push_stat_button.png",
											CC_CALLBACK_1(StoreScene::ClickStatSceneSceneTab, this));
	statTab->setTag(STAT_SCENE_TAB);
	statTab->setPosition(	-tabSize.width * 1.5f,
							visibleSize.height * 0.255f);
	statTab->setScale(	tabSize.width / statTab->getContentSize().width,
						tabSize.height / statTab->getContentSize().height);
	statTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(statTab);
	// Store Tab
	auto storeTab = MenuItemImage::create("Tabs/push_store_button.png", "Tabs/push_store_button.png");
	storeTab->setTag(STORE_SCENE_TAB);
	storeTab->setPosition(	-tabSize.width * 0.5f,
							visibleSize.height * 0.255f);
	storeTab->setScale(	tabSize.width / storeTab->getContentSize().width,
						tabSize.height / storeTab->getContentSize().height);
	storeTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(storeTab);
	
	// Skill Selection Tab
	auto skillsSelectionTab = MenuItemImage::create("Tabs/skill_button.png", "Tabs/push_skill_button.png",
													CC_CALLBACK_1(StoreScene::ClickSkillSelectionSceneTab, this));
	skillsSelectionTab->setTag(SKILL_SELECTION_SCENE_TAB);
	skillsSelectionTab->setPosition(tabSize.width * 0.5f,
									visibleSize.height * 0.255f);
	skillsSelectionTab->setScale(	tabSize.width / skillsSelectionTab->getContentSize().width,
									tabSize.height / skillsSelectionTab->getContentSize().height);
	skillsSelectionTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(skillsSelectionTab);
	
	// Allies Selection Tab
	auto alliesSelectionTab = MenuItemImage::create("Tabs/colleague_button.png", "Tabs/push_colleague_button.png",
													CC_CALLBACK_1(StoreScene::ClickAlliesSelectionSceneTab, this));
	alliesSelectionTab->setTag(ALLIES_SELECTION_SCENE_TAB);
	alliesSelectionTab->setPosition(tabSize.width * 1.5f,
									visibleSize.height * 0.255f);
	alliesSelectionTab->setScale(tabSize.width / alliesSelectionTab->getContentSize().width,
									tabSize.height / alliesSelectionTab->getContentSize().height);
	alliesSelectionTab->setAnchorPoint(Point(0.5f, 0.0f));
	MenuItems->pushBack(alliesSelectionTab);
}

bool StoreScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	//마우스가 누른 위치 받기
	Point location;
	location = touch->getLocation();

	//store gold UI
	if (((Sprite*) this->getChildByTag(BUY_BUTTON))->getBoundingBox().containsPoint(location))
	{
		if (currentbutton)
		{
			if (!selected[currentbutton->getTag() - BUTTON] && TOTAL_NUM_OF_ITEMS < 16 && goldLeft >= price[currentbutton->getTag() - BUTTON])
			{

				goldLeft -= price[currentbutton->getTag() - BUTTON];
				Label* label = ((Label*) this->getChildByTag(NUM_OF_GOLD));
				label->setString(StringUtils::format("%d", goldLeft));

				currentbutton->setTexture("SkillSelection/skill/equip" + StringUtils::format("%d", currentbutton->getTag() - BUTTON) + ".png");
				selected[currentbutton->getTag() - BUTTON] = true;
				currentbutton = NULL;
			}
		}
	}
	//Store UI
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

		for (int i = BUTTON; i < BUTTON + TOTAL_NUM_OF_ITEMS; i++)
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
				Sprite* image = (Sprite*) this->getChildByTag(i + TOTAL_NUM_OF_ITEMS);
				image->setVisible(true);
				currentimage = image;
				break;

			}
		}
	}
	return true;
}

void StoreScene::onTouchEnded(Touch* touch, Event* unused_event)
{
	//마우스가 누른 위치 받기
	Point location;
	location = touch->getLocation();
}

void StoreScene::SaveData()
{
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = 0; i < TOTAL_NUM_OF_ITEMS; i++)
	{
		def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", i)).c_str(), selected[i]);
	}
	def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentGold").c_str(), goldLeft);
}

void StoreScene::CheckAndUnLockPlayButton()
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
void StoreScene::CheckSkillsSelectionCompleted()
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
void StoreScene::CheckStatCompleted()
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

void StoreScene::ClickPlayButton(Ref* pSender)
{
	SaveData();
	StatScene::SaveStatPoints();

	StatScene::DeleteTempSaveStatPoints(false);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, SkillsScene::createScene()));
}

void StoreScene::ClickMainMenuButton(Ref* pSender)
{
	StatScene::DeleteTempSaveStatPoints();

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, IntroScene::createScene()));
}

void StoreScene::ClickAlliesSelectionSceneTab(Ref* pSender)
{
	SaveData();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(AlliesSelectionScene::createScene());
}
void StoreScene::ClickSkillSelectionSceneTab(Ref* pSender)
{
	SaveData();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(SkillSelectionScene::createScene());
}
void StoreScene::ClickStatSceneSceneTab(Ref* pSender)
{
	SaveData();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(StatScene::createScene());
}