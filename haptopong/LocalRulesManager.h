#pragma once
#include "GameRulesManager.h"
#include "GameRules.h"

class LocalRulesManager :
	public GameRulesManager
{
public:
	LocalRulesManager(GameRulesPtr gameRules, int port);
	virtual ~LocalRulesManager(void);
	
	virtual void initialize();

	virtual void onBallHitTable(const Ball& ball, const Table& table);
	virtual void onBallHitRacket(const Ball& ball, const Racket& racket);
	virtual void onBallOut(const Ball& ball);
	
	virtual void update(const double& timeStep);
	virtual void updatePlayerPos(const btVector3& position);

	virtual bool isWaiting() const { return m_isWaiting; }
	
	virtual PlayerId getPlayerId() const { return PLAYER_LOCAL; };
	virtual PlayerId getOpponentId() const { return PLAYER_REMOTE; };
private:
	GameRulesPtr m_gameRules;
	bool m_isWaiting;
	
	int m_port;
	ENetAddress m_address;
	ENetHost* m_server;
	ENetPeer* m_peer;
	unsigned char* m_buffer;

};

