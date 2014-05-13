#pragma once

#include "Ball.h"
#include "Table.h"
#include "Score.h"
#include "PlayerId.h"

class PongScene;

class GameRulesManager
{
public:
	GameRulesManager(PongScene& pongScene);
	virtual ~GameRulesManager(void);
	
	virtual void OnBallHitTable(const Ball& ball, const Table& table) = 0;
	virtual void OnBallHitRacket(const Ball& ball, const Table& table) = 0;
	virtual void OnBallOut(const Ball& ball, const Table& table) = 0;

protected:
	virtual void SendNewRound(const Score& score, PlayerId nextServe, PlayerId prevWinner);
	virtual void SendGameOver(const Score& score, PlayerId winner);

	PongScene& m_pongScene;
};

