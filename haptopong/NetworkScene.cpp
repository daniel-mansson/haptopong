#include "pch.h"
#include "NetworkScene.h"

#include "Application.h"

using namespace chai3d;

NetworkScene::NetworkScene(Application& app) :
	Scene(app),
	m_server(nullptr)
{
	m_world = cWorldPtr(new cWorld());
	m_world->m_backgroundColor.set(0.2f, 0.5f, 0.6f);  

	m_camera = new cCamera(m_world.get());
	m_world->addChild(m_camera);

	m_font = cFontPtr(NEW_CFONTCALIBRI32());

	cLabel* line;
	line = new cLabel(m_font.get());
	line->setString("1. Start server");
	m_labels.push_back(line);

	line = new cLabel(m_font.get());
	line->setString("2. Start client");
	m_labels.push_back(line);

	if (enet_initialize () != 0)
	{
		fprintf (stderr, "An error occurred while initializing ENet.\n");
	}

	for(auto* label : m_labels) 
	{
		m_camera->m_frontLayer->addChild(label);
	}
	m_state = 0;
}

NetworkScene::~NetworkScene(void)
{
	if(m_state == 1)
	{
		if(m_server)
			enet_host_destroy(m_server);
	}
	else if(m_state == 2)
	{
		if(m_client)
			enet_host_destroy(m_client);
	}
}

void NetworkScene::enter(ScenePtr from)
{

}

void NetworkScene::exit(ScenePtr to)
{

}

void NetworkScene::render(const double& timeStep)
{	
	double w = (double)m_app.getWindowWidth();
	double y = (double)m_app.getWindowHeight();
	y -= 10;

	for(auto* label : m_labels)
	{
		double lh = label->getStringHeight();

		y -= lh * 1.2;
		label->setLocalPos(0.5 * (w - label->getStringWidth()), y, 0);
	}

	m_camera->renderView(m_app.getWindowWidth(), m_app.getWindowHeight());
}

void NetworkScene::updateLogic(const double& timeStep)
{
	ENetEvent event;

	if(m_state == 1)
	{

	}
	else if(m_state == 2)
	{
		while(enet_host_service(m_client, &event, 0) > 0)
		{
			switch(event.type)
			{
			case ENET_EVENT_TYPE_CONNECT:
				print("Connected");
				break;

			case ENET_EVENT_TYPE_RECEIVE:
				print(std::string((char*)event.packet->data));
				break;

			case ENET_EVENT_TYPE_DISCONNECT:
				print("Disconnected");
				break;
			}
		}
	}
}

void NetworkScene::updateHaptics(const double& timeStep)
{
#ifdef WIN32
	Sleep(10);
#endif
}

void NetworkScene::onKeyDown(unsigned char key, int x, int y)
{
	ENetEvent event;

	if(m_state == 0)
	{
		switch(key)
		{
		case '1':
			m_address.host = ENET_HOST_ANY;
			m_address.port = 1234;

			m_server = enet_host_create(&m_address,
				32, 2, 0, 0);

			clear();

			if(m_server != nullptr)
				print("Server created");
			else
				print("Failed to create server");

			m_state = 1;
			break;
		case '2':
			m_client = enet_host_create (NULL,
				1, 2, 100000, 100000);

			enet_address_set_host (&m_address, "127.0.0.1");
			m_address.port = 1234;

			m_peer = enet_host_connect (m_client, &m_address, 2, 0);    
			
			clear();

			if (enet_host_service (m_client, &event, 3000) > 0 &&
				event.type == ENET_EVENT_TYPE_CONNECT)
			{
				print("Connected");
				m_state = 2;
			}
			else
			{
				print("Failed to connect");
				m_state = 0;
			}

			break;
		}
	}
	else if(m_state == 1)
	{
		switch(key)
		{
		case '1':
			ENetPacket * packet = enet_packet_create ("pocket", 
				strlen ("pocket") + 1, 
				ENET_PACKET_FLAG_RELIABLE);

			enet_host_broadcast (m_server, 0, packet);
			enet_host_flush (m_server);
			break;
		}
	}
	else if(m_state == 2)
	{
		switch(key)
		{
		case '1':
			ENetPacket * packet = enet_packet_create ("packet", 
				strlen ("packet") + 1, 
				ENET_PACKET_FLAG_RELIABLE);

			enet_peer_send (m_peer, 0, packet);
			enet_host_flush (m_client);
			break;
		}
	}
}


void NetworkScene::print(const std::string& text)
{
	cLabel* line = new cLabel(m_font.get());
	line->setString(text);
	m_labels.push_back(line);	
	m_camera->m_frontLayer->addChild(line);
}

void NetworkScene::clear()
{
	for(auto* label : m_labels) 
	{
		m_camera->m_frontLayer->deleteChild(label);
	}

	m_labels.clear();
}