#pragma once

#include "Ball.h"
#include "Table.h"
#include "Racket.h"
#include "Score.h"
#include "PlayerId.h"
#include "Message.h"

class PongScene;

class GameRulesManager
{
public:
	GameRulesManager();
	virtual ~GameRulesManager(void);

	virtual void initialize() = 0;
	
	virtual void onBallHitTable(const Ball& ball, const Table& table) = 0;
	virtual void onBallHitRacket(const Ball& ball, const Racket& racket) = 0;
	virtual void onBallOut(const Ball& ball) = 0;
	virtual void onServeStart(const Ball& ball) = 0;

	virtual bool isWaiting() const = 0;
	virtual const std::string& getStatus() const { return m_status; }
	
	virtual void update(const double& timeStep) = 0;
	virtual void updatePlayerPos(const btVector3& position) = 0;

	void setPongScene(PongScene* pongScene) { m_pongScene = pongScene; }

	virtual PlayerId getPlayerId() const = 0;
	virtual PlayerId getOpponentId() const = 0;

	void setEnabled(bool enabled) { m_enabled = enabled; }

protected:
	virtual void updateMessages(unsigned char* buffer, int length);

	std::vector<MessagePtr> m_messageQueue;
	unsigned char* m_msgBuffer;
	int m_msgBufferMaxLength;
	int m_msgBufferLength;

	PongScene* m_pongScene;
	std::string m_status;
	bool m_enabled;
};

typedef std::shared_ptr<GameRulesManager> GameRulesManagerPtr;