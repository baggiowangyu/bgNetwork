#ifndef _bgDeviceBusiness_H_
#define _bgDeviceBusiness_H_

#pragma pack(1)
typedef struct _GxxGmDevMsgHead_V1_
{
	unsigned char magic_[4];
	unsigned int packet_size_;
	unsigned int command_id_;

} GxxGmDevMsgHead_V1, *PGxxGmDevMsgHead_V1;

typedef struct _GxxGmDevMsgHeatBeat_V1_
{

};

#pragma pack()

class bgDeviceBusiness
{
public:
	bgDeviceBusiness();
	~bgDeviceBusiness();

public:
	int HandleMessage(const unsigned char *msg_data, int msg_len);

private:
	int HandleHeartBeat();
	int HandleDeviceInfo();
	int HandleDeviceState();
	int HandleDeviceLocation();
	int HandleDeviceSos();
};

#endif//_bgDeviceBusiness_H_
