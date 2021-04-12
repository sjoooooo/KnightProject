#ifndef _STATE_SCENE_H_
#define _STATE_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class StatScene : public cocos2d::Layer
{
public:
	enum TAG
	{
		STR_PLUS_BUTTON = 1,
		HP_PLUS_BUTTON,
		DEF_PLUS_BUTTON,
		RECOVERY_PLUS_BUTTON,
		LUCK_PLUS_BUTTON,
		PRC_PLUS_BUTTON,
		INT_PLUS_BUTTON,
		NUM_OF_PLUS_BUTTONS,

		STR_POINTS,
		HP_POINTS,
		DEF_POINTS,
		RECOVERY_POINTS,
		LUCK_POINTS,
		PRC_POINTS,
		INT_POINTS,
		NUM_OF_POINTS,

		POINTS_LEFT_LABEL,

		FIRST_TIME_TUTORIAL,

		GUI,
		MENU,
		PLAY_BUTTON,
		MAINMENU_BUTTON,

		STAT_SCENE_TAB,
		STORE_SCENE_TAB,
		SKILL_SELECTION_SCENE_TAB,
		ALLIES_SELECTION_SCENE_TAB,
	};
private:
	Size visibleSize;
	Vec2 origin;

	float fontSize;
	int pointsLeft;
	int unsavedPointsLeft;
	bool skillsSelectionSceneCompleted;

	int stat[NUM_OF_PLUS_BUTTONS - STR_PLUS_BUTTON];

	void InitBackGround();
	void InitGUI();
	void InitFirstTimeTutorial();
	void InitTabs(Vector<MenuItem*>* MenuItems);

	void IncreasePoint(int plusButtonTag);
	void CheckAndLockAllPlusButtons();
	void CheckAndUnLockPlayButton();
	void CheckSkillsSelectionCompleted();

	void ClickFirstTime(Ref* pSender);

	void ClickPlayButton(Ref* pSender);
	void ClickMainMenuButton(Ref* pSender);

	void ClickStrPlusButton(Ref* pSender);
	void ClickHpPlusButton(Ref* pSender);
	void ClickDefPlusButton(Ref* pSender);
	void ClickRecoveryPlusButton(Ref* pSender);
	void ClickLuckPlusButton(Ref* pSender);
	void ClickPrcPlusButton(Ref* pSender);
	void ClickIntPlusButton(Ref* pSender);

	void TempSaveStatPoints();
	void ClickStoreSceneTab(Ref* pSender);
	void ClickSkillSelectionSceneTab(Ref* pSender);
	void ClickAlliesSelectionSceneTab(Ref* pSender);
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	static void DeleteTempSaveStatPoints(bool saveUnsavedpoints = true);
	static void SaveStatPoints();

	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	//virtual void onTouchMoved(Touch *touch, Event *event);
	//virtual void onTouchEnded(Touch *touch, Event *event);
	CREATE_FUNC(StatScene);
};

#endif 
