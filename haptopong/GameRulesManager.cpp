#include "pch.h"
#include "GameRulesManager.h"
#include "PongScene.h"

#include "Hello.h"
#include "Welcome.h"
#include "UpdatePos.h"
#include "BallEvent.h"
#include "BallState.h"
#include "MetaGameInfo.h"

GameRulesManager::GameRulesManager() :
	m_enabled(true)
{
	m_msgBufferMaxLength = 2048;
	m_msgBuffer = new unsigned char[m_msgBufferMaxLength];
	m_msgBufferLength = 0;
}

GameRulesManager::~GameRulesManager(void)
{
	delete[] m_msgBuffer;
}

void GameRulesManager::updateMessages(unsigned char* buffer, int length)
{
	if(m_msgBufferLength + length < m_msgBufferMaxLength)
	{
		memcpy(m_msgBuffer + m_msgBufferLength, buffer, length);
		m_msgBufferLength += length;

		unsigned char* buf = m_msgBuffer;
		int processed = 0;
		for(int i = 0; i < m_msgBufferLength;)
		{
			unsigned char peekType = m_msgBuffer[i];

			MessagePtr msg = nullptr;
			switch(peekType)
			{
			case C_HELLO:
				msg = MessagePtr(new Hello());
				break;
			case S_WELCOME:
				msg = MessagePtr(new Welcome());
				break;
			case G_UPDATE_POS:
				msg = MessagePtr(new UpdatePos());
				break;
			case G_BALLEVENT:
				msg = MessagePtr(new BallEvent());
				break;
			case G_BALLSTATE:
				msg = MessagePtr(new BallState());
				break;
			case G_META:
				msg = MessagePtr(new MetaGameInfo());
				break;
			}

			if(m_msgBufferLength - i >= msg->getSize())
			{
				buf += 1;
				msg->setFromBuffer(buf);
				m_messageQueue.push_back(msg);
				processed += msg->getSize();
			}
			
			i += msg->getSize();
		}

        memmove(m_msgBuffer, m_msgBuffer + processed, m_msgBufferLength - processed);
		m_msgBufferLength -= processed;
	}
	else
	{
		std::cerr<<"Network buffer overflow! :("<<std::endl;
	}
}