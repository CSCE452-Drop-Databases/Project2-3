#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

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
	Packet update();

	void close() { network->close(); }
};

