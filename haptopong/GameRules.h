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
	
	virtual void onBallHitTable(const Ball& ball, const Table& table, PlayerId side) = 0;
	virtual void onBallHitRacket(const Ball& ball, const Racket& racket) = 0;
	virtual void onBallOut(const Ball& ball) = 0;
	
	virtual bool isRoundOver() const = 0;
	virtual const Score& getScore() const = 0;
	virtual bool isGameOver() const = 0;
	virtual PlayerId nextServe() const = 0;
	virtual PlayerId winner() const = 0;

	virtual void onNewRound() = 0;
};

