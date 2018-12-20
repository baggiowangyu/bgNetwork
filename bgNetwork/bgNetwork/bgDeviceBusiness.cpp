#include "bgDeviceBusiness.h"
#include "bgDeviceDef.h"
#include "bgDeviceManager.h"

// 512K设备缓存，用于缓存服务器发来的消息
#define DEVICE_BUFFER_SIZE 512 * 1024

bgDevice::bgDevice(bgDeviceManager *device_manager)
: device_msg_buffer_(new Poco::FIFOBuffer(DEVICE_BUFFER_SIZE))
, device_manager_(device_manager)
{
	//
}

bgDevice::~bgDevice()
{
	//
}

int bgDevice::HandleMessage(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;

	// 不管三七二十一，先追加到设备缓存里面
	device_msg_buffer_->write(msg_data, msg_len);

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

			// 在这里处理心跳相关的返回值信息
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

int bgDevice::HandleHeartBeat(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	//std::cout<<"收到保活心跳..."<<std::endl;

	GxxGmDevMsgHeatBeat_V1 *heartbeat = (GxxGmDevMsgHeatBeat_V1 *)msg_data;

	if (device_gbcode_.empty())
	{
		// 取出设备ID，保存，并且告知管理器，设备上线
		device_gbcode_ = heartbeat->device_serial_;
		device_manager_->DeviceOnline(device_gbcode_);
	}
	else
	{
		// 检查是否是同一个设备发送的信息，如果不是，则可能是数据错乱，可以将设备踢下线
		if (device_gbcode_.compare(heartbeat->device_serial_) != 0)
			return ERR_HANDLEMSG_NOT_SAME_DEVICE_ID;
	}

	// 更新此设备最后一次发上来的数据时间节点，使用Tick来计算
	*response_data_len = sizeof(GxxGmDevMsgHead_V1);
	*response_data = new char[*response_data_len];
	GxxGmDevMsgHead_V1 *header = (GxxGmDevMsgHead_V1 *)(*response_data);

	header->magic_ = NETWORK_MAGIC_CODE;
	header->command_id_ = CMDID_RequstHeartBeat;
	header->packet_size_ = *response_data_len;

	*need_response = true;

	return errCode;
}

int bgDevice::HandleDeviceInfo(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	std::cout<<"收到设备基本信息..."<<std::endl;

	// 上报观察者，收到设备信息

	return errCode;
}

int bgDevice::HandleDeviceState(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	std::cout<<"收到设备状态信息..."<<std::endl;

	// 上报观察者，

	return errCode;
}

int bgDevice::HandleDeviceLocation(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	std::cout<<"收到设备定位信息..."<<std::endl;

	// 上报观察者，

	return errCode;
}

int bgDevice::HandleDeviceSos(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	std::cout<<"收到设备SOS..."<<std::endl;

	// 上报观察者，

	return errCode;
}

bool bgDevice::HasPacket()
{
	//std::cout<<"检查缓冲区内是否存在一个完整的协议包..."<<std::endl;
	if (device_msg_buffer_->used() < GxxGmDevMsgHead_V1_Size)
		return false;

	GxxGmDevMsgHead_V1 *packet_header = (GxxGmDevMsgHead_V1 *)device_msg_buffer_->begin();
	if (device_msg_buffer_->used() < packet_header->packet_size_)
		return false;

	return true;
}
