#ifndef _LOAD_DATA_SCENE_H_
#define _LOAD_DATA_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class LoadDataScene : public cocos2d::Layer
{
private:
	Size visibleSize;
	Vec2 origin;

	int currentData;

	void Load(Ref* pSender);
	void Reset(Ref* pSender);
	void PrevScene(Ref* pSender);
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();
	void InitMenu();

	CREATE_FUNC(LoadDataScene);
};

#endif 
