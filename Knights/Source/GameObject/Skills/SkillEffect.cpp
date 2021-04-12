#include "SkillEffect.h"
#include "../../Managers/AnimationManager.h"
#include "Skill.h"
#include "../../Scene/StatScene.h"
#include "..\Item\Item.h"

SkillEffect::SkillEffect()
	: m_sprite(NULL)
	, m_active(false)
	, m_timeToDamage(0.0f)
	, m_damage(0)
{
}

SkillEffect::~SkillEffect()
{
	if (m_sprite)
		delete m_sprite;
}

// Setters
void SkillEffect::Init()
{
	m_sprite = Sprite::create();
	m_sprite->setAnchorPoint(Point(0.5f, 0.0f));
	this->addChild(m_sprite, 0);
}
void SkillEffect::SetActive(bool active)
{
	m_active = active;
	this->setVisible(m_active);
}
void SkillEffect::SetTimeToDamage(float timeToDamage)
{
	m_timeToDamage = timeToDamage;
}
void SkillEffect::SetDamage(int damage)
{
	m_damage = damage;
}

// Getters
Sprite* SkillEffect::GetSprite()
{
	return m_sprite;
}
bool SkillEffect::IfActive()
{
	return m_active;
}
float SkillEffect::GetTimeToDamage()
{
	return m_timeToDamage;
}
int SkillEffect::GetDamage()
{
	return m_damage;
}

void SkillEffect::Stop()
{
	m_sprite->stopAllActions();
	this->stopAllActions();
	this->SetActive(false);
}

// Others
void SkillEffect::Activate(Vec2 targetPosition, Size size)
{
	UserDefault* def = UserDefault::getInstance();
	int currentData = def->getIntegerForKey("CurrentData", 1);
	int strPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::STR_PLUS_BUTTON)).c_str(), 0);
	int intPoints = def->getIntegerForKey(("Data" + StringUtils::format("%d", currentData) + "_Stat" + StringUtils::format("%d", StatScene::INT_PLUS_BUTTON)).c_str(), 0);
	// Set Active
	this->SetActive(true);
	switch (this->GetSprite()->getTag())
	{
		case Skill::ARROW:
		{
			// Duration
			float duration = 1.f;
			float delayDuration = 1.f;
			m_sprite->setTexture("SkillImage/arrow/arrow-1.png");
			// Get Animation
			auto animation = Sequence::create(DelayTime::create(delayDuration), AnimationManager::GetSkillAnimation(this->GetSprite()->getTag(), duration), NULL);
			animation->setTag(ANIMATION);
			m_sprite->runAction(animation);
			// Set attack frame
			m_timeToDamage = delayDuration + duration * 0.9f;
			// Set attack damage
			m_damage = 40 + strPoints * 5;
			// Set Postion Y up abit
			this->setPositionY(this->getPositionY() + size.height);
			// Bezier To
			ccBezierConfig bezierConfig;
			float distanceX = targetPosition.x - this->getPositionX();
			bezierConfig.controlPoint_1 = Point(this->getPositionX() + distanceX * 0.4, this->getPositionY() + size.height * 1.5f);
			bezierConfig.controlPoint_2 = Point(this->getPositionX() + distanceX * 0.7, this->getPositionY() + size.height * 1.5f);
			bezierConfig.endPosition = targetPosition;
			auto bezierTo = BezierTo::create(duration * 11.f / 12.f, bezierConfig);
			auto action = Sequence::create(DelayTime::create(delayDuration), bezierTo, NULL);
			action->setTag(MOVE);
			this->runAction(action);
			break;
		}
		case Skill::LIGHTNING:
		{
			// Duration
			float duration = 1.f;
			float delayDuration = 0.f;
			m_sprite->setTexture("SkillImage/lightning/lightning-1.png");
			// Get Animation
			auto animation = Sequence::create(DelayTime::create(delayDuration), AnimationManager::GetSkillAnimation(this->GetSprite()->getTag(), duration), NULL);
			animation->setTag(ANIMATION);
			m_sprite->runAction(animation);
			// Set attack frame
			m_timeToDamage = delayDuration + duration * 0.9f;
			// Set attack damage
			m_damage = 80 + intPoints * 5;
			// Set Postion Y up abit
			this->setPosition(targetPosition);
			break;
		}
		case Skill::METEOR:
		{
			// Duration
			float duration = 1.f;
			float delayDuration = 2.f;
			m_sprite->setTexture("SkillImage/meteor/meteor-1.png");
			// Get Animation
			auto animation = Sequence::create(DelayTime::create(delayDuration), AnimationManager::GetSkillAnimation(this->GetSprite()->getTag(), duration), NULL);
			animation->setTag(ANIMATION);
			m_sprite->runAction(animation);
			// Set attack frame
			m_timeToDamage = delayDuration + duration * 0.7f;
			// Set attack damage
			m_damage = 90 + intPoints * 10;
			// Set Postion Y up abit
			this->setPositionY(this->getPositionY() + size.height * 5);
			this->setPositionZ(this->getPositionZ() - size.width * 5);
			// Move To
			auto moveTo = MoveTo::create(duration * 5.f / 8.f, targetPosition);
			auto action = Sequence::create(DelayTime::create(delayDuration), moveTo, NULL);
			action->setTag(MOVE);
			this->runAction(action);
			break;
		}
		case Skill::HURRICANE:
		{
			// Duration
			float duration = 1.f;
			float delayDuration = 0.f;
			m_sprite->setTexture("SkillImage/hurricane/hurricane-1.png");
			// Get Animation
			auto animation = Sequence::create(DelayTime::create(delayDuration), AnimationManager::GetSkillAnimation(this->GetSprite()->getTag(), duration), NULL);
			animation->setTag(ANIMATION);
			m_sprite->runAction(animation);
			// Set attack frame
			m_timeToDamage = 0.f;
			// Set attack damage
			m_damage = 60 + intPoints * 10;
			// Set Postion
			this->setPosition(targetPosition);
			break;
		}
		case Skill::ICE_SPEAR:
		{
			// Duration
			float duration = 0.5f;
			float delayDuration = 0.f;
			m_sprite->setTexture("SkillImage/ice_spear/ice_spear-1.png");
			// Get Animation
			auto animation = Sequence::create(DelayTime::create(delayDuration), AnimationManager::GetSkillAnimation(this->GetSprite()->getTag(), duration), NULL);
			animation->setTag(ANIMATION);
			m_sprite->runAction(animation);
			// Set attack frame
			m_timeToDamage = 0.01f;
			// Set attack damage
			m_damage = 100 + intPoints * 20;
			// Move To
			auto moveTo = MoveTo::create(duration, targetPosition);
			auto action = Sequence::create(DelayTime::create(delayDuration), moveTo, NULL);
			action->setTag(MOVE);
			this->runAction(action);
			break;
		}
		case Skill::FIREBALL:
		{
			// Duration
			float duration = 0.5f;
			float delayDuration = 0.f;
			m_sprite->setTexture("SkillImage/fire_ball/fire_ball-1.png");
			// Get Animation
			auto animation = Sequence::create(DelayTime::create(delayDuration), AnimationManager::GetSkillAnimation(this->GetSprite()->getTag(), duration), NULL);
			animation->setTag(ANIMATION);
			m_sprite->runAction(animation);
			// Set attack frame
			m_timeToDamage = 0.01f;
			// Set attack damage
			m_damage = 90 + intPoints * 15;
			// Move To
			auto moveTo = MoveTo::create(duration, targetPosition);
			auto action = Sequence::create(DelayTime::create(delayDuration), moveTo, NULL);
			action->setTag(MOVE);
			this->runAction(action);
			break;
		}
	}
	// Set Scale
	m_sprite->setScale(	size.width / m_sprite->getContentSize().width,
						size.height / m_sprite->getContentSize().height);

	// Item increased damage
	if (def->getBoolForKey(("Data" + StringUtils::format("%d", currentData) + "_ItemSelected" + StringUtils::format("%d", this->GetSprite()->getTag())).c_str(), false))
	{
		m_damage += Item::GetItemEffect_Damage(this->GetSprite()->getTag());
	}
}