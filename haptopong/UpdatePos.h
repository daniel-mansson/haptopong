#pragma once

#include "Message.h"

class UpdatePos : public Message
{
public:

	UpdatePos()
	{
	}

	UpdatePos(const btVector3& position) :
		m_position(position)
	{
		
	}

	const btVector3& getPosition() const
	{
		return m_position;
	}
	
	virtual int getSize() const
	{
		//type + pos
		return 1 + sizeof(float) * 3;
	}

	virtual const unsigned char* getData() const
	{
		return nullptr;
	}

	virtual unsigned char getType() const
	{
		return G_UPDATE_POS;
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
	}
	
	virtual void setFromBuffer(unsigned char*& buffer)
	{
		for(int i = 0; i < 3; ++i)
		{
			m_position[i] = *(float*)buffer;
			buffer += sizeof(float);
		}
	}

private:

	btVector3 m_position;
};

