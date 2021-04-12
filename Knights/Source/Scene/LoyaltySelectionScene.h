#ifndef _LOYALTY_SELECTION_SCENE_H_
#define _LOYALTY_SELECTION_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class LoyaltySelectionScene : public cocos2d::Layer
{
private:
	Size visibleSize;
	Vec2 origin;

	void Loyal(Ref* pSender);
	void Betray(Ref* pSender);
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();
	void InitMenu();

	CREATE_FUNC(LoyaltySelectionScene);
};

#endif 
