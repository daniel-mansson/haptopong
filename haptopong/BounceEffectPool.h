#pragma once

#include "BounceEffect.h"

class BounceEffectPool
{
public:
	BounceEffectPool(chai3d::cWorld* world, int count);
	~BounceEffectPool(void);

	void put(const btVector3& pos);

	void update(const double& timeStep);

private:
	
	std::list<BounceEffectPtr> m_unused;
	std::list<BounceEffectPtr> m_used;
};

typedef std::shared_ptr<BounceEffectPool> BounceEffectPoolPtr;