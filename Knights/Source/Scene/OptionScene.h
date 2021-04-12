#ifndef _OPTION_SCENE_H_
#define _OPTION_SCENE_H_

#include "cocos2d.h"
#include "ui\CocosGUI.h"
USING_NS_CC;

class OptionScene : public cocos2d::Layer
{
private:
	Size visibleSize;
	Vec2 origin;
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(OptionScene);

	void BackGroundSoundControl(Ref *sender, ui::Slider::EventType type);
	void EffectSoundControl(Ref *sender, ui::Slider::EventType type);
	void menuCloseCallback(cocos2d::Ref* pSender);
	void InitBackGround();
	void InitMenu();
};
#endif 
