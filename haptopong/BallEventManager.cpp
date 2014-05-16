#include "pch.h"
#include "BallEventManager.h"
#include "LinearResponse.h"

BallEventManager::BallEventManager(HapticResponseManagerPtr hapticResponseMgr, GameRulesManagerPtr gameRulesMgr) :
	m_hapticResponseMgr(hapticResponseMgr),
	m_gameRulesMgr(gameRulesMgr),
	m_lastHit(NONE)
{
	m_tableHit = new SoundPlayer("../sounds/tennis_ball_hit_by_table.wav", 0);
	m_racketHit = new SoundPlayer("../sounds/tennis_ball_hit_by_racket.wav", 0);
}


BallEventManager::~BallEventManager(void)
{
}

void BallEventManager::OnTableHit(btManifoldPoint& point, Table& table, Ball& ball)
{
	float bzvel = ball.getVelocity().z();

	if (chai3d::cAbs(ball.getVelocity().z()) > 0.2) m_tableHit->play(0, chai3d::cAbs(bzvel*0.3));

	//std::cout<<"Table hit!\n";
	if(m_gameRulesMgr != nullptr)
		m_gameRulesMgr->onBallHitTable(ball, table);
	
	m_lastHit = TABLE;
}

void BallEventManager::OnNetHit(btManifoldPoint& point, Net& net, Ball& ball)
{
	//std::cout<<"Net hit!\n";
	
	m_lastHit = NET;
}

void BallEventManager::OnRacketHit(btManifoldPoint& point, Racket& racket, Ball& ball)
{
	if(ball.isActive() && racket.getPlayerId() != m_gameRulesMgr->getOpponentId())
	{
		btVector3 bvel = ball.getVelocity();
		btVector3 rvel = Util::Vec(racket.getVelocity() * racket.getMoveAreaScale());

		float xvel = rvel[0] - bvel[0];
		if(xvel < 0.0f)
		{
			float scale = 0.23f;
			float offset = 0;
			float tmp = chai3d::cAbs(xvel) * scale + offset;
			m_racketHit->play(chai3d::cClamp(tmp, 0.7f, 1.6f));

			m_hapticResponseMgr->setCurrent(CollisionResponsePtr(new LinearResponse(racket, ball)));

			//TODO: real forces
			xvel *= (1.0f + point.m_combinedRestitution);	
			bvel[0] += xvel * 1.0f;
			bvel[2] += -xvel * 0.07f;
			ball.setVelocity(bvel);

			ball.setActive(false);

			if(m_gameRulesMgr != nullptr)
				m_gameRulesMgr->onBallHitRacket(ball, racket);
		}
	}

	//std::cout<<"Racket hit!\n";
	m_lastHit = RACKET;
}

void BallEventManager::OnOutside(btManifoldPoint& point, Ball& ball)
{
	if(m_lastHit != OUTSIDE)
	{
		std::cout<<"OUTSIDE!\n";
		m_lastHit = OUTSIDE;

		if(m_gameRulesMgr != nullptr)
			m_gameRulesMgr->onBallOut(ball);
	}
}