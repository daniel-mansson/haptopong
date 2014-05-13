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
	//std::cout<<"Table hit!\n";
}

void BallEventManager::OnNetHit(btManifoldPoint& point, Net& net, Ball& ball)
{
	//std::cout<<"Net hit!\n";
}

void BallEventManager::OnRacketHit(btManifoldPoint& point, Racket& racket, Ball& ball)
{
    m_racketHit->play();
    
	if(ball.isActive())
	{
		btVector3 bvel = ball.getVelocity();
		btVector3 rvel = Util::Vec(racket.getVelocity() * racket.getMoveAreaScale());

		float xvel = rvel[0] - bvel[0];
		if(xvel < 0.0f)
		{
			m_hapticResponseMgr->setCurrent(CollisionResponsePtr(new LinearResponse(racket, ball)));
		
			//TODO: real forces
			xvel *= (1.0f + point.m_combinedRestitution);	
			bvel[0] += xvel;
			bvel[2] += -xvel * 0.04f;
			ball.setVelocity(bvel);

			ball.setActive(false);
		}
	}

	//std::cout<<"Racket hit!\n";
}
