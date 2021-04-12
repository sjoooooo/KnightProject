#include "SplashScene.h"
#include "SimpleAudioEngine.h"
#include "IntroScene.h"

//사운드 할라면 꼭 필요
using namespace CocosDenshion;
USING_NS_CC;

Scene* SplashScene::createScene()
{
	auto scene = Scene::create();
	auto layer = SplashScene::create();
	scene->addChild(layer);

	return scene;
}

bool SplashScene::init()
{

	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	InitBackGround();

	this->schedule(schedule_selector(SplashScene::update));

	auto splashsceneTime = DelayTime::create(3.0f);
	splashsceneTime->setTag(SPLASH_TIME);
	this->runAction(splashsceneTime);

	nextScene = false;

	return true;
}
void SplashScene::update(float ft)
{
	auto splashsceneTime = (DelayTime*)this->getActionByTag(SPLASH_TIME);
	if (!splashsceneTime && !nextScene)
	{
		nextScene = true;
		Director::getInstance()->replaceScene(TransitionFade::create(1.0f, IntroScene::createScene()));
	}
}
void SplashScene::InitBackGround()
{
	auto sprite = Sprite::create("Bug_soft.png");
	sprite->setScale(visibleSize.width / sprite->getContentSize().width, visibleSize.height * 0.5f / sprite->getContentSize().height);
	sprite->setPosition(Point(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.75f + origin.y));
	this->addChild(sprite, 0);

	sprite = Sprite::create("nyp_sidm_logos.png");
	sprite->setScale(visibleSize.width / sprite->getContentSize().width, visibleSize.height * 0.5f / sprite->getContentSize().height);
	sprite->setPosition(Point(visibleSize.width * 0.5f + origin.x, visibleSize.height * 0.25f + origin.y));
	this->addChild(sprite, 0);
}