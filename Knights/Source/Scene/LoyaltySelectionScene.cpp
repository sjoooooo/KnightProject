#include "LoyaltySelectionScene.h"
#include "SimpleAudioEngine.h"
#include "BossSelectionScene.h"
#include "..\GameObject\Units\Unit.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* LoyaltySelectionScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LoyaltySelectionScene::create();
    scene->addChild(layer);

    return scene;
}

bool LoyaltySelectionScene::init()
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
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/mp3/Music/traitor_or_loyalist.mp3", true);
	}
    return true;
}
void LoyaltySelectionScene::update(float ft)
{
}

void LoyaltySelectionScene::InitBackGround()
{
	Sprite* backgroundSprite = Sprite::create("bossSelection/boss_finish.png");
	backgroundSprite->setScale(visibleSize.width / backgroundSprite->getContentSize().width, visibleSize.height / backgroundSprite->getContentSize().height);
	backgroundSprite->setPosition(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.5f + origin.y);
	this->addChild(backgroundSprite);
}
void LoyaltySelectionScene::InitMenu()
{
	Vector<MenuItem*> MenuItems;
	Size ButtonSize(visibleSize.width * 0.4f, visibleSize.height * 0.4f);
	// Loayl Button
	auto loayl = MenuItemImage::create("Buttons/LoyaltySelection/loyal_button.png", "Buttons/LoyaltySelection/push_loyal_button.png",
										CC_CALLBACK_1(LoyaltySelectionScene::Loyal, this));
	loayl->setScale(ButtonSize.width / loayl->getContentSize().width,
					ButtonSize.height / loayl->getContentSize().height);
	loayl->setPosition(-visibleSize.width * 0.25f, 0);
	MenuItems.pushBack(loayl);
	// Betray Button
	auto betray = MenuItemImage::create("Buttons/LoyaltySelection/betrayal_button.png", "Buttons/LoyaltySelection/push_betrayal_button.png",
										CC_CALLBACK_1(LoyaltySelectionScene::Betray, this));
	betray->setScale(	ButtonSize.width / betray->getContentSize().width,
						ButtonSize.height / betray->getContentSize().height);
	betray->setPosition(visibleSize.width * 0.25f, 0);
	MenuItems.pushBack(betray);
	// Menu
	auto menu = Menu::createWithArray(MenuItems);
	this->addChild(menu, 1);
}

void LoyaltySelectionScene::Loyal(Ref* pSender)
{

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, BossSelectionScene::createScene()));
}
void LoyaltySelectionScene::Betray(Ref* pSender)
{
	// Remain Loyal
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	def->setBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_remainedLoyal").c_str(), true);

	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, BossSelectionScene::createScene()));
}