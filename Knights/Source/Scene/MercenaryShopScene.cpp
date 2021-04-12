#include "MercenaryShopScene.h"
#include "SimpleAudioEngine.h"
#include "IntroScene.h"
#include "BossCapScene.h"
#include "StatScene.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* MercenaryShopScene::createScene()
{
    auto scene = Scene::create();
    auto layer = MercenaryShopScene::create();
    scene->addChild(layer);

    return scene;
}

bool MercenaryShopScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin      = Director::getInstance()->getVisibleOrigin();

	// Add number of luck here
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	// Get saved unused points
	goldLeft = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::PRC_PLUS_BUTTON)).c_str(), 0) * 50;

	InitBackGround();
	InitGUI();
		
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(MercenaryShopScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/mp3/Music/Skill_stat.mp3", true);
	}
    return true;
}
void MercenaryShopScene::update(float ft)
{
}
void MercenaryShopScene::InitBackGround()
{
	auto background = Sprite::create("MercenaryShopScene/background.png");
	background->setPosition(Point(	visibleSize.width * 0.5f + origin.x, 
									visibleSize.height * 0.5f + origin.y));
	background->setScale(	(visibleSize.width / background->getContentSize().width), 
							(visibleSize.height / background->getContentSize().height));
	this->addChild(background, 0);
}
void MercenaryShopScene::InitGUI()
{
	// GUI
	auto gui = Node::create();
	gui->setTag(GUI);
	this->addChild(gui, 1);

	/* Menu */
	Vector<MenuItem*> MenuItems;
	Size MenuButtonSize(visibleSize.height * 0.15f, visibleSize.height * 0.1f);
	float spacingOffset(visibleSize.height * 0.05f);
	

	// Prev button
	auto prevButton = MenuItemImage::create("Buttons/backbton.png", "Buttons/backbtoff.png",
										CC_CALLBACK_1(MercenaryShopScene::PrevScene, this));
	prevButton->setScale(	MenuButtonSize.width / prevButton->getContentSize().width,
							MenuButtonSize.height / prevButton->getContentSize().height);
	prevButton->setPosition(-visibleSize.width * 0.5f + MenuButtonSize.width * 0.5f + spacingOffset,
							visibleSize.height * 0.5f - MenuButtonSize.height * 0.5f - spacingOffset);
	MenuItems.pushBack(prevButton);
	// Next button
	auto nextButton = MenuItemImage::create("Buttons/nextbton.png", "Buttons/nextbtoff.png",
										CC_CALLBACK_1(MercenaryShopScene::NextScene, this));
	nextButton->setScale(	MenuButtonSize.width / nextButton->getContentSize().width,
							MenuButtonSize.height / nextButton->getContentSize().height);
	nextButton->setPosition(visibleSize.width * 0.5f - MenuButtonSize.width * 0.5f - spacingOffset,
							visibleSize.height * 0.5f - MenuButtonSize.height * 0.5f - spacingOffset);
	MenuItems.pushBack(nextButton);


	auto menu = Menu::createWithArray(MenuItems);
	menu->setTag(MENU);
	gui->addChild(menu, 1);
	/********/

	/* Plus Buttons */
	Size PlusButtonSize(visibleSize.height * 0.1f, visibleSize.height * 0.1f);
	Vec2 SWORDSMANPosition(	visibleSize.width * 0.35f + origin.x,
							visibleSize.height * 0.7f + origin.y);
	Vec2 plusButtonOffset(	visibleSize.width * 0.5f,
							visibleSize.height * 0.25f);
	float amountFontSize = visibleSize.height * 0.25f;
	Vec2 amountPositionOffset(	-PlusButtonSize.width * 1.25f,
								PlusButtonSize.height * 1.5f);
	for (int i = SWORDSMAN; i < NUM_OF_MERCENARIES; i++)
	{
		AmountOfMercenaries[i - SWORDSMAN] = 0;
		// Plus Button
		Sprite* plusButton = Sprite::create("StatScene/plus_button.png");
		plusButton->setTag(i);
		// Scale
		plusButton->setScale(	PlusButtonSize.width / plusButton->getContentSize().width,
								PlusButtonSize.height / plusButton->getContentSize().height);
		// Position
		if (i < HORSEMAN)
		{
			plusButton->setPositionX(SWORDSMANPosition.x);
			plusButton->setPositionY(SWORDSMANPosition.y - (i - SWORDSMAN) * plusButtonOffset.y);
		}
		else
		{
			plusButton->setPositionX(SWORDSMANPosition.x + plusButtonOffset.x);
			plusButton->setPositionY(SWORDSMANPosition.y - (i - HORSEMAN) * plusButtonOffset.y);
		}
		// Add to GUI
		gui->addChild(plusButton, 1);

		// Amount Label
		auto amount = Label::create(StringUtils::format("%d", AmountOfMercenaries[i - SWORDSMAN]), "MiniGameResources/CuteFont.ttf", amountFontSize);
		amount->setTextColor(Color4B::WHITE);
		amount->setPosition(amountPositionOffset);
		amount->setAnchorPoint(Point(1.0f, 0.5f));
		amount->setTag(AMOUNT);
		plusButton->addChild(amount, 1);
	}
	/****************/
	// Cost of mercenary
	CostOfMercenaries[SWORDSMAN - SWORDSMAN] = 20;
	CostOfMercenaries[ARCHER - SWORDSMAN] = 25;
	CostOfMercenaries[SPEARMAN - SWORDSMAN] = 40;
	CostOfMercenaries[HORSEMAN - SWORDSMAN] = 35;
	CostOfMercenaries[SHIELDMAN - SWORDSMAN] = 50;
	CostOfMercenaries[WIZARD - SWORDSMAN] = 80;

	CheckAndLockAllPlusButton();
	
	// Gold Left
	float goldLefttFontSize = visibleSize.height * 0.075f;
	auto goldLeftLabel = Label::create(StringUtils::format("%d", goldLeft), "MiniGameResources/CuteFont.ttf", goldLefttFontSize);
	goldLeftLabel->setTextColor(Color4B::WHITE);
	goldLeftLabel->setPosition(	visibleSize.width * 0.825f + origin.x + goldLeftLabel->getContentSize().width * goldLeftLabel->getScaleX() * 0.5f,
								origin.y + goldLefttFontSize * 0.5f);
	goldLeftLabel->setAnchorPoint(Point(0.0f, 0.5f));
	goldLeftLabel->setTag(GOLD_LEFT_LABEL);
	gui->addChild(goldLeftLabel, 1);
}
void MercenaryShopScene::IncreasePoint(Sprite* plusButton)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	//SimpleAudioEngine::getInstance()->playEffect("Sounds/ogg/Effects/Gold_Use_sound.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	SimpleAudioEngine::getInstance()->playEffect("Sounds/wav/Effects/Gold_Use_sound.wav");
#endif
	// Increse this mercenary points
	Label* amount = (Label*)plusButton->getChildByTag(AMOUNT);
	amount->setString(StringUtils::format("%d", ++AmountOfMercenaries[plusButton->getTag() - SWORDSMAN]));

	// Reduce gold left
	goldLeft -= CostOfMercenaries[plusButton->getTag() - SWORDSMAN];
	Label* goldLeftLabel = (Label*)this->getChildByTag(GUI)->getChildByTag(GOLD_LEFT_LABEL);
	goldLeftLabel->setString(StringUtils::format("%d", goldLeft));

	CheckAndLockAllPlusButton();
}
void MercenaryShopScene::CheckAndLockAllPlusButton()
{
	// Check and Lock all of the plus_buttons
	for (int i = SWORDSMAN; i < NUM_OF_MERCENARIES; i++)
	{
		CheckAndLockPlusButton((Sprite*)this->getChildByTag(GUI)->getChildByTag(i));
	}
}
void MercenaryShopScene::CheckAndLockPlusButton(Sprite* plusButton)
{
	if (CostOfMercenaries[plusButton->getTag() - SWORDSMAN] > goldLeft)
	{
		LockPlusButtons(plusButton);
	}
}
void MercenaryShopScene::LockPlusButtons(Sprite* plusButton)
{
	// Tint to gray color
	auto action = TintTo::create(0.1f, Color3B::GRAY);
	plusButton->runAction(action);
}
bool MercenaryShopScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	Point location = touch->getLocation();

		// Check all of the plus_buttons
	for (int i = SWORDSMAN; i < NUM_OF_MERCENARIES; i++)
	{
		Sprite* plusButton = (Sprite*)getChildByTag(GUI)->getChildByTag(i);

		if (CostOfMercenaries[plusButton->getTag() - SWORDSMAN] <= goldLeft)
		{
			Rect rect = plusButton->getBoundingBox();

			// Check if touch location on this plus button
			if (rect.containsPoint(location)) {
				IncreasePoint(plusButton);
				break;
			}
		}
	}
	return false;
}

void MercenaryShopScene::NextScene(Ref* pSender)
{
	// Save amount of mercenaries bought
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	for (int i = SWORDSMAN; i < NUM_OF_MERCENARIES; i++)
	{
		std::string keyName = "Data" + StringUtils::format("%d", currentData) + "_MercaneryBought" + StringUtils::format("%d", i);
		def->setIntegerForKey(keyName.c_str(), AmountOfMercenaries[i - SWORDSMAN]);
	}

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, BossCapScene::createScene()));
}

void MercenaryShopScene::PrevScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, IntroScene::createScene()));
}