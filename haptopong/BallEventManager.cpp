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
	if(ball.isActive() && (m_gameRulesMgr == nullptr || racket.getPlayerId() != m_gameRulesMgr->getOpponentId()))
	{
		btVector3 bvel = ball.getVelocity();
		btVector3 rvel = Util::Vec(racket.getVelocity() * racket.getMoveAreaScale());

		float xvel = rvel[0] - bvel[0];
		if(xvel < 0.0f)
		{
			float yvel = rvel[1] - bvel[1];
			float zvel = rvel[2] - bvel[2];

			float scale = 0.23f;
			float offset = 0;
			float tmp = chai3d::cAbs(xvel) * scale + offset;
			m_racketHit->play(chai3d::cClamp(tmp, 0.7f, 1.6f));

			m_hapticResponseMgr->setCurrent(CollisionResponsePtr(new LinearResponse(racket, ball)));

			//TODO: real forces
			xvel *= (1.0f + point.m_combinedRestitution);	
			bvel[0] += xvel;
			bvel[1] += yvel * 0.4f;
			bvel[2] += zvel * 0.4f;
			ball.setVelocity(bvel);

			btVector3 angVel = ball.getAngularVelocity();
			angVel[1] += -rvel[2] * 200.0f;
			angVel[2] += rvel[1] * 200.0f;
			std::cout<< "-Y "<<-rvel[2] * 200.0f<<"\t|Z "<< rvel[1] * 200.0f<<"\n";
			ball.setAngularVelocity(angVel);

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