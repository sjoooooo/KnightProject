#ifndef _HERO_H_
#define _HERO_H_

#include "Unit.h"

#include "cocos2d.h"
USING_NS_CC;

class Hero : public Unit
{
private:
	bool m_movingRight;
public:
	Hero();
	virtual ~Hero();

	// Setters
	void SetMovingRight(bool movingRight);

	// Getters
	bool IfMovingRight();

	// Others
	void Update(float dt);
};

#endif 
