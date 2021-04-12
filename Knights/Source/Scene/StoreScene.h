#ifndef _STORE_SCENE_H_
#define _STORE_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class StoreScene : public cocos2d::Layer
{
public:
	enum TAG
	{
		GUI,
		MENU,
		PLAY_BUTTON,
		MAINMENU_BUTTON,

		STAT_SCENE_TAB,
		STORE_SCENE_TAB,
		SKILL_SELECTION_SCENE_TAB,
		ALLIES_SELECTION_SCENE_TAB,

		NUM_OF_GOLD,
		BUY_BUTTON,

		TOTAL_NUM_OF_ITEMS = 15,

		BUTTON = 100,
	};
private:
	/* Member variables */
	Size visibleSize;
	Size innerVisibleSize;
	Vec2 origin;
	Vec2 innerMidpointOffset;
	Vec2 outerOffset;
	Vec2 midPoint;

	Sprite* currentbutton;
	Sprite* currentimage;
	Sprite* buy_button;

	int numOfEquipedItems;
	int goldLeft;
	int price[15];

	bool selected[TOTAL_NUM_OF_ITEMS];
	bool nextSelected;

	bool statSceneCompleted;
	bool skillsSelectionSceneCompleted;

	/* Functions */
	void SaveData();
	void CheckAndUnLockPlayButton();
	void CheckSkillsSelectionCompleted();
	void CheckStatCompleted();

	void ClickPlayButton(Ref* pSender);
	void ClickMainMenuButton(Ref* pSender);

	void ClickStatSceneSceneTab(Ref* pSender);
	void ClickAlliesSelectionSceneTab(Ref* pSender);
	void ClickSkillSelectionSceneTab(Ref* pSender);
public:
    static cocos2d::Scene* createScene();
	cocos2d::Vector<cocos2d::Sprite*> mpData;
	
    virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();
	void InitMenu();
	void InitTabs(Vector<MenuItem*>* MenuItems);
	
	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onTouchEnded(Touch* touch, Event* unused_event);
	
	CREATE_FUNC(StoreScene);
};


#endif 
