#pragma once

#include "PlayerId.h"
#include "Ball.h"
#include "Table.h"
#include "Score.h"
#include "Racket.h"

class GameRules
{
public:
	GameRules(void);
	virtual ~GameRules(void);
	
	virtual void onBallHitTable(PlayerId side) = 0;
	virtual void onBallHitRacket(PlayerId racketId) = 0;
	virtual void onBallOut() = 0;
	
	virtual bool isRoundOver() const = 0;
	virtual const Score& getScore() const = 0;
	virtual bool isGameOver() const = 0;
	virtual PlayerId nextServe() const = 0;
	virtual PlayerId winner() const = 0;

	virtual void onNewRound() = 0;
};

typedef std::shared_ptr<GameRules> GameRulesPtr;
