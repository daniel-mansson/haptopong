#include "pch.h"
#include "GameRulesManager.h"
#include "PongScene.h"

GameRulesManager::GameRulesManager(PongScene& pongScene) :
	m_pongScene(pongScene)
{
}


GameRulesManager::~GameRulesManager(void)
{
}

void GameRulesManager::SendNewRound(const Score& score, PlayerId nextServe, PlayerId prevWinner)
{
	m_pongScene.onNewRound(score, nextServe, prevWinner);
}

void GameRulesManager::SendGameOver(const Score& score, PlayerId winner)
{
	m_pongScene.onGameOver(score, winner);
}
