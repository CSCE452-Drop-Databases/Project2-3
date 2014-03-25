//#include "stdafx.h" 
#include "ServerPaint.h"

unsigned int ServerPaint::client_id;

ServerPaint::ServerPaint(void)
{
	// id's to assign clients for our table
	client_id = 0;

	// set up the server network to listen 
	network = new ServerNetwork();
}

void ServerPaint::update()
{

	// get new clients
	if (network->acceptNewClient(client_id))
	{
		printf("client %d has been connected to the server\n", client_id);

		client_id++;
	}
}

