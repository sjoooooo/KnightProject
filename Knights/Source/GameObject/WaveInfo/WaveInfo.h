#ifndef _WAVE_INFO_H_
#define _WAVE_INFO_H_

#include "cocos2d.h"
USING_NS_CC;

class WaveInfo : public Node
{
private:
	// Unit Type
	Sprite* m_unitType;
	Label* m_amount;
public:
	WaveInfo();
	~WaveInfo();

	// Setters
	void SetWaveInfo(int type, Size size, int amount);

	// Getters

	// Others
	void Clear();
};

#endif 
