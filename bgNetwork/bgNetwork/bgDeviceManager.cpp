#include "bgDeviceManager.h"
#include "bgDeviceDef.h"
#include "bgDeviceBusiness.h"

bgDeviceManager::bgDeviceManager()
{
	// 构造的时候就开始
}

bgDeviceManager::~bgDeviceManager()
{
	//
}

int bgDeviceManager::HandleMessage(const char *client_tag, const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	
	// 先检查是否存在对应tag的设备对象
	std::map<std::string, bgDevice*>::iterator iter = devices_.find(client_tag);
	if (iter != devices_.end())
	{
		// 找到设备了，扔进去处理
		bgDevice *device = iter->second;
		errCode = device->HandleMessage(msg_data, msg_len, response_data, response_data_len, need_response);
	}
	else
	{
		// 没找到设备，新建一个设备
		bgDevice *device = new bgDevice(this);
		errCode = device->HandleMessage(msg_data, msg_len, response_data, response_data_len, need_response);
	}

	// 如果发现设备缓冲区内没有有效数据，并且协议头错误了，那我们应该将设备踢下线

	return errCode;
}