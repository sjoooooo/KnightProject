#ifndef __SKILL_SELECTION_SCENE_H__
#define __SKILL_SELECTION_SCENE_H__

#include "cocos2d.h"
USING_NS_CC;

class SkillSelectionScene : public cocos2d::Layer
{
public:
	enum TAG
	{
		GUI = 1,
		MENU,
		INNER_MENU,
		PLAY_BUTTON,
		MAINMENU_BUTTON,

		STAT_SCENE_TAB,
		STORE_SCENE_TAB,
		SKILL_SELECTION_SCENE_TAB,
		ALLIES_SELECTION_SCENE_TAB,
				
		FIRST_TIME_TUTORIAL,

		NUM_OF_EQUIPPED,
		EQUIP_BUTTON,
		CLEAR_BUTTON,
		BUTTON=100,

		TOTAL_NUM_OF_SKILLS = 15,
	};
private:

	Size visibleSize;
	Size innerVisibleSize;
	Vec2 origin;
	Vec2 innerMidpointOffset;
	Vec2 outerOffset;
	Vec2 midPoint;

	Sprite* currentbutton;
	Sprite* currentimage;
	Sprite* equip_button;
	Sprite* return_button;
	int numOfEquipedskills;
	bool selected[TOTAL_NUM_OF_SKILLS];
	bool nextSelected;

	bool statSceneCompleted;

	void SaveData();

	void CheckAndUnLockPlayButton();
	void CheckStatCompleted();

	void ClickPlayButton(Ref* pSender);
	void ClickMainMenuButton(Ref* pSender);

	void ClickAlliesSelectionSceneTab(Ref* pSender);
	void ClickStatSceneSceneTab(Ref* pSender);
	void ClickStoreSceneTab(Ref* pSender);
public:
    static cocos2d::Scene* createScene();
	cocos2d::Vector<cocos2d::Sprite*> mpData;
	
    virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();
	void InitLabel();
	void InitMenu();
	void InitFirstTimeTutorial();
	void InitTabs(Vector<MenuItem*>* MenuItems);

	void ClickFirstTime(Ref* pSender);
	
	
	
	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onTouchEnded(Touch* touch, Event* unused_event);
	
	
	
	
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	
	CREATE_FUNC(SkillSelectionScene);
	
	
};


#endif 
