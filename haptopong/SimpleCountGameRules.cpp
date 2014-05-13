#include "pch.h"
#include "SimpleCountGameRules.h"

SimpleCountGameRules::SimpleCountGameRules(int scoreLimit) :
	m_turn(PLAYER_LOCAL),
	m_winner(NO_PLAYER),
	m_state(GOING_TOWARDS_PLAYER),
	m_roundOver(false),
	m_gameOver(false),
	m_scoreLimit(scoreLimit)
{
	
}


SimpleCountGameRules::~SimpleCountGameRules(void)
{
}

void SimpleCountGameRules::onBallHitTable(const Ball& ball, const Table& table, PlayerId side)
{
	if(m_state == GOING_TOWARDS_TABLE && m_turn == side)
		m_state = GOING_TOWARDS_PLAYER;
	else if(m_state == GOING_TOWARDS_TABLE && m_turn != side)
		roundOver(m_turn);
	else
		roundOver((PlayerId)(3 - m_turn));
}

void SimpleCountGameRules::onBallHitRacket(const Ball& ball, const Racket& racket)
{
	if(m_state == GOING_TOWARDS_PLAYER && racket.getPlayerId() == m_turn)
	{
		m_state = GOING_TOWARDS_TABLE;
		m_turn = (PlayerId)(3 - m_turn);
	}
	else
	{
		roundOver((PlayerId)(3 - m_turn));	
	}
}

void SimpleCountGameRules::onBallOut(const Ball& ball)
{
	roundOver((PlayerId)(3 - m_turn));	
}

void SimpleCountGameRules::onNewRound()
{

}

void SimpleCountGameRules::roundOver(PlayerId winner)
{
	m_winner = winner;
	m_roundOver = true;
	m_serve = (PlayerId)(3 - m_serve);
	m_turn = m_serve;
	m_state = GOING_TOWARDS_PLAYER;
	m_score.addScore(m_winner, 1);

	for(int i = 1; i <= 2; ++i)
	{
		if(m_score.getScore((PlayerId)i) >= m_scoreLimit)
		{
			m_gameOver = true;
			break;
		}
	}
}

bool SimpleCountGameRules::isRoundOver() const
{
	return m_roundOver;
}

const Score& SimpleCountGameRules::getScore() const
{
	return m_score;
}

bool SimpleCountGameRules::isGameOver() const
{
	return m_gameOver;
}

PlayerId SimpleCountGameRules::nextServe() const
{
	return m_turn;
}

PlayerId SimpleCountGameRules::winner() const
{
	return m_winner;
}
