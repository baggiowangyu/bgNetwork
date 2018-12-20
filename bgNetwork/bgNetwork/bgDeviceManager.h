#ifndef _bgDeviceManager_H_
#define _bgDeviceManager_H_

#include <map>
#include "Poco/Thread.h"

class bgDevice;

class bgDeviceManager
{
public:
	bgDeviceManager();
	~bgDeviceManager();

public:
	int HandleMessage(const char *client_tag, const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response);

public:
	// 设备上线
	int DeviceOnline();

public:
	std::map<std::string, bgDevice*> devices_;
	Poco::Thread heartbeat_check_thread_;
};

#endif//_bgDeviceManager_H_
