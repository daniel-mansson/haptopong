#include "pch.h"
#include "RemoteRulesManager.h"

#include "PongScene.h"
#include "Hello.h"
#include "UpdatePos.h"
#include "BallEvent.h"

RemoteRulesManager::RemoteRulesManager(GameRulesPtr gameRules, ENetAddress addr) :
	m_gameRules(gameRules),
	m_address(addr)
{
	m_buffer = new unsigned char[2048];
}

RemoteRulesManager::~RemoteRulesManager(void)
{
	delete[] m_buffer;
	enet_host_destroy(m_client);
}

void RemoteRulesManager::initialize()
{
	m_client = enet_host_create (NULL,
		1, 2, 100000, 100000);

	if(m_client != nullptr) 
	{
		std::cout<<"Connecting to the server..."<<std::endl;
	}
	else
		std::cout<<"Failed to create server"<<std::endl;

	m_peer = enet_host_connect (m_client, &m_address, 2, 0); 
}

void RemoteRulesManager::onBallHitTable(const Ball& ball, const Table& table)
{
	PlayerId id = ball.getPosition().x() < 0.0f ? getOpponentId() : getPlayerId();;
	sendMessage(MessagePtr(new BallEvent(id, BallEvent::BALLEVENT_TABLE)), ENET_PACKET_FLAG_RELIABLE);
}

void RemoteRulesManager::onBallHitRacket(const Ball& ball, const Racket& racket)
{
	sendMessage(MessagePtr(new BallEvent(racket.getPlayerId(), BallEvent::BALLEVENT_RACKET)), ENET_PACKET_FLAG_RELIABLE);

	//TODO: state update
}

void RemoteRulesManager::onBallOut(const Ball& ball)
{
	sendMessage(MessagePtr(new BallEvent(NO_PLAYER, BallEvent::BALLEVENT_OUTSIDE)), ENET_PACKET_FLAG_RELIABLE);
}

void RemoteRulesManager::update(const double& timeStep)
{	
	ENetEvent event;

	while(enet_host_service(m_client, &event, 0) > 0)
	{
		switch (event.type)
		{
		case ENET_EVENT_TYPE_CONNECT:
			{
				unsigned char* buf = m_buffer;

				MessagePtr hello = MessagePtr(new Hello("Client"));
				sendMessage(hello, ENET_PACKET_FLAG_RELIABLE);
			}
			break;

		case ENET_EVENT_TYPE_RECEIVE:
			updateMessages(event.packet->data, event.packet->dataLength);

			for(auto msg : m_messageQueue)
			{
				switch(msg->getType())
				{
				case S_WELCOME:
					std::cout<<"GOT WELCOME "<<msg->getData()<<std::endl;
					m_isWaiting = false;
					break;
				case G_UPDATE_POS:
					m_pongScene->updateOpponentPos(((UpdatePos*)msg.get())->getPosition());
					break;
				}
			}
			m_messageQueue.clear();

			//std::cout<<"Got data: "<< event.packet->dataLength << std::endl;
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout<<"Client disconnected: "<< event.peer->address.host << " : " << event.peer->address.port<<std::endl;
			break;
		}
	}
}

void RemoteRulesManager::updatePlayerPos(const btVector3& position)
{
	if(isWaiting())
		return;

	unsigned char* buf = m_buffer;

	MessagePtr msg = MessagePtr(new UpdatePos(position));
	
	sendMessage(msg, ENET_PACKET_FLAG_UNRELIABLE_FRAGMENT);
}

void RemoteRulesManager::sendMessage(MessagePtr msg, enet_uint32 reliability)
{
	unsigned char* buf = m_buffer;
	msg->addToBuffer(buf);

	ENetPacket * packet = enet_packet_create (m_buffer, 
		msg->getSize(), 
		reliability);

	enet_peer_send (m_peer, 0, packet);
	enet_host_flush (m_client);
}