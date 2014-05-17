#pragma once
#include "GameRules.h"
#include "Score.h"

class IgnoreEverythingRules : public GameRules
{
public:
	IgnoreEverythingRules(){}
	virtual ~IgnoreEverythingRules(void){}
	
	virtual void onBallHitTable(PlayerId side){}
	virtual void onBallHitRacket(PlayerId racketId){}
	virtual void onBallOut(){}
	
	virtual bool isRoundOver() const { return false; }
	virtual const Score& getScore() const { return m_score; }
	virtual bool isGameOver() const { return false; }
	virtual PlayerId nextServe() const { return PLAYER_LOCAL; }
	virtual PlayerId winner() const { return PLAYER_LOCAL; }

	virtual void onNewRound(){}

private:
	
	Score m_score;
};

