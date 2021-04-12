#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;
USING_NS_CC;


Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);

    return scene;
}

bool HelloWorld::init()
{

	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

		ui::Slider *slider = ui::Slider::create();
		slider->loadBarTexture("Slider/line.png");
		slider->loadBarTexture("Slider/background.png");
		slider->loadSlidBallTextureNormal("Slider/button.png");
		slider->loadProgressBarTexture("Slider/progress.png");
		slider->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 3 + origin.y));

		slider->addEventListener(CC_CALLBACK_2(HelloWorld::sliderEvent, this));
		this->addChild(slider);

		ui::Slider *slider2 = ui::Slider::create();
		slider2->loadBarTexture("Slider/line.png");
		slider2->loadBarTexture("Slider/background.png");
		slider2->loadSlidBallTextureNormal("Slider/button.png");
		slider2->loadProgressBarTexture("Slider/progress2.png");
		slider2->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 1.5 + origin.y));

		slider2->addEventListener(CC_CALLBACK_2(HelloWorld::sliderEvent, this));
		this->addChild(slider2);
		return true;
}

void HelloWorld::sliderEvent(Ref *sender, ui::Slider::EventType type)
{
	//HELP GREG!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//if (type == ui::Slider::EventType::ON_PERCENTAGE_CHANGED)
	//{
	//	ui::Slider *slider = dynamic_cast<ui::Slider *>(sender);
	//	int percent = slider->getPercent();
	//	//log("%i", percent);
	//	Label* slidernumber = Label::createWithTTF(StringUtils::format("number : %d", percent), "MiniGameResources/CuteFont.ttf");
	//	slidernumber->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 1 + origin.y));
	//	slidernumber->setColor(Color3B::WHITE);
	//	slidernumber->setAnchorPoint(Point(0.0f, 0.5f));
	//	slidernumber->setTag(SLIDER);
	//	this->addChild(slidernumber, 3);
	//}
}