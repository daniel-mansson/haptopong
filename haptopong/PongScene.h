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
#include "StepTimer.h"
#include "Outside.h"
#include "AimAssistance.h"

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

	void updateOpponentPos(const btVector3& position);
	void updateBallState(const btVector3& position, const btVector3& velocity, const btVector3& angularVelocity);
	
	static ScenePtr create(Application& app) { return ScenePtr(new PongScene(app, nullptr)); }
	static ScenePtr create(Application& app, GameRulesManagerPtr gameRules) 
	{ 
		PongScene* scene = new PongScene(app, gameRules);
		gameRules->setPongScene(scene);
		return ScenePtr(scene); 
	}

private:

	btVector3 invert(const btVector3& vec);
	
	void prepareServe(PlayerId serve);
	void startServe();
	 
	chai3d::cWorldPtr m_world;
    chai3d::cCamera* m_camera;
	
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	
	btCollisionShapePtr m_tableCollisionShape;
    btCollisionShapePtr m_netCollisionShape;
    btCollisionShapePtr m_ballCollisionShape;
    btCollisionShapePtr m_racketsCollisionShape;
    btCollisionShapePtr m_outsideCollisionShape;
	
	TablePtr m_table;
	NetPtr m_net;
	BallPtr m_ball;
    RacketPtr m_playerRacket;
    RacketPtr m_opponentRacket;
	BallEventManagerPtr m_ballEventMgr;
	OutsidePtr m_outside;
	HapticResponseManagerPtr m_hapticResponseMgr;
	StepTimer m_networkTimer;
    
	void createCamera();
    void createLight();
	void createTable();
	void createNet();
	void createBall();
	void createRackets();
	void createOutside();
	
	GameRulesManagerPtr m_gameRules;
	AimAssistancePtr m_aimAssistance;
	PlayerId m_serve;
    
    chai3d::cShapeSphere* m_ballShadow;
};

