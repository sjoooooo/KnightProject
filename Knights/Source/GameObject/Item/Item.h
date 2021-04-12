#ifndef _ITEM_H_
#define _ITEM_H_

#include "..\Units\Unit.h"

#include "cocos2d.h"
USING_NS_CC;

class Item
{
public:
	Item();
	~Item();
	

	// Others
	static void SetItemEffect_Info(int type, UnitInfo* info);
	static void SetItemEffect_BonusInfo(int type, UnitInfoBonus* info);
	static int GetItemEffect_Damage(int type);
	static int GetItemEffect_Heal(int type);
	static float GetItemEffect_Cooldown(int type);
	static float GetItemEffect_Gauge(int type);
	static int GetPrice(int type);
};

#endif 
