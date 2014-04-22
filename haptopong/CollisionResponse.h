#pragma once
class CollisionResponse
{
public:
	CollisionResponse(void);
	virtual ~CollisionResponse(void);
	
	virtual void updateHaptics(const double& timeStep, chai3d::cVector3d& force) = 0;
	virtual bool isDone() const = 0;
};

typedef std::shared_ptr<CollisionResponse> CollisionResponsePtr;