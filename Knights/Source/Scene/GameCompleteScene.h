#ifndef _GAME_COMPLETE_SCENE_H_
#define _GAME_COMPLETE_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class GameCompleteScene : public cocos2d::Layer
{
private:
	Size visibleSize;
	Vec2 origin;

	void NextScene(Ref* pSender);
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

	void InitBackGround();

	CREATE_FUNC(GameCompleteScene);
};

#endif 
