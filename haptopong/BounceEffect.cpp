#include "pch.h"
#include "BounceEffect.h"

#include "ShadowlessCylinder.h"

using namespace chai3d;

BounceEffect::BounceEffect(chai3d::cWorld* world)
{
	m_shape = new ShadowlessCylinder(0.1, 0.1, 0.003);
	cMaterial mat;
	mat.m_ambient.set(1, 1, 1);
	mat.m_diffuse.set(0, 1, 1);
	m_shape->setMaterial(mat, true);
	world->addChild(m_shape);
	m_duration = 0.45;
	m_shape->setUseTransparency(true);
	m_shape->setEnabled(false);
	//m_shape->setShaderProgram(
}

BounceEffect::~BounceEffect(void)
{

}

void BounceEffect::start(const btVector3& pos)
{
	m_shape->setEnabled(true);
	m_shape->setLocalPos(Util::Vec(pos));
	
	m_time = 0.0;
}

void BounceEffect::update(const double& timeStep)
{
	m_time += timeStep;
	double t = m_time / m_duration;
	
	m_shape->setBaseRadius(0.03 + t * 0.09);
	m_shape->setTopRadius( 0.03 + t * 0.09);
	m_shape->setTransparencyLevel((1.0f - (float)(t))*(1.0f - (float)(t)));
}

bool BounceEffect::isDone() const
{
	return m_time >= m_duration;
}

void BounceEffect::stop()
{
	m_shape->setEnabled(false);
}
