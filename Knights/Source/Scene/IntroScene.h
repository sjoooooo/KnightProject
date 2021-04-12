#ifndef __INTRO_SCENE_H__
#define __INTRO_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class IntroScene : public cocos2d::Layer
{
private:
	Size visibleSize;
	Vec2 origin;
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();
	void InitLabel();
	void InitMenu();
	void Tick(float delta);
	void menuCloseCallback(cocos2d::Ref* pSender);
	void menuStartCallback(cocos2d::Ref* pSender);
	void menuOptionCallback(cocos2d::Ref* pSender);

	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	//virtual void onTouchMoved(Touch *touch, Event *event);
	//virtual void onTouchEnded(Touch *touch, Event *event);
	CREATE_FUNC(IntroScene);
};

#endif 
