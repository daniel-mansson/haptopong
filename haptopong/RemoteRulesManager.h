#pragma once
#include "GameRulesManager.h"
#include "GameRules.h"

class RemoteRulesManager :
	public GameRulesManager
{
public:
	RemoteRulesManager(ENetAddress addr);
	virtual ~RemoteRulesManager(void);
	
	virtual void initialize();

	virtual void onBallHitTable(const Ball& ball, const Table& table);
	virtual void onBallHitRacket(const Ball& ball, const Racket& racket, float hitMagnitude);
	virtual void onBallOut(const Ball& ball);
	virtual void onServeStart(const Ball& ball);
	
	virtual void update(const double& timeStep);
	virtual void updatePlayerPos(const btVector3& position);

	virtual bool isWaiting() const { return m_isWaiting; }
	
	virtual PlayerId getPlayerId() const { return PLAYER_REMOTE; };
	virtual PlayerId getOpponentId() const { return PLAYER_LOCAL; };
private:

	void sendMessage(MessagePtr msg, enet_uint32 reliability);

	bool m_isWaiting;
	
	int m_port;
	ENetAddress m_address;
	ENetHost* m_client;
	ENetPeer* m_peer;
	unsigned char* m_buffer;

};

