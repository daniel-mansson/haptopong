#pragma once
#include "GameRulesManager.h"

class LocalRulesManager :
	public GameRulesManager
{
public:
	LocalRulesManager(void);
	virtual ~LocalRulesManager(void);
	
	virtual void OnBallHitTable(const Ball& ball, const Table& table);
	virtual void OnBallHitRacket(const Ball& ball, const Table& table);
	virtual void OnBallOut(const Ball& ball, const Table& table);


};

