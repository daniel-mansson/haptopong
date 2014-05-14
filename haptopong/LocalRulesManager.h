#pragma once
#include "GameRulesManager.h"
#include "GameRules.h"

class LocalRulesManager :
	public GameRulesManager
{
public:
	LocalRulesManager(PongScene& pongScene, GameRulesPtr gameRules, int port);
	virtual ~LocalRulesManager(void);
	
	virtual void initialize();

	virtual void onBallHitTable(const Ball& ball, const Table& table);
	virtual void onBallHitRacket(const Ball& ball, const Table& table);
	virtual void onBallOut(const Ball& ball, const Table& table);
	
	virtual void update(const double& timeStep);

	virtual bool isWaiting() const { return m_isWaiting; }
private:
	GameRulesPtr m_gameRules;
	bool m_isWaiting;
	
	int m_port;
	ENetAddress m_address;
	ENetHost* m_server;
	ENetPeer* m_peer;

};

