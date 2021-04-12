#include "Skillbar.h"

Skillbar::Skillbar()
	: m_background(NULL)
	, m_currentOffsetX(0.0f)
	, m_maxOffsetX(0.0f)
{
}

Skillbar::~Skillbar()
{
	if (m_background)
		delete m_background;
	for (int i = 0; i < m_skillList.size(); i++)
	{
		if (m_skillList[i])
			delete m_skillList[i];
	}
}

// Setters
void Skillbar::SetCurrentOffsetX(float currentOffsetX)
{
	m_currentOffsetX = currentOffsetX;
}
void Skillbar::SetMaxOffsetX(float maxOffsetX)
{
	m_maxOffsetX = maxOffsetX;
}
// Getters
float Skillbar::GetCurrentOffsetX()
{
	return m_currentOffsetX;
}
float Skillbar::GetMaxOffsetX()
{
	return m_maxOffsetX;
}
Sprite* Skillbar::GetBackground()
{
	return m_background;
}

// Others
void Skillbar::Init(Size* skillBarSize, Vec2* Position, float spacing)
{
	// Background
	m_background = Sprite::create("Background/skillbar_background.png");
	m_background->setScale(	skillBarSize->width / m_background->getContentSize().width,
							skillBarSize->height / m_background->getContentSize().height);
	m_background->setPosition(*Position);
	this->addChild(m_background, 0);

	// Skills
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	int currentIndex = 0;
	bool skillSelected[Skill::NUM_OF_TYPES];
	for (int i = 0; i < Skill::NUM_OF_TYPES; i++)
	{
		std::stringstream ss1;
		ss1 << "Data" << currentData << "_SkillSelected" << i;
		skillSelected[i] = def->getBoolForKey(ss1.str().c_str()); // Default returns false
	}
	for (int i = 0; i < Skill::NUM_OF_TYPES; i++)
	{
		if (skillSelected[i])
		{
			std::stringstream ss2;
			ss2 << "SkillImage/skill" << i << ".png";
			m_skillList.push_back((Skill*)ProgressTimer::create(Sprite::create(ss2.str().c_str())));

			Skill* skill = m_skillList.back();
			skill->setType(ProgressTimer::Type::RADIAL);
			skill->setPercentage(100);
			skill->CalculateInfo(i);
			skill->setScale(	skillBarSize->height * 0.8f / skill->getContentSize().width,
								skillBarSize->height * 0.8f / skill->getContentSize().height);
			// Get Left end position X of skillbar
			float positionX = Position->x - skillBarSize->width * 0.5f;
			// Add Half the size of skill_sprite
			positionX += skillBarSize->height * 0.5f + spacing;
			// Add the size of skill_sprite depending of i
			positionX += currentIndex * (skillBarSize->height + spacing);

			skill->setPosition(positionX, Position->y);

			this->addChild(skill, 2);

			std::stringstream ss3;
			ss3 << "SkillImage/skill" << i << ".png";
			auto skillBackground = Sprite::create(ss3.str().c_str());
			skillBackground->setColor(Color3B::GRAY);
			skillBackground->setScale(	skillBarSize->height * 0.8f / skill->getContentSize().width,
										skillBarSize->height * 0.8f / skill->getContentSize().height);
			skillBackground->setPosition(positionX, Position->y);
			this->addChild(skillBackground, 1);

			currentIndex++;
		}
	}

	m_maxOffsetX = (Position->x + skillBarSize->width * 0.5f - skillBarSize->height * 0.5f - spacing) - m_skillList.back()->getPositionX();
}

void Skillbar::MoveSkillsX(float moveby)
{
	m_currentOffsetX += moveby;
	for (int i = 0; i < m_skillList.size(); i++)
	{
		m_skillList[i]->setPositionX(m_skillList[i]->getPositionX() + moveby);
	}
}
void Skillbar::ContraintsMovedSkillsX()
{
	if (m_currentOffsetX > 0)
	{
		for (int i = 0; i < m_skillList.size(); i++)
		{
			auto action = MoveBy::create(0.25f, Vec2(-m_currentOffsetX, 0));
			m_skillList[i]->runAction(action);
		}
		m_currentOffsetX = 0;
	}
	else if (m_currentOffsetX < m_maxOffsetX)
	{
		for (int i = 0; i < m_skillList.size(); i++)
		{
			auto action = MoveBy::create(0.25f, Vec2(-(m_currentOffsetX - m_maxOffsetX), 0));
			m_skillList[i]->runAction(action);
		}
		m_currentOffsetX = m_maxOffsetX;
	}
}
bool Skillbar::IfSkillsMoving()
{
	return m_skillList.back()->getNumberOfRunningActions() != 0;
}
Skill* Skillbar::TouchedSkill(Point* location)
{
	for (int i = 0; i < m_skillList.size(); i++)
	{
		// if not on cool down
		if (m_skillList[i]->getPercentage() == 100.f)
		{
			// if touched skill
			if (m_skillList[i]->getBoundingBox().containsPoint(*location))
			{
				return m_skillList[i];
			}
		}
	}
	return NULL;
}