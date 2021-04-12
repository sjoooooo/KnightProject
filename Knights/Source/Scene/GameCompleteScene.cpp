#include "GameCompleteScene.h"
#include "SimpleAudioEngine.h"
#include "IntroScene.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* GameCompleteScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameCompleteScene::create();
    scene->addChild(layer);

    return scene;
}

bool GameCompleteScene::init()
{

    if ( !Layer::init() )
    {
        return false;
    }

	visibleSize = Director::getInstance()->getVisibleSize();
	origin      = Director::getInstance()->getVisibleOrigin();
	
	InitBackGround();

	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/mp3/Music/traitor_or_loyalist.mp3", true);
	}


    return true;
}

void GameCompleteScene::InitBackGround()
{
	Vector<MenuItem*> MenuItems;

	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	// Remain Loyal
	if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_remainedLoyal").c_str(), false))
	{
		auto background = MenuItemImage::create("End/loyal_end.png", "End/loyal_end.png",
												CC_CALLBACK_1(GameCompleteScene::NextScene, this));
		background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		MenuItems.pushBack(background);
	}
	// betrayed
	else
	{
		auto background = MenuItemImage::create("End/betrayal_end.png", "End/betrayal_end.png",
												CC_CALLBACK_1(GameCompleteScene::NextScene, this));
		background->setScale(visibleSize.width / background->getContentSize().width, visibleSize.height / background->getContentSize().height);
		MenuItems.pushBack(background);
	}
	// Menu
	auto menu = Menu::createWithArray(MenuItems);
	this->addChild(menu, 1);
}
void GameCompleteScene::NextScene(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, IntroScene::createScene()));
}