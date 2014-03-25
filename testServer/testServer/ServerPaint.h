#include "ServerNetwork.h"

class ServerPaint
{

public:

	ServerPaint(void);
	~ServerPaint(void);

	void update();

private:

	// IDs for the clients connecting for table in ServerNetwork 
	static unsigned int client_id;

	// The ServerNetwork object 
	ServerNetwork* network;
};
