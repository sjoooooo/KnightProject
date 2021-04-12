#ifndef _SPLASH_SCENE_H_
#define _SPLASH_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class SplashScene : public cocos2d::Layer
{
private:
	Size visibleSize;
	Vec2 origin;

	enum TAG
	{
		SPLASH_TIME = 0,
	};
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();

	bool nextScene;

	CREATE_FUNC(SplashScene);
};

#endif 
