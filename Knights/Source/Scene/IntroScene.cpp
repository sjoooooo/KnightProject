#include "IntroScene.h"
#include "SimpleAudioEngine.h"
#include "OptionScene.h"
#include "StatScene.h"
#include "SkillsScene.h"
#include "DataSelectionScene.h"

//사운드 할라면 꼭 필요
using namespace CocosDenshion;
USING_NS_CC;

Scene* IntroScene::createScene()
{
	auto scene = Scene::create();
	auto layer = IntroScene::create();
	scene->addChild(layer);

	return scene;
}

bool IntroScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	InitBackGround();
	InitLabel();
	InitMenu();

	//터치 리스너 등록
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(IntroScene::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(listener, 1);

	//배경음악
	if (!SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
	{
		SimpleAudioEngine::getInstance()->playBackgroundMusic("Sounds/mp3/Music/intro.mp3", true);
	}

	return true;
}
void IntroScene::update(float ft)
{
	//씬 자체적으로 반복 되야하는 내용
}
void IntroScene::InitBackGround()
{
	auto sprite = Sprite::create("KnightResources/backGround1.png");
	sprite->setScale(visibleSize.width / sprite->getContentSize().width, visibleSize.height / sprite->getContentSize().height);
	sprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(sprite, 0);

	Sprite* logoSpr = Sprite::create("KnightResources/knightLogo1.png");
	logoSpr->setScale(visibleSize.width * 0.3f / logoSpr->getContentSize().width, visibleSize.height * 0.3f / logoSpr->getContentSize().height);
	logoSpr->setPosition(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.75f + origin.y);
	this->addChild(logoSpr);
}

void IntroScene::InitLabel()
{
	auto label = Label::create("Press START Button", "knightResources/CuteFont.ttf", 40);
	label->setTextColor(Color4B(255, 255, 255, 255));
	label->setPosition(Point(615, 100));
	this->addChild(label, 2);

	auto action_0 = Sequence::create(FadeOut::create(2), FadeIn::create(2), NULL);
	auto action_1 = RepeatForever::create(action_0);
	label->runAction(action_1);
}
void IntroScene::InitMenu()
{
	Size buttonSize(visibleSize.width * 0.2f, visibleSize.height * 0.1f);
	//특정 이유 때문에 일부로 사진을 넣었다.
	auto startbt = MenuItemImage::create("Buttons/starton.png", "Buttons/startoff.png", CC_CALLBACK_1(IntroScene::menuStartCallback, this));
	
	startbt->setScale(	buttonSize.width / startbt->getContentSize().width,
						buttonSize.height / startbt->getContentSize().height);
	auto start = Menu::create(startbt, NULL);
	this->addChild(start, 2);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	startbt->setPosition(Point(0, 0));

	auto optionbt = MenuItemImage::create(
		"Buttons/option.png",
		"Buttons/push_option.png",
		CC_CALLBACK_1(IntroScene::menuOptionCallback, this));
	optionbt->setPosition(Point(0, -visibleSize.height * 0.125f));
	optionbt->setScale(	buttonSize.width / optionbt->getContentSize().width,
						buttonSize.height / optionbt->getContentSize().height);

	auto option = Menu::create(optionbt, NULL);
	this->addChild(option, 2);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	startbt->setPosition(Point(0, -visibleSize.height * 0.125f));
#endif

	auto quitbt = MenuItemImage::create(
		"Buttons/quiton.png",
		"Buttons/quitoff.png",
		CC_CALLBACK_1(IntroScene::menuCloseCallback, this));
	quitbt->setPosition(Point(0, -visibleSize.height * 0.25f));
	quitbt->setScale(	buttonSize.width / quitbt->getContentSize().width,
						buttonSize.height / quitbt->getContentSize().height);

	auto quit = Menu::create(quitbt, NULL);
	this->addChild(quit, 2);
}
bool IntroScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	return false;
}

void IntroScene::menuCloseCallback(Ref* pSender)
{
	// unload sounds
	auto audio = SimpleAudioEngine::getInstance();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	audio->unloadEffect("Sounds/ogg/Effects/arrows.ogg");
	audio->unloadEffect("Sounds/ogg/Effects/fire_ball.ogg");
	audio->unloadEffect("Sounds/ogg/Effects/hurricane.ogg");
	audio->unloadEffect("Sounds/ogg/Effects/ice_spear.ogg");
	audio->unloadEffect("Sounds/ogg/Effects/lightning.ogg");
	audio->unloadEffect("Sounds/ogg/Effects/Mass_unit_spawn.ogg");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	audio->preloadEffect("Sounds/wav/Effects/arrows.wav");
	audio->preloadEffect("Sounds/wav/Effects/fire_ball.wav");
	audio->preloadEffect("Sounds/wav/Effects/hurricane.wav");
	audio->preloadEffect("Sounds/wav/Effects/ice_spear.wav");
	audio->preloadEffect("Sounds/wav/Effects/lightning.wav");
	audio->preloadEffect("Sounds/wav/Effects/Mass_unit_spawn.wav");
	audio->preloadEffect("Sounds/wav/Effects/Gold_Use_sound.wav");
	audio->preloadEffect("Sounds/wav/Effects/archer_man_attack_sound.wav");
	audio->preloadEffect("Sounds/wav/Effects/lightning_wizard_attack.wav");
	audio->preloadEffect("Sounds/wav/Effects/horse_man_spawn.wav");
	audio->preloadEffect("Sounds/wav/Effects/ice_wizard_attack.wav");
#endif


	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
void IntroScene::menuStartCallback(Ref* pSender)
{
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, DataSelectionScene::createScene()));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}
void IntroScene::menuOptionCallback(Ref* pSender)
{
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, OptionScene::createScene()));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}