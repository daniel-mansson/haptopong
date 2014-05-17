#include "pch.h"
#include "SimpleCountGameRules.h"

SimpleCountGameRules::SimpleCountGameRules(int scoreLimit) :
	m_turn(PLAYER_LOCAL),
	m_winner(NO_PLAYER),
	m_state(GOING_TOWARDS_TABLE),
	m_roundOver(false),
	m_gameOver(false),
	m_scoreLimit(scoreLimit),
	m_serve(PLAYER_LOCAL)
{
	
}


SimpleCountGameRules::~SimpleCountGameRules(void)
{
}

void SimpleCountGameRules::onBallHitTable(PlayerId side)
{
	//std::cout<<"1table: "<<m_state<<"\t"<<m_turn<<"\n";
	if(m_state == GOING_TOWARDS_TABLE && m_turn == side)
		m_state = GOING_TOWARDS_PLAYER;
	else if(m_state == GOING_TOWARDS_TABLE && m_turn != side)
		roundOver(m_turn);
	else
		roundOver((PlayerId)(3 - m_turn));

	//std::cout<<"2table: "<<m_state<<"\t"<<m_turn<<"\n";
}

void SimpleCountGameRules::onBallHitRacket(PlayerId racketId)
{
	//std::cout<<"1racket: "<<m_state<<"\t"<<m_turn<<"\n";
	if(m_state == GOING_TOWARDS_PLAYER && racketId == m_turn)
	{
		m_state = GOING_TOWARDS_TABLE;
		m_turn = (PlayerId)(3 - m_turn);
	}
	else
	{
		roundOver((PlayerId)(3 - m_turn));	
	}

//	std::cout<<"2racket: "<<m_state<<"\t"<<m_turn<<"\n";
}

void SimpleCountGameRules::onBallOut()
{
	//std::cout<<"1out: "<<m_state<<"\t"<<m_turn<<"\n";
	roundOver((PlayerId)(3 - m_turn));

	//std::cout<<"2out: "<<m_state<<"\t"<<m_turn<<"\n";	
}

void SimpleCountGameRules::onNewRound()
{
	m_roundOver = false;
}

void SimpleCountGameRules::roundOver(PlayerId winner)
{
	//std::cout<<"round: "<<winner<<"\n";	
	m_winner = winner;
	m_roundOver = true;
	m_serve = winner;//(PlayerId)(3 - m_serve);
	m_turn = m_serve;
	m_state = GOING_TOWARDS_TABLE;
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
