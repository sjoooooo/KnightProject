#ifndef _DATA_SELECTION_SCENE_H_
#define _DATA_SELECTION_SCENE_H_

#include "cocos2d.h"
USING_NS_CC;

class DataSelectionScene : public cocos2d::Layer
{
private:
	enum TAG
	{
		DATA_1 = 1,
		DATA_2,
		DATA_3,
		MENU
	};
	Size visibleSize;
	Vec2 origin;

	void SelectData(int DataNo);

	void Data1(Ref* pSender);
	void Data2(Ref* pSender);
	void Data3(Ref* pSender);
	void PrevScene(Ref* pSender);
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void update(float delta) override;

	void InitBackGround();
	void InitMenu();

	CREATE_FUNC(DataSelectionScene);
};

#endif 
