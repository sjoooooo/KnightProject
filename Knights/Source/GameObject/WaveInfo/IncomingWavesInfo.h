#ifndef _INCOMING_WAVES_INFO_H_
#define _INCOMING_WAVES_INFO_H_

#include "WaveInfo.h"
#include "..\Spawner\AISpawner.h"

#include "cocos2d.h"
USING_NS_CC;

class IncomingWavesInfo : public Node
{
private:
	// Background
	Sprite* m_background;
	Sprite* m_currentwaveinfoBox;
	// Incoming waves
	WaveInfo* m_currentWaveInfo;
	WaveInfo* m_nextWaveInfo;
	Size m_size;
	int m_currentWave;
	AISpawner* m_AISpawner;
public:
	IncomingWavesInfo();
	~IncomingWavesInfo();

	// Setters

	// Getters
	Size GetSize();

	// Others
	void Init(AISpawner* m_AISpawner, Size* size);
	void Update(float dt);
};

#endif 
