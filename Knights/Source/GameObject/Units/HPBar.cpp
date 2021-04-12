#include "HPBar.h"

HPBar::HPBar()
	: m_background(NULL)
	, m_hpbar(NULL)
	, m_outline(NULL)
{
}

HPBar::~HPBar()
{
	if (m_background)
		delete m_background;
	if (m_hpbar)
		delete m_hpbar;
	if (m_outline)
		delete m_outline;
}


// Setters
void HPBar::Init(std::string background, std::string hpbar, std::string outline)
{
	if (!m_background)
	{
		m_background = Sprite::create(background);
		this->addChild(m_background, 1);
	}
	

	if (!m_hpbar)
	{
		m_hpbar = Sprite::create(hpbar);
		this->addChild(m_hpbar, 2);
	}
	

	if (!m_outline)
	{
		m_outline = Sprite::create(outline);
		this->addChild(m_outline, 0);
	}
	
}
void HPBar::Resize(Size size, bool decreasingTowardLeft, float outlineSize)
{
	m_size = size;

	if (m_background->getTexture())
	{
		m_background->setScale(	m_size.width / m_background->getContentSize().width,
								m_size.height / m_background->getContentSize().height);
	}
	if (m_hpbar->getTexture())
	{
		if (decreasingTowardLeft)
		{
			m_hpbar->setAnchorPoint(Vec2(0.0f, 0.5f));
			m_hpbar->setPositionX(-m_size.width * 0.5f);
		}
		else
		{
			m_hpbar->setAnchorPoint(Vec2(1.0f, 0.5f));
			m_hpbar->setPositionX(m_size.width * 0.5f);
		}
		m_hpbar->setScale(	m_size.width / m_hpbar->getContentSize().width,
							m_size.height / m_hpbar->getContentSize().height);
	}
	if (m_outline->getTexture())
	{
		m_outline->setScale((m_size.width + m_size.height * outlineSize) / m_outline->getContentSize().width,
							(m_size.height + m_size.height * outlineSize) / m_outline->getContentSize().height);
	}
}
void HPBar::SetHealth(int health, int maxHealth)
{
	if (m_hpbar)
	{
		if (m_hpbar->getTexture())
		{
			m_hpbar->setScaleX((((float)health / (float)maxHealth) * m_size.width) / m_hpbar->getContentSize().width);
		}
	}
}

// Getters

// Others