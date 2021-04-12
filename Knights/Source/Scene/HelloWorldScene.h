#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui\CocosGUI.h"
USING_NS_CC;


class HelloWorld : public cocos2d::Layer
{
private:
	Size visibleSize;
	Vec2 origin;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(HelloWorld);

	void sliderEvent(Ref *sender, ui::Slider::EventType type);
};
enum TAG
{
	SLIDER,
	SLIDER1
};
#endif 
