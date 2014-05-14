#include "pch.h"
#include "GameRulesManager.h"
#include "PongScene.h"

GameRulesManager::GameRulesManager(PongScene& pongScene) :
	m_pongScene(pongScene)
{
	m_msgBufferMaxLength = 2048;
	m_msgBuffer = new unsigned char[m_msgBufferMaxLength];
	m_msgBufferLength = 0;
}

GameRulesManager::~GameRulesManager(void)
{
}

void GameRulesManager::sendNewRound(const Score& score, PlayerId nextServe, PlayerId prevWinner)
{
	m_pongScene.onNewRound(score, nextServe, prevWinner);
}

void GameRulesManager::sendGameOver(const Score& score, PlayerId winner)
{
	m_pongScene.onGameOver(score, winner);
}

void GameRulesManager::updateMessages(char* buffer, int length)
{
	if(m_msgBufferLength + length < m_msgBufferMaxLength)
	{
		memcpy(m_msgBuffer + m_msgBufferLength, buffer, length);

		while(true)
		{
			break;//TODO
		}
	}
	else
	{
		std::cerr<<"Network buffer overflow! :("<<std::endl;
	}
}