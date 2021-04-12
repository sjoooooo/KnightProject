#include "LoadDataScene.h"
#include "SimpleAudioEngine.h"
#include "StatScene.h"
#include "DataSelectionScene.h"
#include "MercenaryShopScene.h"
#include "StoreScene.h"
#include "GameCompleteScene.h"
#include "SkillSelectionScene.h"
#include "..\GameObject\Units\Unit.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* LoadDataScene::createScene()
{
    auto scene = Scene::create();
	auto layer = LoadDataScene::create();
    scene->addChild(layer);

    return scene;
}

bool LoadDataScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin      = Director::getInstance()->getVisibleOrigin();
	
	InitBackGround();
	InitMenu();

    return true;
}
void LoadDataScene::update(float ft)
{
}

void LoadDataScene::InitBackGround()
{
	Sprite* backgroundSprite = Sprite::create("KnightResources/BackGround1.png");
	backgroundSprite->setScale(visibleSize.width / backgroundSprite->getContentSize().width, visibleSize.height / backgroundSprite->getContentSize().height);
	backgroundSprite->setPosition(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y);
	this->addChild(backgroundSprite);

	Sprite* logoSpr = Sprite::create("KnightResources/knightlogo1.png");
	logoSpr->setScale(visibleSize.width * 0.3f / logoSpr->getContentSize().width, visibleSize.height * 0.3f / logoSpr->getContentSize().height);
	logoSpr->setPosition(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.75f + origin.y);
	this->addChild(logoSpr);
}
void LoadDataScene::InitMenu()
{
	Vector<MenuItem*> MenuItems;
	Size DataButtonSize(visibleSize.width * 0.25f, visibleSize.height * 0.25f);
	Size SceneButtonSize(visibleSize.height * 0.1f, visibleSize.height * 0.1f);


	UserDefault* def = UserDefault::getInstance();

	currentData = def->getIntegerForKey("CurrentData", 1);

	if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_IsNew").c_str(), true))
	{
		// New Button
		auto newButton = MenuItemImage::create("Buttons/Save/new.png", "Buttons/Save/push_new.png",
												CC_CALLBACK_1(LoadDataScene::Load, this));
		newButton->setScale(DataButtonSize.width / newButton->getContentSize().width,
							DataButtonSize.height / newButton->getContentSize().height);
		newButton->setPosition(0, -visibleSize.height * 0.2f);
		MenuItems.pushBack(newButton);
	}
	else
	{
		// Load Button
		auto loadButton = MenuItemImage::create("Buttons/Save/load.png", "Buttons/Save/push_load.png",
												CC_CALLBACK_1(LoadDataScene::Load, this));
		loadButton->setScale(	DataButtonSize.width / loadButton->getContentSize().width,
								DataButtonSize.height / loadButton->getContentSize().height);
		loadButton->setPosition(-visibleSize.width * 0.2f, -visibleSize.height * 0.2f);
		MenuItems.pushBack(loadButton);
		// Reset Button
		auto resetButton = MenuItemImage::create("Buttons/Save/delete.png", "Buttons/Save/push_delete.png",
													CC_CALLBACK_1(LoadDataScene::Reset, this));
		resetButton->setScale(	DataButtonSize.width / resetButton->getContentSize().width,
								DataButtonSize.height / resetButton->getContentSize().height);
		resetButton->setPosition(visibleSize.width * 0.2f, -visibleSize.height * 0.2f);
		MenuItems.pushBack(resetButton);
	}
	// Prevous Scene Button
	auto prevSceneButton = MenuItemImage::create(	"Buttons/backbton.png", "Buttons/backbtoff.png",
													CC_CALLBACK_1(LoadDataScene::PrevScene, this));
	prevSceneButton->setScale(	SceneButtonSize.width / prevSceneButton->getContentSize().width,
								SceneButtonSize.height / prevSceneButton->getContentSize().height);
	prevSceneButton->setPosition(-visibleSize.width * 0.5f + SceneButtonSize.width, visibleSize.height * 0.5f - SceneButtonSize.height);
	MenuItems.pushBack(prevSceneButton);

	// Menu
	auto menu = Menu::createWithArray(MenuItems);
	this->addChild(menu, 1);
}

void LoadDataScene::Load(Ref* pSender)
{
	StatScene::DeleteTempSaveStatPoints(false);
	UserDefault* def = UserDefault::getInstance();
	def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_IsNew").c_str(), false);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();

	if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "JustWon").c_str(), false))
	{
		int type = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_RICH);
		if (type == Unit::BOSS_QUEEN)
		{
			Director::getInstance()->replaceScene(TransitionFade::create(1.0f, GameCompleteScene::createScene()));
		}
		else
		{
			Director::getInstance()->replaceScene(TransitionFade::create(1.0f, MercenaryShopScene::createScene()));
		}
	}
	else
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, StatScene::createScene()));
}

void LoadDataScene::Reset(Ref* pSender)
{
	UserDefault* def = UserDefault::getInstance();

	// Stat
	for (int i = StatScene::STR_PLUS_BUTTON; i < StatScene::NUM_OF_PLUS_BUTTONS; i++)
	{
		def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", i)).c_str());
		def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_TempStat" + StringUtils::format("%d", i)).c_str());
	}
	def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_AddNewPoints").c_str());
	def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_UnsavedPoints").c_str());
	def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_TempUnsavedPoints").c_str());

	// Skills
	for (int i = 0; i < SkillSelectionScene::TOTAL_NUM_OF_SKILLS; i++)
	{
		def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_SkillSelected" + StringUtils::format("%d", i)).c_str());
	}

	// Items
	for (int i = 0; i < StoreScene::TOTAL_NUM_OF_ITEMS; i++)
	{
		def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", i)).c_str());
	}


	// Allies
	for (int i = 0; i <= Unit::BOSS_KING - Unit::BOSS_RICH; i++)
	{
		def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_AllySelected" + StringUtils::format("%d", i)).c_str());
		def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_AllySelectable" + StringUtils::format("%d", i)).c_str());
	}

	// Boss
	def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str());

	// Just won: for boss capture
	def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "JustWon").c_str());

	// Loyalty and betray
	def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_remainedLoyal").c_str());

	// Current gold
	def->deleteValueForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentGold").c_str());

	// Data is New
	def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_IsNew").c_str(), true);

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, LoadDataScene::createScene()));
}

void LoadDataScene::PrevScene(Ref* pSender)
{
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, DataSelectionScene::createScene()));
}