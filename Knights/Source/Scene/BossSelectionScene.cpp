#include "BossSelectionScene.h"
#include "SimpleAudioEngine.h"
#include "StatScene.h"
#include "..\GameObject\Units\Unit.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* BossSelectionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = BossSelectionScene::create();
    scene->addChild(layer);

    return scene;
}

bool BossSelectionScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin      = Director::getInstance()->getVisibleOrigin();
	
	InitBackGround();
	InitMenu();

	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/mp3/Music/bossSelection.mp3", true);
	}
    return true;
}
void BossSelectionScene::update(float ft)
{
}

void BossSelectionScene::InitBackGround()
{
	Sprite* backgroundSprite = Sprite::create("bossSelection/boss_finish.png");
	backgroundSprite->setScale(visibleSize.width / backgroundSprite->getContentSize().width, visibleSize.height / backgroundSprite->getContentSize().height);
	backgroundSprite->setPosition(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y);
	this->addChild(backgroundSprite);
}
void BossSelectionScene::InitMenu()
{
	Vector<MenuItem*> MenuItems;
	Size ButtonSize(visibleSize.width * 0.4f, visibleSize.height * 0.4f);
	// Recruit Button
	auto recruit = MenuItemImage::create("bossSelection/get_boss.png", "bossSelection/push_get_boss.png",
										CC_CALLBACK_1(BossSelectionScene::Recruit, this));
	recruit->setScale(	ButtonSize.width / recruit->getContentSize().width,
						ButtonSize.height / recruit->getContentSize().height);
	recruit->setPosition(-visibleSize.width * 0.25f, 0);
	MenuItems.pushBack(recruit);
	// Recruit Button
	auto execute = MenuItemImage::create("bossSelection/kill_boss.png", "bossSelection/push_kill_boss.png",
										CC_CALLBACK_1(BossSelectionScene::Execute, this));
	execute->setScale(	ButtonSize.width / execute->getContentSize().width,
						ButtonSize.height / execute->getContentSize().height);
	execute->setPosition(visibleSize.width * 0.25f, 0);
	MenuItems.pushBack(execute);
	// Menu
	auto menu = Menu::createWithArray(MenuItems);
	this->addChild(menu, 1);
}

void BossSelectionScene::Recruit(Ref* pSender)
{
	// Ynlock boss as ally
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	int index = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_CurrentStageBoss").c_str(), Unit::BOSS_FALLEN_ALCHEMIST) - 1;
	if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_remainedLoyal").c_str(), false))
		index++;
	def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_AllySelectable" + StringUtils::format("%d", index - Unit::BOSS_RICH)).c_str(), true);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, StatScene::createScene()));
}
void BossSelectionScene::Execute(Ref* pSender)
{
	// Add 5 points
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	def->setIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_UnsavedPoints").c_str(), def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_UnsavedPoints").c_str(), 0) + 5);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, StatScene::createScene()));
}