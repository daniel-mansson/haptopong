#include "pch.h"
#include "BounceEffectPool.h"


BounceEffectPool::BounceEffectPool(chai3d::cWorld* world, int count)
{
	for(int i = 0; i < count; ++i)
	{
		m_unused.push_back(BounceEffectPtr(new BounceEffect(world)));
	}
}


BounceEffectPool::~BounceEffectPool(void)
{
}

void BounceEffectPool::put(const btVector3& pos)
{
	if(!m_unused.empty())
	{
		auto e = m_unused.back();
		m_unused.pop_back();
		e->start(pos);
		m_used.push_back(e);
	}
}


void BounceEffectPool::update(const double& timeStep)
{
	auto it = m_used.begin();
	while(it != m_used.end())
	{
		(*it)->update(timeStep);

		if((*it)->isDone())
		{
			auto toRemove = it;
			++it;
			(*toRemove)->stop();
			m_unused.push_back(*toRemove);
			m_used.erase(toRemove);
		}
		else
			++it;
	}
}
