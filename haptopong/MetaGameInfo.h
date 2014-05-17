#pragma once

#include "Message.h"
#include "PlayerId.h"

class MetaGameInfo : public Message
{
public:

	enum EventType
	{
		META_ROUND_START,
		META_GAME_OVER
	};

	MetaGameInfo()
	{
	}

	MetaGameInfo(unsigned char winner, unsigned char eventType, unsigned char serve, int scoreLocal, int scoreRemote) :
		m_playerId(winner),
		m_eventType(eventType),
		m_scoreLocal(scoreLocal),
		m_scoreRemote(scoreRemote),
		m_serve(serve)
	{
		
	}
	
	PlayerId getWinner() const 
	{
		return (PlayerId)m_playerId;
	}

	PlayerId getServe() const 
	{
		return (PlayerId)m_serve;
	}

	EventType getEventType() const
	{
		return (EventType)m_eventType;
	}
	
	int getLocalScore() const
	{
		return m_scoreLocal;
	}

	int getRemoteScore() const
	{
		return m_scoreRemote;
	}

	int getScore(PlayerId id) const
	{
		if(id == PLAYER_LOCAL)
			return m_scoreLocal;
		else if(id == PLAYER_REMOTE)
			return m_scoreRemote;
		else 
			return 0;
	}
	
	virtual int getSize() const
	{
		//type + winner + eventtype + 2 * score
		return 4 * sizeof(unsigned char) + 2 * sizeof(int);
	}

	virtual const unsigned char* getData() const
	{
		return nullptr;
	}

	virtual unsigned char getType() const
	{
		return G_META;
	}
	
	virtual void addToBuffer(unsigned char*& buffer) const
	{
		*buffer = getType();
		buffer += 1;
		*buffer = m_playerId;
		buffer += 1;
		*buffer = m_eventType;
		buffer += 1;
		*buffer = m_serve;
		buffer += 1;
		*(int*)buffer = m_scoreLocal;
		buffer += sizeof(int);
		*(int*)buffer = m_scoreRemote;
		buffer += sizeof(int);
	}
	
	virtual void setFromBuffer(unsigned char*& buffer)
	{
		m_playerId = *buffer;
		buffer += 1;		
		m_eventType = *buffer;
		buffer += 1;	
		m_serve = *buffer;
		buffer += 1;
		m_scoreLocal = *(int*)buffer;
		buffer += sizeof(int);
		m_scoreRemote = *(int*)buffer;
		buffer += sizeof(int);
	}

private:
	unsigned char m_playerId;
	unsigned char m_eventType;
	unsigned char m_serve;
	int m_scoreLocal;
	int m_scoreRemote;
};

