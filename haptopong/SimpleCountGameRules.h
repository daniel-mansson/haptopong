#pragma once
#include "GameRules.h"
#include "Score.h"

class SimpleCountGameRules : public GameRules
{
public:
	SimpleCountGameRules(int scoreLimit);
	virtual ~SimpleCountGameRules(void);
	
	virtual void onBallHitTable(const Ball& ball, const Table& table, PlayerId side);
	virtual void onBallHitRacket(const Ball& ball, const Racket& racket);
	virtual void onBallOut(const Ball& ball);
	
	virtual bool isRoundOver() const;
	virtual const Score& getScore() const;
	virtual bool isGameOver() const;
	virtual PlayerId nextServe() const;
	virtual PlayerId winner() const;

	virtual void onNewRound();

private:
	
	void roundOver(PlayerId winner);

	enum State
	{
		GOING_TOWARDS_PLAYER,
		GOING_TOWARDS_TABLE
	} m_state;

	PlayerId m_serve;
	PlayerId m_turn;
	PlayerId m_winner;
	
	bool m_roundOver;
	bool m_gameOver;
	int m_scoreLimit;
	Score m_score;
};

