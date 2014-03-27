#include "ServerNetwork.h"
#include "NetworkData.h"

class ServerPaint
{

public:

	ServerPaint(void);
	~ServerPaint(void);

	Packet update();
	Packet receiveFromClients();
	void sendActionPackets();

	//Customize your packet before passing it to this
	void sendPacket(Packet _packet);
	void close() { network->close(); }

private:

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];
};
