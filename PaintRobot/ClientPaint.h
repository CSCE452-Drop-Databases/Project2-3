#include "stdafx.h"

class ClientPaint
{

public:

	ClientPaint();
	~ClientPaint(void);

	ClientNetwork* network;

	void sendActionPackets();

	//Customize your packet before passing it to this
	void sendPacket(Packet _packet);

	char network_data[MAX_PACKET_SIZE];

	//This should return a packet containing the latest information
	Packet update(Packet _outgoing_packet);

	void close() { network->close(); }
};

