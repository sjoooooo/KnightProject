#include "Hero.h"

#include "../Spawner/Spawner.h"

Hero::Hero()
	: Unit()
	, m_movingRight(true)
{
}

Hero::~Hero()
{
}

// Setters
void Hero::SetMovingRight(bool movingRight)
{
	m_movingRight = movingRight;
}

// Getters
bool Hero::IfMovingRight()
{
	return m_movingRight;
}

// Others
void Hero::Update(float dt)
{
	switch (m_currentState)
	{
		// IDLING
		case IDLING:
		{
			// if no idling action,
			if (!this->getActionByTag(IDLING))
			{
				// Continue to move to target
				MoveToTarget();
			}
			break;
		}
		// MOVING
		case MOVING:
		{
			// if no moving Action
			if (!this->getActionByTag(MOVING))
			{
				// BackStep
				if (this->getPosition() != m_targetLocation)
				{
					m_movingRight = true;
					Idle(1.0f);
				}
			}
			break;
		}
	}
	Unit::Update(dt);
}