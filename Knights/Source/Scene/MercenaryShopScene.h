#ifndef _MERCENARY_SHOP_SCENE_H_
#define _MERCENARY_SHOP_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class MercenaryShopScene : public cocos2d::Layer
{
public:
	enum TAG
	{
		SWORDSMAN = 1,
		ARCHER,
		SPEARMAN,

		HORSEMAN,
		SHIELDMAN,
		WIZARD,

		NUM_OF_MERCENARIES,
		AMOUNT,
		GUI,
		MENU,
		GOLD_LEFT_LABEL,
	};
private:
	Size visibleSize;
	Vec2 origin;

	int AmountOfMercenaries[NUM_OF_MERCENARIES - SWORDSMAN];
	int CostOfMercenaries[NUM_OF_MERCENARIES - SWORDSMAN];
	int goldLeft;

	void InitBackGround();
	void InitGUI();

	void IncreasePoint(Sprite* plusButton);

	void CheckAndLockAllPlusButton();
	void CheckAndLockPlusButton(Sprite* plusButton);

	void LockPlusButtons(Sprite* plusButton);

	void NextScene(Ref* pSender);
	void PrevScene(Ref* pSender);
public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float delta) override;


	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	//virtual void onTouchMoved(Touch *touch, Event *event);
	//virtual void onTouchEnded(Touch *touch, Event *event);
	CREATE_FUNC(MercenaryShopScene);
};

#endif 
