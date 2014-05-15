#pragma once
#include "Scene.h"

#include "PlayerId.h"
#include "Table.h"
#include "Net.h"
#include "Ball.h"
#include "Racket.h"
#include "BallEventManager.h"
#include "HapticResponseManager.h"
#include "GameRulesManager.h"

class PongScene : public Scene
{
public:
	PongScene(Application& app, GameRulesManagerPtr gameRules);
	~PongScene(void);

	void enter(ScenePtr from);
	void exit(ScenePtr to);

	void render(const double& timeStep);
	void updateLogic(const double& timeStep);
	void updateHaptics(const double& timeStep);

	void onKeyDown(unsigned char key, int x, int y);
    void onSpecialDown(int key, int x, int y);
	
	void onNewRound(const Score& score, PlayerId nextServe, PlayerId prevWinner);
	void onGameOver(const Score& score, PlayerId winner);
	
	static ScenePtr create(Application& app) { return ScenePtr(new PongScene(app, nullptr)); }
	static ScenePtr create(Application& app, GameRulesManagerPtr gameRules) 
	{ 
		PongScene* scene = new PongScene(app, gameRules);
		gameRules->setPongScene(scene);
		return ScenePtr(scene); 
	}

private:
	 
	chai3d::cWorldPtr m_world;
    chai3d::cCamera* m_camera;
	
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	
	btCollisionShapePtr m_tableCollisionShape;
    btCollisionShapePtr m_netCollisionShape;
    btCollisionShapePtr m_ballCollisionShape;
    btCollisionShapePtr m_racketsCollisionShape;
	
	TablePtr m_table;
	NetPtr m_net;
	BallPtr m_ball;
    RacketPtr m_playerRacket;
    RacketPtr m_opponentRacket;
	BallEventManagerPtr m_ballEventMgr;
	HapticResponseManagerPtr m_hapticResponseMgr;
    
	void createCamera();
    void createLight();
	void createTable();
	void createNet();
	void createBall();
	void createRackets();

	GameRulesManagerPtr m_gameRules;
};

