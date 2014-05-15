#pragma once

#include "Message.h"
#include "PlayerId.h"

class BallEvent : public Message
{
public:

	enum EventType
	{
		BALLEVENT_TABLE,
		BALLEVENT_RACKET,
		BALLEVENT_OUTSIDE,
	};

	BallEvent()
	{
	}

	BallEvent(unsigned char playerId, unsigned char eventType) :
		m_playerId(playerId),
		m_eventType(eventType)
	{
		
	}

	PlayerId getPlayerId() const 
	{
		return (PlayerId)m_playerId;
	}

	EventType getEventType() const
	{
		return (EventType)m_eventType;
	}
	
	virtual int getSize() const
	{
		//type + id + eventtype
		return 3;
	}

	virtual const unsigned char* getData() const
	{
		return nullptr;
	}

	virtual unsigned char getType() const
	{
		return G_BALLEVENT;
	}
	
	virtual void addToBuffer(unsigned char*& buffer) const
	{
		*buffer = getType();
		buffer += 1;
		*buffer = m_playerId;
		buffer += 1;
		*buffer = m_eventType;
		buffer += 1;
	}
	
	virtual void setFromBuffer(unsigned char*& buffer)
	{
		m_playerId = *buffer;
		buffer += 1;		
		m_eventType = *buffer;
		buffer += 1;
	}

private:
	unsigned char m_playerId;
	unsigned char m_eventType;
};

