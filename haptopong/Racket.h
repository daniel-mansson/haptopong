#pragma once

#include "GameObject.h"
#include "RacketProperties.h"

class Racket : public GameObject
{
public:
	Racket(chai3d::cMultiMesh* shape, btCollisionShape* collisionShape, const RacketProperties &properties = RacketProperties(), const btTransform &startTransform = btTransform(btMatrix3x3::getIdentity()));
	~Racket(void);
	
	virtual GameObjectType getType() const { return RACKET; }

	virtual void render(float timeStep);
	virtual void updateLogic(float timeStep);
	virtual void updateHaptics(chai3d::cGenericHapticDevicePtr device, const double& timeStep);

	virtual void onCollision(const btCollisionResult& collision);

	void setProperties(const RacketProperties& properties) { m_properties = properties; }

	//Setters for testing (should be updated by the physics engine later)
	void setVelocity(const chai3d::cVector3d& velocity) { m_velocity = velocity; }
	void setNormal(const chai3d::cVector3d& normal) { m_normal = normal; }
	void setForce(const chai3d::cVector3d& force) { m_force = force; }

	const RacketProperties& getProperties() const { return m_properties; }
	const chai3d::cVector3d& getNormal() const { return m_normal; }
	const chai3d::cVector3d& getVelocity() const { return m_velocity; }
	const chai3d::cVector3d& getVForce() const { return m_force; }
    
    btRigidBody* getBody() const;
    chai3d::cMultiMesh* getShape() const;

private:
	chai3d::cVector3d m_normal;
	chai3d::cVector3d m_velocity;
	chai3d::cVector3d m_force;
	RacketProperties m_properties;
	chai3d::cMultiMesh* m_shape;
    btRigidBodyPtr m_body;
	
	btTransform m_transform;
	btVector3 m_origin;
	chai3d::cVector3d m_startPos;
	chai3d::cVector3d m_hapticPos;
	chai3d::cVector3d m_hapticVel;
	chai3d::cVector3d m_offset;
};

typedef std::shared_ptr<Racket> RacketPtr;