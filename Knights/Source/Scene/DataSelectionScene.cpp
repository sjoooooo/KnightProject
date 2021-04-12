#include "DataSelectionScene.h"
#include "SimpleAudioEngine.h"
#include "LoadDataScene.h"
#include "IntroScene.h"
#include "..\GameObject\Units\Unit.h"

using namespace CocosDenshion;
USING_NS_CC;

Scene* DataSelectionScene::createScene()
{
    auto scene = Scene::create();
	auto layer = DataSelectionScene::create();
    scene->addChild(layer);

    return scene;
}

bool DataSelectionScene::init()
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
void DataSelectionScene::update(float ft)
{
}

void DataSelectionScene::InitBackGround()
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
void DataSelectionScene::InitMenu()
{
	Vector<MenuItem*> MenuItems;
	Size DataButtonSize(visibleSize.width * 0.25f, visibleSize.height * 0.25f);
	Size SceneButtonSize(visibleSize.height * 0.1f, visibleSize.height * 0.1f);
	// Data1 Button
	auto data1Button = MenuItemImage::create("Buttons/Save/save-1.png", "Buttons/Save/push_save-1.png",
										CC_CALLBACK_1(DataSelectionScene::Data1, this));
	data1Button->setScale(	DataButtonSize.width / data1Button->getContentSize().width,
							DataButtonSize.height / data1Button->getContentSize().height);
	data1Button->setPosition(-visibleSize.width * 0.3f, -visibleSize.height * 0.2f);
	MenuItems.pushBack(data1Button);
	// Data2 Button
	auto data2Button = MenuItemImage::create("Buttons/Save/save-2.png", "Buttons/Save/push_save-2.png",
										CC_CALLBACK_1(DataSelectionScene::Data2, this));
	data2Button->setScale(	DataButtonSize.width / data2Button->getContentSize().width,
							DataButtonSize.height / data2Button->getContentSize().height);
	data2Button->setPosition(0, -visibleSize.height * 0.2f);
	data2Button->setTag(DATA_2);
	MenuItems.pushBack(data2Button);
	// Data3 Button
	auto data3Button = MenuItemImage::create("Buttons/Save/save-3.png", "Buttons/Save/push_save-3.png",
										CC_CALLBACK_1(DataSelectionScene::Data3, this));
	data3Button->setScale(	DataButtonSize.width / data3Button->getContentSize().width,
							DataButtonSize.height / data3Button->getContentSize().height);
	data3Button->setPosition(visibleSize.width * 0.3f, -visibleSize.height * 0.2f);
	MenuItems.pushBack(data3Button);
	
	// Prevous Scene Button
	auto prevSceneButton = MenuItemImage::create(	"Buttons/backbton.png", "Buttons/backbtoff.png",
													CC_CALLBACK_1(DataSelectionScene::PrevScene, this));
	prevSceneButton->setScale(	SceneButtonSize.width / prevSceneButton->getContentSize().width,
								SceneButtonSize.height / prevSceneButton->getContentSize().height);
	prevSceneButton->setPosition(-visibleSize.width * 0.5f + SceneButtonSize.width, visibleSize.height * 0.5f - SceneButtonSize.height);
	MenuItems.pushBack(prevSceneButton);

	// Menu
	auto menu = Menu::createWithArray(MenuItems);
	this->addChild(menu, 1);

	// Labels
	float labelSize = visibleSize.height * 0.05f;
	UserDefault* def = UserDefault::getInstance();
	for (int i = DATA_1; i <= DATA_3; i++)
	{
		bool isNew = def->getBoolForKey(StringUtils::format("Data%d_IsNew", i).c_str(), true);
		std::string toShow = "";
		// New
		if (isNew)
			toShow = "*New*";
		// Old
		else
		{
			int currentBoss = def->getIntegerForKey(StringUtils::format("Data%d_CurrentStageBoss", i).c_str(), Unit::BOSS_RICH);
			bool bossCap = def->getBoolForKey(StringUtils::format("Data%dJustWon", i).c_str(), false);
			// Completed
			if (currentBoss == Unit::BOSS_QUEEN && bossCap)
			{
				toShow = "*Completed*";
			}
			else
			{
				toShow = StringUtils::format("Stage: %d", currentBoss - Unit::BOSS_RICH + 1);
				if (bossCap)
					toShow += "-1";
			}
		}
		auto dataStageLabel = Label::create(toShow.c_str(), "CocosResources/CuteFont.ttf", labelSize);
		dataStageLabel->setAnchorPoint(Point(0.5f, 0.0f));
		auto dataButton = MenuItems.at(i - 1);
		Vec2 offset(0.0f, -(dataButton->getScaleY() * dataButton->getContentSize().height) * 0.5f);
		Vec2 midPoint(	origin.x + visibleSize.width * 0.5f,
						origin.y + visibleSize.height * 0.5f);
		dataStageLabel->setPosition(dataButton->getPosition() + midPoint + offset);
		this->addChild(dataStageLabel, 2);
	}
}

void DataSelectionScene::Data1(Ref* pSender)
{
	SelectData(1);
}

void DataSelectionScene::Data2(Ref* pSender)
{
	SelectData(2);
}

void DataSelectionScene::Data3(Ref* pSender)
{
	SelectData(3);
}

void DataSelectionScene::PrevScene(Ref* pSender)
{
	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, IntroScene::createScene()));
}

void DataSelectionScene::SelectData(int DataNo)
{
	UserDefault* def = UserDefault::getInstance();
	def->setIntegerForKey("CurrentData", DataNo);

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	Director::getInstance()->replaceScene(LoadDataScene::createScene());
	Director::getInstance()->replaceScene(TransitionFade::create(1.0f, LoadDataScene::createScene()));

}