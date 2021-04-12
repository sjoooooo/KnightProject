#ifndef _ALLIES_SELECTION_SCENE_H_
#define _ALLIES_SELECTION_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class AlliesSelectionScene : public cocos2d::Layer
{
private:
	enum TAG
	{
		NUM_OF_EQUIPPED_LABEL = 1,
		CHOICE,
		LOCKED,
		UNLOCK,
		SELECT,
		EQUIP,
		UNEQUIP,
		NEXT_SET,
		PREV_SET,

		GUI,
		MENU,
		INNER_MENU,
		PLAY_BUTTON,
		MAINMENU_BUTTON,

		STAT_SCENE_TAB,
		STORE_SCENE_TAB,
		SKILL_SELECTION_SCENE_TAB,
		ALLIES_SELECTION_SCENE_TAB,

		ALLY,
		
	};
public:
	static const int totalNumOfAllies = 6;
	static const int maxNumOfSelectedAllies = 3;
private:
	/* Member variables */
	Size visibleSize;
	Size innerVisibleSize;
	Vec2 origin;
	Vec2 innerMidpointOffset;
	Vec2 outerOffset;
	Vec2 midPoint;

	Sprite* currentAlly;

	int numOfEquippedAllies;
	int numOfAvailableAllies;
	bool selectedType[totalNumOfAllies];
	bool selectable[totalNumOfAllies];

	int numOfPages;
	int currentPage;

	bool statSceneCompleted;
	bool skillsSelectionSceneCompleted;

	/* Functions */
	void SaveData();
	void UnselectCurrentAlly();

	void Equip(Ref* pSender);
	void Unequip(Ref* pSender);
	void NextSet(Ref* pSender);
	void PrevSet(Ref* pSender);

	void CheckAndUnLockPlayButton();
	void CheckSkillsSelectionCompleted();
	void CheckStatCompleted();

	void ClickPlayButton(Ref* pSender);
	void ClickMainMenuButton(Ref* pSender);

	void ClickStatSceneSceneTab(Ref* pSender);
	void ClickStoreSceneTab(Ref* pSender);
	void ClickSkillSelectionSceneTab(Ref* pSender);

	void SetTexture(Sprite* spr, int state); 
public:
    static cocos2d::Scene* createScene();
	cocos2d::Vector<cocos2d::Sprite*> mpData;
	
    virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();
	void InitAllies();
	void InitMenu();
	void InitTabs(Vector<MenuItem*>* MenuItems);
	
	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onTouchEnded(Touch* touch, Event* unused_event);
	
	CREATE_FUNC(AlliesSelectionScene);
};


#endif 
