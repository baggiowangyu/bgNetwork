#ifndef _bgDeviceManager_H_
#define _bgDeviceManager_H_

#include "Poco/Thread.h"

class bgDeviceManager
{
public:
	bgDeviceManager();
	~bgDeviceManager();

public:
	Poco::Thread heartbeat_check_thread_;
};

#endif//_bgDeviceManager_H_
