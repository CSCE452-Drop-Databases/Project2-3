#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"

class ClientPaint
{

public:

	ClientPaint();
	~ClientPaint(void);

	ClientNetwork* network;
};

