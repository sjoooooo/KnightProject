#ifndef _HP_BAR_H_
#define _HP_BAR_H_

#include "cocos2d.h"
USING_NS_CC;

class HPBar : public Node
{
private:
	// Background sprite
	Sprite* m_background;
	// Hp Bar sprite
	Sprite* m_hpbar;
	// Outline sprite
	Sprite* m_outline;
	// Size
	Size m_size;
public:
	HPBar();
	~HPBar();

	// Setters
	void Init(std::string background, std::string hpbar, std::string outline);
	void Resize(Size size, bool decreasingTowardLeft = true, float outlineSize = 0.2f);
	void SetHealth(int health, int maxHealth);

	// Getters

	// Others
};

#endif 
