#include "SkillEffectManager.h"

SkillEffectManager::SkillEffectManager()
{
}

SkillEffectManager::~SkillEffectManager()
{
}

// Setters
void SkillEffectManager::AddSkillEffect(SkillEffect* skillEffect)
{
	m_skillEffectList.push_back(skillEffect);
}

// Getters
std::vector<SkillEffect*>* SkillEffectManager::GetSkillEffectList()
{
	return &m_skillEffectList;
}
SkillEffect* SkillEffectManager::GetInactiveSkillEffect()
{
	// Skill effect
	for (SkillEffect* skillEffect : m_skillEffectList)
	{
		if (!skillEffect->IfActive())
		{
			return skillEffect;
		}
	}
}

// Others
void SkillEffectManager::Update(float dt)
{
	// Skill effect
	for (SkillEffect* skillEffect : m_skillEffectList)
	{
		if (skillEffect->IfActive())
		{
			if (skillEffect->GetSprite()->getNumberOfRunningActions() == 0)
			{
				skillEffect->Stop();
			}
		}
	}
}