#include "pch.h"
#include "RemoteRulesManager.h"

#include "Hello.h"

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

}

void RemoteRulesManager::onBallHitRacket(const Ball& ball, const Table& table)
{

}

void RemoteRulesManager::onBallOut(const Ball& ball, const Table& table)
{

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
				hello->addToBuffer(buf);

				ENetPacket * packet = enet_packet_create (m_buffer, 
					hello->getSize(), 
					ENET_PACKET_FLAG_RELIABLE);

				enet_peer_send (m_peer, 0, packet);
				enet_host_flush (m_client);
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
				}
			}

			std::cout<<"Got data: "<< (char*)event.packet->data << std::endl;
			break;

		case ENET_EVENT_TYPE_DISCONNECT:
			std::cout<<"Client disconnected: "<< event.peer->address.host << " : " << event.peer->address.port<<std::endl;
			break;
		}
	}
}