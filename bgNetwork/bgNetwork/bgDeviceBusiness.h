#ifndef _bgDeviceBusiness_H_
#define _bgDeviceBusiness_H_

#include "Poco/FIFOBuffer.h"

class bgDeviceManager;

class bgDevice
{
public:
	bgDevice(bgDeviceManager *device_manager);
	~bgDevice();

public:
	int HandleMessage(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response);

private:
	int HandleHeartBeat(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response);
	int HandleDeviceInfo(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response);
	int HandleDeviceState(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response);
	int HandleDeviceLocation(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response);
	int HandleDeviceSos(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response);

private:
	bool HasPacket();

public:
	// 设备专用缓冲区
	Poco::FIFOBuffer *device_msg_buffer_;

	// 设备国标编码
	std::string device_gbcode_;
	// 最后一次收到设备消息的时间

private:
	bgDeviceManager *device_manager_;
};

#endif//_bgDeviceBusiness_H_
