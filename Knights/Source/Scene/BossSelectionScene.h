#ifndef _BOSS_SELECTION_SCENE_H_
#define _BOSS_SELECTION_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class BossSelectionScene : public cocos2d::Layer
{
private:
	Size visibleSize;
	Vec2 origin;

	void Recruit(Ref* pSender);
	void Execute(Ref* pSender);
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();
	void InitMenu();

	CREATE_FUNC(BossSelectionScene);
};

#endif 
