#include "OptionScene.h"
#include "SimpleAudioEngine.h"
#include "ui\CocosGUI.h"
#include "SimpleAudioEngine.h"
#include "IntroScene.h"

using namespace CocosDenshion;
USING_NS_CC;




Scene* OptionScene::createScene()
{
	auto scene = Scene::create();
	auto layer = OptionScene::create();
	scene->addChild(layer);

	return scene;
}

void OptionScene::InitBackGround()
{
	auto sprite = Sprite::create("KnightResources/BackGround1.png");
	sprite->setScale(visibleSize.width / sprite->getContentSize().width, visibleSize.height / sprite->getContentSize().height);
	sprite->setPosition(Point(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(sprite, 0);

	auto backgroundmusicicon = Sprite::create("Slider/backgroundmusicicon.png");
	backgroundmusicicon->setScale(0.5f);
	backgroundmusicicon->setPosition(Vec2(visibleSize.width / 8 + origin.x, visibleSize.height / 3 + origin.y));
	this->addChild(backgroundmusicicon, 1);

	auto effectmusicicon = Sprite::create("Slider/effectmusicicon.png");
	effectmusicicon->setScale(0.5f);
	effectmusicicon->setPosition(Vec2(visibleSize.width / 8 + origin.x, visibleSize.height / 1.5 + origin.y));
	this->addChild(effectmusicicon, 1);
}

void OptionScene::InitMenu()
{
	auto backtomenu = MenuItemImage::create(
		"Buttons/closebton.png", "Buttons/closebtoff.png",
		CC_CALLBACK_1(OptionScene::menuCloseCallback, this));
	backtomenu->setPosition(visibleSize.width * 0.4f, visibleSize.height *  0.4f);
	//backtomenu->setScale(0.6f);
	auto quit = Menu::create(backtomenu, NULL);
	this->addChild(quit, 1);
}

void OptionScene::menuCloseCallback(Ref* pSender)
{
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, IntroScene::createScene()));

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

bool OptionScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	InitBackGround();
	InitMenu();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ui::Slider *slider = ui::Slider::create();
	slider->loadBarTexture("Slider/line.png");
	slider->loadBarTexture("Slider/background.png");
	slider->loadSlidBallTextureNormal("Slider/button.png");
	slider->loadProgressBarTexture("Slider/progress.png");
	slider->setPosition(Vec2(visibleSize.width / 1.8 + origin.x, visibleSize.height / 1.5 + origin.y));
	slider->setScale(0.7);
	slider->setPercent(SimpleAudioEngine::getInstance()->getBackgroundMusicVolume() * 100.0f);

	slider->addEventListener(CC_CALLBACK_2(OptionScene::BackGroundSoundControl, this));
	this->addChild(slider);

	ui::Slider *slider2 = ui::Slider::create();
	slider2->loadBarTexture("Slider/line.png");
	slider2->loadBarTexture("Slider/background.png");
	slider2->loadSlidBallTextureNormal("Slider/button.png");
	slider2->loadProgressBarTexture("Slider/progress2.png");
	slider2->setPosition(Vec2(visibleSize.width / 1.8 + origin.x, visibleSize.height / 3 + origin.y));
	slider2->setScale(0.7);
	slider2->setPercent(SimpleAudioEngine::getInstance()->getEffectsVolume() * 100.0f);

	slider2->addEventListener(CC_CALLBACK_2(OptionScene::EffectSoundControl, this));
	this->addChild(slider2);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#endif*/

	return true;
}

void OptionScene::BackGroundSoundControl(Ref *sender, ui::Slider::EventType type)
{
	auto backgroundmusic = SimpleAudioEngine::getInstance();

	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		ui::Slider *slider = dynamic_cast<ui::Slider *>(sender);
		float percent = slider->getPercent();
		//log("%i", percent);
		backgroundmusic->setBackgroundMusicVolume(percent / 100.f);
	}
}

void OptionScene::EffectSoundControl(Ref *sender, ui::Slider::EventType type)
{
	auto effectmusic = SimpleAudioEngine::getInstance();

	if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		ui::Slider *slider2 = dynamic_cast<ui::Slider *>(sender);
		float percent2 = slider2->getPercent();
		//log("%i", percent);
		effectmusic->setEffectsVolume(percent2 / 100.f);
	}
}
