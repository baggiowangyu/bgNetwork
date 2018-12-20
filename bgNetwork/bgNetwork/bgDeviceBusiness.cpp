#include "bgDeviceBusiness.h"
#include "bgDeviceDef.h"
#include "bgDeviceManager.h"

// 512K�豸���棬���ڻ����������������Ϣ
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

	// �������߶�ʮһ����׷�ӵ��豸��������
	device_msg_buffer_->write(msg_data, msg_len);

	// �жϵ�ǰ�������Ƿ������������ݰ�
	while (true)
	{
		bool has_packet = HasPacket();
		if (!has_packet)
		{
			// ����������������������������
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
			// �յ�������
			errCode = HandleHeartBeat(packet_data, packet_data_len, response_data, response_data_len, need_response);

			// �����ﴦ��������صķ���ֵ��Ϣ
			break;
		case CMDID_PostDeviceInfo:
			// �յ��豸��Ϣ
			errCode = HandleDeviceInfo(packet_data, packet_data_len, response_data, response_data_len, need_response);
			break;
		case CMDID_PostGpsInfo:
			// �յ��豸��λ��Ϣ
			errCode = HandleDeviceLocation(packet_data, packet_data_len, response_data, response_data_len, need_response);
			break;
		case CMDID_PostSosInfo:
			// �յ�SOS��Ϣ
			errCode = HandleDeviceSos(packet_data, packet_data_len, response_data, response_data_len, need_response);
			break;
		case CMDID_PostDeviceState:
			// �յ��豸״̬��Ϣ
			errCode = HandleDeviceState(packet_data, packet_data_len, response_data, response_data_len, need_response);
			break;
		default:
			// ������Ϣ��������
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
	//std::cout<<"�յ���������..."<<std::endl;

	GxxGmDevMsgHeatBeat_V1 *heartbeat = (GxxGmDevMsgHeatBeat_V1 *)msg_data;

	if (device_gbcode_.empty())
	{
		// ȡ���豸ID�����棬���Ҹ�֪���������豸����
		device_gbcode_ = heartbeat->device_serial_;
		device_manager_->DeviceOnline(device_gbcode_);
	}
	else
	{
		// ����Ƿ���ͬһ���豸���͵���Ϣ��������ǣ�����������ݴ��ң����Խ��豸������
		if (device_gbcode_.compare(heartbeat->device_serial_) != 0)
			return ERR_HANDLEMSG_NOT_SAME_DEVICE_ID;
	}

	// ���´��豸���һ�η�����������ʱ��ڵ㣬ʹ��Tick������
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
	std::cout<<"�յ��豸������Ϣ..."<<std::endl;

	// �ϱ��۲��ߣ��յ��豸��Ϣ

	return errCode;
}

int bgDevice::HandleDeviceState(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	std::cout<<"�յ��豸״̬��Ϣ..."<<std::endl;

	// �ϱ��۲��ߣ�

	return errCode;
}

int bgDevice::HandleDeviceLocation(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	std::cout<<"�յ��豸��λ��Ϣ..."<<std::endl;

	// �ϱ��۲��ߣ�

	return errCode;
}

int bgDevice::HandleDeviceSos(const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	std::cout<<"�յ��豸SOS..."<<std::endl;

	// �ϱ��۲��ߣ�

	return errCode;
}

bool bgDevice::HasPacket()
{
	//std::cout<<"��黺�������Ƿ����һ��������Э���..."<<std::endl;
	if (device_msg_buffer_->used() < GxxGmDevMsgHead_V1_Size)
		return false;

	GxxGmDevMsgHead_V1 *packet_header = (GxxGmDevMsgHead_V1 *)device_msg_buffer_->begin();
	if (device_msg_buffer_->used() < packet_header->packet_size_)
		return false;

	return true;
}
