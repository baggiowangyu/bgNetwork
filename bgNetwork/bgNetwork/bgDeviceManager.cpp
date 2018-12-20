#include "bgDeviceManager.h"
#include "bgDeviceDef.h"
#include "bgDeviceBusiness.h"

bgDeviceManager::bgDeviceManager()
{
	// �����ʱ��Ϳ�ʼ
}

bgDeviceManager::~bgDeviceManager()
{
	//
}

int bgDeviceManager::HandleMessage(const char *client_tag, const char *msg_data, int msg_len, char **response_data, int *response_data_len, bool *need_response)
{
	int errCode = 0;
	
	// �ȼ���Ƿ���ڶ�Ӧtag���豸����
	std::map<std::string, bgDevice*>::iterator iter = devices_.find(client_tag);
	if (iter != devices_.end())
	{
		// �ҵ��豸�ˣ��ӽ�ȥ����
		bgDevice *device = iter->second;
		errCode = device->HandleMessage(msg_data, msg_len, response_data, response_data_len, need_response);
	}
	else
	{
		// û�ҵ��豸���½�һ���豸
		bgDevice *device = new bgDevice(this);
		errCode = device->HandleMessage(msg_data, msg_len, response_data, response_data_len, need_response);
	}

	// ��������豸��������û����Ч���ݣ�����Э��ͷ�����ˣ�������Ӧ�ý��豸������

	return errCode;
}