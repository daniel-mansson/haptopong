#pragma once

#include "Message.h"

class BallState : public Message
{
public:

	BallState()
	{
	}

	BallState(const btVector3& position, const btVector3& vel, const btVector3& angVel, float hitMagnitude, unsigned char serve = 0) :
		m_position(position),
		m_velocity(vel),
		m_angularVelocity(angVel),
		m_serve(serve)
	{

	}

	const btVector3& getPosition() const
	{
		return m_position;
	}

	const btVector3& getVelocity() const
	{
		return m_velocity;
	}

	const btVector3& getAngularVelocity() const
	{
		return m_angularVelocity;
	}

	unsigned char getServe() const
	{
		return m_serve;
	}

	float getHitMagnitude() const
	{
		return m_hitMagnitude;
	}

	virtual int getSize() const
	{
		//type + pos + serve
		return 1 + 3 * 3 * sizeof(float) + 1 + sizeof(float);
	}

	virtual const unsigned char* getData() const
	{
		return nullptr;
	}

	virtual unsigned char getType() const
	{
		return G_BALLSTATE; 
	}

	virtual void addToBuffer(unsigned char*& buffer) const
	{
		*buffer = getType();
		buffer += 1;

		for(int i = 0; i < 3; ++i)
		{
			*(float*)buffer = m_position[i];
			buffer += sizeof(float);
		}

		for(int i = 0; i < 3; ++i)
		{
			*(float*)buffer = m_velocity[i];
			buffer += sizeof(float);
		}

		for(int i = 0; i < 3; ++i)
		{
			*(float*)buffer = m_angularVelocity[i];
			buffer += sizeof(float);
		}
		
		*(float*)buffer = m_hitMagnitude;
		buffer += sizeof(float);

		*buffer = m_serve;
		buffer += 1;
	}

	virtual void setFromBuffer(unsigned char*& buffer)
	{
		for(int i = 0; i < 3; ++i)
		{
			m_position[i] = *(float*)buffer;
			buffer += sizeof(float);
		}

		for(int i = 0; i < 3; ++i)
		{
			m_velocity[i] = *(float*)buffer;
			buffer += sizeof(float);
		}

		for(int i = 0; i < 3; ++i)
		{
			m_angularVelocity[i] = *(float*)buffer;
			buffer += sizeof(float);
		}
		
		m_hitMagnitude = *(float*)buffer;
		buffer += sizeof(float);

		m_serve = *buffer;
		buffer += 1;
	}

private:

	btVector3 m_position;
	btVector3 m_velocity;
	btVector3 m_angularVelocity;
	float m_hitMagnitude;
	unsigned char m_serve;
};

