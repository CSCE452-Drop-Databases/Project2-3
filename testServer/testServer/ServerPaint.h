#include "ServerNetwork.h"
#include "NetworkData.h"

class ServerPaint
{

public:

	ServerPaint(void);
	~ServerPaint(void);

	void update();
	void receiveFromClients();
	void sendActionPackets();

private:

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;

	// data buffer
	char network_data[MAX_PACKET_SIZE];
};
