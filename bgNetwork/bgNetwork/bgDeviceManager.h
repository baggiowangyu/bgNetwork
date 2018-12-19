#ifndef _bgDeviceManager_H_
#define _bgDeviceManager_H_

#include <map>
#include "Poco/Thread.h"

#define ERR_HANDLEMSG_SUCCESS		0
//#define ERR_HANDLEMSG_PROTOCOL_HEADER
#define ERR_HANDLEMSG_NOT_A_PACKET	1

class bgDevice;

class bgDeviceManager
{
public:
	bgDeviceManager();
	~bgDeviceManager();

public:
	int HandleMessage(const char *client_tag, const unsigned char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response);

public:
	std::map<std::string, bgDevice*> devices_;
	Poco::Thread heartbeat_check_thread_;
};

#endif//_bgDeviceManager_H_
