#include "pch.h"
#include "BallEventManager.h"
#include "LinearResponse.h"

BallEventManager::BallEventManager(HapticResponseManagerPtr hapticResponseMgr):
	m_hapticResponseMgr(hapticResponseMgr)
{
    m_racketHit = new SoundPlayer("../sounds/racket.mp3");
}


BallEventManager::~BallEventManager(void)
{
}

void BallEventManager::OnTableHit(btManifoldPoint& point, Table& table, Ball& ball)
{
	std::cout<<"Table hit!\n";
}

void BallEventManager::OnNetHit(btManifoldPoint& point, Net& net, Ball& ball)
{
	std::cout<<"Net hit!\n";
}

void BallEventManager::OnRacketHit(btManifoldPoint& point, Racket& racket, Ball& ball)
{
    m_racketHit->play();
    
	if(ball.isActive())
	{
		btVector3 vel = ball.getVelocity();
		vel[0] *= -1.0f;
		vel[2] += 2.0f;
		ball.setVelocity(vel);
		ball.setActive(false);

		m_hapticResponseMgr->setCurrent(CollisionResponsePtr(new LinearResponse(racket, ball)));
	}

	std::cout<<"Racket hit!\n";
}
