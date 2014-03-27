
class ServerPaint
{

public:

	ServerPaint(void);
	~ServerPaint(void);

	Packet update(Packet _outgoing_packet);
	Packet receiveFromClients(Packet _outgoing_packet);
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
