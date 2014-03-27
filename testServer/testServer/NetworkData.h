#include <string>

#ifndef _packet_types_
#define _packet_types_

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

	INIT_CONNECTION = 0,

	ACTION_EVENT = 1,

};

struct Packet {

	unsigned int packet_type;

	//these two are just here to see what works
	char contents;
	double many_vals[30];

	//TODO when this is a part of the main code, have packets contain PaintArm objects
	//This will allow all relevant info to transfer in one go

	void serialize(char * data) {
		memcpy(data, this, sizeof(Packet));
	}

	void deserialize(char * data) {
		memcpy(this, data, sizeof(Packet));
	}
};

#endif

