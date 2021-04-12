#include "IncomingWavesInfo.h"

IncomingWavesInfo::IncomingWavesInfo()
	: m_currentWaveInfo(NULL)
	, m_background(NULL)
	, m_currentwaveinfoBox(NULL)
	, m_AISpawner(NULL)
	, m_size(0, 0)
	, m_currentWave(0)
{
}

IncomingWavesInfo::~IncomingWavesInfo()
{
	if (m_currentWaveInfo)
		delete m_currentWaveInfo;
	if (m_nextWaveInfo)
		delete m_nextWaveInfo;
	if (m_background)
		delete m_background;
	if (m_currentwaveinfoBox)
		delete m_currentwaveinfoBox;
	if (m_AISpawner)
		delete m_AISpawner;
}

// Setters

// Getters
Size IncomingWavesInfo::GetSize()
{
	return m_size;
}

// Others
void IncomingWavesInfo::Init(AISpawner* AISpawner, Size* size)
{
	// Background
	m_background = Sprite::create("CocosResources/overWatch.jpg");
	m_background->setScale(	size->width / m_background->getContentSize().width,
							size->height / m_background->getContentSize().height);
	this->addChild(m_background, 0);

	m_AISpawner = AISpawner;
	m_size = *size;

	Size waveInfoSize(m_size.width* 0.25f, m_size.height);

	// Current Wave Info Box 
	m_currentwaveinfoBox = Sprite::create("lightred_rect.png");
	m_currentwaveinfoBox->setScale(waveInfoSize.width / m_currentwaveinfoBox->getContentSize().width, waveInfoSize.height / m_currentwaveinfoBox->getContentSize().height);
	m_currentwaveinfoBox->setPositionX(-size->width * 0.5f + waveInfoSize.width * 0.5f);
	this->addChild(m_currentwaveinfoBox, 2);

	// Current Wave Info
	m_currentWaveInfo = new WaveInfo;
	m_currentWaveInfo->setPositionX(-size->width * 0.5f + waveInfoSize.width * 0.5f);
	this->addChild(m_currentWaveInfo, 1);

	// Next wave
	m_nextWaveInfo = new WaveInfo;
	m_nextWaveInfo->setPositionX(size->width * 0.5f - waveInfoSize.width * 0.5f);
	this->addChild(m_nextWaveInfo, 1);
	SpawnPattern* nextSP = m_AISpawner->GetSpawnPattern(m_currentWave);
	if (nextSP)
	{
		m_nextWaveInfo->SetWaveInfo(nextSP->GetType(), waveInfoSize, nextSP->GetAmountToSpawn());
		auto moveTo = MoveTo::create(nextSP->GetTimeToSpawn(), m_currentWaveInfo->getPosition());
		m_nextWaveInfo->runAction(moveTo);
	}

	m_currentWave = m_AISpawner->GetCurrentSpawnPatternIndex();
}

void IncomingWavesInfo::Update(float dt)
{
	if (m_currentWave != m_AISpawner->GetCurrentSpawnPatternIndex())
	{
		// Current Wave Info
		SpawnPattern* currentSP = m_AISpawner->GetSpawnPattern(m_currentWave);
		if (currentSP)
		{
			Size waveInfoSize(m_size.width* 0.25f, m_size.height);
			m_currentWaveInfo->SetWaveInfo(currentSP->GetType(), waveInfoSize, currentSP->GetAmountToSpawn());
		}
		// Next wave
		SpawnPattern* nextSP = m_AISpawner->GetSpawnPattern(m_currentWave + 1);
		if (nextSP)
		{
			Size waveInfoSize(m_size.width* 0.25f, m_size.height);
			m_nextWaveInfo->SetWaveInfo(nextSP->GetType(), waveInfoSize, nextSP->GetAmountToSpawn());
			m_nextWaveInfo->setPositionX(m_size.width * 0.5f - waveInfoSize.width * 0.5f);
			auto moveTo = MoveTo::create(nextSP->GetTimeToSpawn(), m_currentWaveInfo->getPosition());
			m_nextWaveInfo->runAction(moveTo);
		}
		else
		{
			m_nextWaveInfo->Clear();
		}

		m_currentWave = m_AISpawner->GetCurrentSpawnPatternIndex();
	}
}