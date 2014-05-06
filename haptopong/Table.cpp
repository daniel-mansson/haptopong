#include "pch.h"
#include "Table.h"


Table::Table(chai3d::cMultiMesh* mesh, btRigidBody* body) :
	m_mesh(mesh),
	m_body(body)
{
}


Table::~Table(void)
{
}

void Table::render(float timeStep)
{

}

void Table::updateLogic(float timeStep)
{

}

void Table::updateHaptics(float timeStep)
{

}

void Table::onCollision(const btCollisionResult& collision)
{

}
