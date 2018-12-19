#include "bgDeviceBusiness.h"

// 512K设备缓存，用于缓存服务器发来的消息
#define DEVICE_BUFFER_SIZE 512 * 1024

bgDevice::bgDevice()
: (new Poco::FIFOBuffer(DEVICE_BUFFER_SIZE))
{
	//
}

bgDevice::~bgDevice()
{
	//
}

int bgDevice::HandleMessage(const unsigned char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;

	// 不管三七二十一，先追加到设备缓存里面
	device_msg_buffer_->append(msg_data, msg_len);

	// 判断当前缓存中是否有完整的数据包
	while (true)
	{
		bool has_packet = HasPacket();
		if (!has_packet)
		{
			// 不包含完整包，跳出，继续接收
			errCode = ERR_HANDLEMSG_NOT_A_PACKET;
			break;
		}

		GxxGmDevMsgHead_V1 *protocol_header = (GxxGmDevMsgHead_V1 *)device_msg_buffer_->begin();
		int protocol_data_len = protocol_header->packet_size_;

		int packet_data_len = protocol_header->packet_size_;
		char *packet_data = new char[packet_data_len];
		device_msg_buffer_->read(packet_data, packet_data_len);
		device_msg_buffer_->next();

		switch (protocol_header->command_id_)
		{
		case CMDID_PostClientDeviceInfo:
			// 收到心跳包
			errCode = HandleHeartBeat(packet_data, packet_data_len, response_data, response_data_len, need_response);
			break;
		case CMDID_PostDeviceInfo:
			// 收到设备信息
			errCode = HandleDeviceInfo(packet_data, packet_data_len, response_data, response_data_len, need_response);
			break;
		case CMDID_PostGpsInfo:
			// 收到设备定位信息
			errCode = HandleDeviceLocation(packet_data, packet_data_len, response_data, response_data_len, need_response);
			break;
		case CMDID_PostSosInfo:
			// 收到SOS信息
			errCode = HandleDeviceSos(packet_data, packet_data_len, response_data, response_data_len, need_response);
			break;
		case CMDID_PostDeviceState:
			// 收到设备状态信息
			errCode = HandleDeviceState(packet_data, packet_data_len, response_data, response_data_len, need_response);
			break;
		default:
			// 其他信息，不处理
			break;
		}

		delete [] packet_data;
		packet_data = NULL;

	}

	return errCode;
}

int bgDevice::HandleHeartBeat(const unsigned char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;

	// 更新此设备最后一次发上来的数据时间节点，使用Tick来计算

	*need_response = true;

	return errCode;
}

int bgDevice::HandleDeviceInfo(const unsigned char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;

	return errCode;
}

int bgDevice::HandleDeviceState(const unsigned char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;

	return errCode;
}

int bgDevice::HandleDeviceLocation(const unsigned char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;

	return errCode;
}

int bgDevice::HandleDeviceSos(const unsigned char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;

	return errCode;
}


