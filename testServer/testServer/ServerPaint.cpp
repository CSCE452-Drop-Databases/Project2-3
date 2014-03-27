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

Packet ServerPaint::update()
{

	// get new clients
	if (network->acceptNewClient(client_id))
	{
		printf("client %d has been connected to the server\n", client_id);

		client_id++;
	}
	return receiveFromClients();
}


//modify this for different messages
Packet ServerPaint::receiveFromClients()
{
	//TODO return the packet object from function

	Packet packet;

	// go through all clients
	std::map<unsigned int, SOCKET>::iterator iter;

	for (iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
	{
		int data_length = network->receiveData(iter->first, network_data);

		if (data_length <= 0)
		{
			//no data recieved
			continue;
		}

		int i = 0;
		while (i < (unsigned int)data_length)
		{
			packet.deserialize(&(network_data[i]));
			i += sizeof(Packet);

			Packet pak;

			switch (packet.packet_type) {

			case INIT_CONNECTION:

				printf("server received init packet from client\n");

				//sendActionPackets();
				//Packet pak;
				pak.packet_type = ACTION_EVENT;
				pak.contents = 'B';
				sendPacket(pak);
				return packet;
				break;

			case ACTION_EVENT:

				printf("server received action event packet from client: %c\n", packet.contents);

				//sendActionPackets();
				//Packet pak;
				pak.packet_type = ACTION_EVENT;
				pak.contents = 'B';
				sendPacket(pak);
				return packet;
				break;

			default:

				printf("error in packet types\n");

				break;
			}
		}
	}
	
	//ready or not, we must return something!
	return packet;
}

void ServerPaint::sendActionPackets()
{
	// send action packet
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	Packet packet;
	packet.packet_type = ACTION_EVENT;

	packet.contents = 'B';
	packet.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}

void ServerPaint::sendPacket(Packet _packet) {
	const unsigned int packet_size = sizeof(Packet);
	char packet_data[packet_size];

	_packet.serialize(packet_data);

	network->sendToAll(packet_data, packet_size);
}