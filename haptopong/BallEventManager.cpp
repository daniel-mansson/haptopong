#include "pch.h"
#include "BallEventManager.h"
#include "LinearResponse.h"

BallEventManager::BallEventManager(HapticResponseManagerPtr hapticResponseMgr, GameRulesManagerPtr gameRulesMgr) :
	m_hapticResponseMgr(hapticResponseMgr),
	m_gameRulesMgr(gameRulesMgr)
{
	m_tableHit = new SoundPlayer("../sounds/tennis_ball_hit_by_table.wav", 0);
	m_racketHit = new SoundPlayer("../sounds/tennis_ball_hit_by_racket.wav", 0);
}


BallEventManager::~BallEventManager(void)
{
}

void BallEventManager::OnTableHit(btManifoldPoint& point, Table& table, Ball& ball)
{
	if (chai3d::cAbs(ball.getVelocity().z()) > 0.2) m_tableHit->play();

	//std::cout<<"Table hit!\n";
	if(m_gameRulesMgr != nullptr)
		m_gameRulesMgr->onBallHitTable(ball, table);
}

void BallEventManager::OnNetHit(btManifoldPoint& point, Net& net, Ball& ball)
{
	//std::cout<<"Net hit!\n";

}

void BallEventManager::OnRacketHit(btManifoldPoint& point, Racket& racket, Ball& ball)
{
	std::cout << racket.getVelocity().length() << std::endl;

	if(ball.isActive())
	{
		m_racketHit->play(chai3d::cAbs(racket.getVelocity().length()) + 1);

		btVector3 bvel = ball.getVelocity();
		btVector3 rvel = Util::Vec(racket.getVelocity() * racket.getMoveAreaScale());

		float xvel = rvel[0] - bvel[0];
		if(xvel < 0.0f)
		{
			//std::cout << 

			m_hapticResponseMgr->setCurrent(CollisionResponsePtr(new LinearResponse(racket, ball)));

			//TODO: real forces
			xvel *= (1.0f + point.m_combinedRestitution);	
			bvel[0] += xvel;
			bvel[2] += -xvel * 0.04f;
			ball.setVelocity(bvel);

			ball.setActive(false);

			if(m_gameRulesMgr != nullptr)
				m_gameRulesMgr->onBallHitRacket(ball, racket);
		}
	}

	//std::cout<<"Racket hit!\n";
}
