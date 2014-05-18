#pragma once

#include "GameObject.h"
#include "BallProperties.h"
#include "ShadowlessSphere.h"
#include "ShadowSphere.h"

class Ball : public GameObject
{
public:
	Ball(ShadowlessSphere* shape, ShadowSphere* shadowShape, btCollisionShape* collisionShape, const BallProperties &properties = BallProperties(), const btTransform &startTransform = btTransform(btMatrix3x3::getIdentity()));
	~Ball(void);

	virtual GameObjectType getType() const { return BALL; };
	
	virtual void render(float timeStep);
	virtual void updateLogic(float timeStep);
	virtual void updateHaptics(chai3d::cGenericHapticDevicePtr device, const double& timeStep);

	virtual void onCollision(const btCollisionResult& collision);

	void setProperties(const BallProperties& properties) { m_properties = properties; }
	const BallProperties& getProperties() const { return m_properties; }

	void setPosition(const btVector3& position);
	void setVelocity(const btVector3& velocity);
	void setAngularVelocity(const btVector3& angularVelocity);
	void setAngularVelocity(const btVector3& axis, float angle);
	void stop();

	const btVector3& getPosition() const;
	const btVector3& getVelocity() const;
	const btVector3& getAngularVelocity() const;
	
    btRigidBody* getBody() const;
    chai3d::cShapeSphere* getShape() const;
	
	void setBernulli(const chai3d::cVector3d& bernoulli) { m_bernoulli = bernoulli; }
	void setResistance(const chai3d::cVector3d& resistance) { m_resistance = resistance; }
	const chai3d::cVector3d& getBernoulli() const { return m_bernoulli; }
	const chai3d::cVector3d& getResistance() const { return m_resistance; }

	//A ball is active if it is moving towards the local player.
	bool isActive() const { return m_isActive; }
	void setActive(bool active) { m_isActive = active; }
	
	void onRoundStart();
	void onRoundEnd();

private:
	chai3d::cVector3d m_bernoulli;
	chai3d::cVector3d m_resistance;
	chai3d::cVector3d m_velocity;
	chai3d::cVector3d m_angularVelocity;

	BallProperties m_properties;
	btRigidBodyPtr m_body;
	ShadowlessSphere* m_shape;
	ShadowSphere* m_shadowShape;
    
	const btMotionState* m_motionState;
	btTransform m_transform;
	bool m_isActive;
	
	chai3d::cMaterial m_playMat;
	chai3d::cMaterial m_inactiveMat;
};

typedef std::shared_ptr<Ball> BallPtr;