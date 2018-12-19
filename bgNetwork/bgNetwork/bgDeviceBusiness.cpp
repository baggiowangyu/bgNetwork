#include "bgDeviceBusiness.h"

// 512K�豸���棬���ڻ����������������Ϣ
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

	// �������߶�ʮһ����׷�ӵ��豸��������
	device_msg_buffer_->append(msg_data, msg_len);

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

int bgDevice::HandleHeartBeat(const unsigned char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;

	// ���´��豸���һ�η�����������ʱ��ڵ㣬ʹ��Tick������

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


