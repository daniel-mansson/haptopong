#include "pch.h"
#include "LocalRulesManager.h"

#include "Hello.h"

LocalRulesManager::LocalRulesManager(PongScene& pongScene, GameRulesPtr gameRules, int port) :
	m_gameRules(gameRules),
	GameRulesManager(pongScene),
	m_port(port)
{

}

LocalRulesManager::~LocalRulesManager(void)
{
}

void LocalRulesManager::initialize()
{
	m_address.host = ENET_HOST_ANY;
	m_address.port = m_port;

	m_server = enet_host_create(&m_address,
		32, 2, 0, 0);

	if(m_server != nullptr)
		std::cout<<"Server listening to port: "<<m_port<<std::endl;
	else
		std::cout<<"Failed to create server"<<std::endl;
}

void LocalRulesManager::onBallHitTable(const Ball& ball, const Table& table)
{

}

void LocalRulesManager::onBallHitRacket(const Ball& ball, const Table& table)
{

}

void LocalRulesManager::onBallOut(const Ball& ball, const Table& table)
{

}

void LocalRulesManager::update(const double& timeStep)
{	
	ENetEvent event;

	while(enet_host_service(m_server, &event, 0) > 0)
	{
		switch (event.type)
		{
			case ENET_EVENT_TYPE_CONNECT:
				std::cout<<"Client connected: "<< event.peer->address.host << " : " << event.peer->address.port<<std::endl;
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				if(m_isWaiting)
				{
				
				}
				else
				{
				
				}
				std::cout<<"Got data: "<< (char*)event.packet->data << std::endl;
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				std::cout<<"Client disconnected: "<< event.peer->address.host << " : " << event.peer->address.port<<std::endl;
				break;
		}
	}
}