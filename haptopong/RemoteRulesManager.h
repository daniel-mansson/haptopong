#pragma once
#include "GameRulesManager.h"
#include "GameRules.h"

class RemoteRulesManager :
	public GameRulesManager
{
public:
	RemoteRulesManager(GameRulesPtr gameRules, ENetAddress addr);
	virtual ~RemoteRulesManager(void);
	
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
	ENetHost* m_client;
	ENetPeer* m_peer;
	unsigned char* m_buffer;

};

