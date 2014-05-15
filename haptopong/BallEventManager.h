#pragma once

#include "Ball.h"
#include "Table.h"
#include "Racket.h"
#include "Net.h"
#include "HapticResponseManager.h"
#include "SoundPlayer.h"

class BallEventManager
{
public:
	BallEventManager(HapticResponseManagerPtr hapticResponseMgr);
	~BallEventManager(void);
	
	void OnTableHit(btManifoldPoint& point, Table& table, Ball& ball);
	void OnNetHit(btManifoldPoint& point, Net& net, Ball& ball);
	void OnRacketHit(btManifoldPoint& point, Racket& racket, Ball& ball);

private:
	HapticResponseManagerPtr m_hapticResponseMgr;
    SoundPlayer* m_racketHit;
    SoundPlayer* m_tableHit;
};

typedef std::shared_ptr<BallEventManager> BallEventManagerPtr;
