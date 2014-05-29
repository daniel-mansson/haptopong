#include "pch.h"
#include "BallEventManager.h"
#include "LinearResponse.h"

BallEventManager::BallEventManager(HapticResponseManagerPtr hapticResponseMgr, GameRulesManagerPtr gameRulesMgr, BounceEffectPoolPtr bouncePool) :
	m_hapticResponseMgr(hapticResponseMgr),
	m_gameRulesMgr(gameRulesMgr),
	m_lastHit(NONE),
	m_count(0),
	m_bouncePool(bouncePool),
	m_tableHitCooldown(1.0)
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

	if (chai3d::cAbs(ball.getVelocity().z()) > 0.2) 
		m_tableHit->play(0, (float)chai3d::cAbs(bzvel*0.3));
	
	if(m_tableHitCooldown < 0.1)
		return;

	std::cout<<"Table hit!\n";
	if(m_gameRulesMgr != nullptr && ball.isActive())
	{
		if(m_lastHit != TABLE)
			m_count = 0;
		else
			++m_count;
		
		if(m_count < 5)
		{
			m_gameRulesMgr->onBallHitTable(ball, table);
			m_tableHitCooldown = 0.0;
			m_count = 0;
		}
	}
	
	m_lastHit = TABLE;
	
	btVector3 pos = ball.getPosition();
	pos[2] = 0.055f;
	m_bouncePool->put(pos);
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
			m_hapticResponseMgr->setCurrent(CollisionResponsePtr(new LinearResponse(racket, ball)));
			
			float scale = 0.23f;
			float offset = 0;
			float tmp = chai3d::cAbs(xvel) * scale + offset;
			float hitMagnitude = (float)chai3d::cClamp(tmp, 0.7f, 1.6f);
			playSound(hitMagnitude);
			m_aimAssistance->applyImpulseFromRacket(point);

			if(m_gameRulesMgr != nullptr && ball.isActive())
				m_gameRulesMgr->onBallHitRacket(ball, racket, hitMagnitude);
			
			racket.flash();

			ball.setActive(false);
		}
	}

	//std::cout<<"Racket hit!\n";
	m_lastHit = RACKET;
}

void BallEventManager::playSound(float hitMagnitude)
{
	m_racketHit->play(hitMagnitude);
}

void BallEventManager::OnOutside(btManifoldPoint& point, Ball& ball)
{
	if(m_lastHit != OUTSIDE)
	{
		//std::cout<<"OUTSIDE!\n";
		m_lastHit = OUTSIDE;

		if(m_gameRulesMgr != nullptr && ball.isActive())
			m_gameRulesMgr->onBallOut(ball);
	}
}