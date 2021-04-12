#include "WaveInfo.h"
#include "..\Units\Unit.h"

WaveInfo::WaveInfo()
	: m_unitType(NULL)
	, m_amount(NULL)
{
	m_unitType = Sprite::create();
	m_unitType->setFlippedX(true);
	this->addChild(m_unitType, 0);
}

WaveInfo::~WaveInfo()
{
	if (m_unitType)
		delete m_unitType;
	if (m_amount)
		delete m_amount;
}

// Setters
void WaveInfo::SetWaveInfo(int type, Size size, int amount)
{
	if (!m_amount)
	{
		m_amount = Label::create("", "MiniGameResources/CuteFont.ttf", size.height * 0.3);
		this->addChild(m_amount, 1);
	}
	switch (type)
	{
		case Unit::SWORDSMAN:
		{
			m_unitType->setTexture("CharacterAnimations/stop_sd.png");
			break;
		}
		case Unit::ARCHER:
		{
			m_unitType->setTexture("CharacterAnimations/stop_sd.png");
			break;
		}
		case Unit::HORSEMAN:
		{
			m_unitType->setTexture("CharacterAnimations/stop_sd.png");
			break;
		}
		case Unit::SPEARMAN:
		{
			m_unitType->setTexture("CharacterAnimations/stop_sd.png");
			break;
		}
		case Unit::WIZARD:
		{
			m_unitType->setTexture("CharacterAnimations/stop_sd.png");
			break;
		}
		case Unit::SHIELDMAN:
		{
			m_unitType->setTexture("CharacterAnimations/stop_sd.png");
			break;
		}
	}
	m_unitType->setScale(	size.width / m_unitType->getContentSize().width,
							(size.height * 0.7f) / m_unitType->getContentSize().height);
	m_unitType->setPositionY(-size.height * 0.15f);
	m_amount->setString(StringUtils::format("%d", amount));
	m_amount->setPositionY(size.height * 0.3f);
}
// Getters

// Others
void WaveInfo::Clear()
{
	m_unitType->setTexture("");
	if (m_amount)
	{
		m_amount->setString("");
	}
}