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

	char network_data[MAX_PACKET_SIZE];

	void update();
};

