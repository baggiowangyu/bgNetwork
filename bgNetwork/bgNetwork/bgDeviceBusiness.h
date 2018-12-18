#ifndef _bgDeviceBusiness_H_
#define _bgDeviceBusiness_H_

#define MAKE_MAGIC_CODE(a,b,c,d)	(a | (b<<8) | (c<<16) | (d<<24))
#define NETWORK_MAGIC_CODE			MAKE_MAGIC_CODE('G', 'X', 'X', 'H')

typedef enum
{
	CMDID_RequstHeartBeat		= 10001001,
	CMDID_ResponseHeartBeat		= 10002001,

	CMDID_RequestRealplay		= 20001001,
	CMDID_RequestTalkback		= 20001027,

	CMDID_PostVioceUpData		= 3008,

	CMDID_PostClientDeviceInfo	= 3022,
	CMDID_PostDeviceInfo	= 3220,
	CMDID_PostGpsInfo		= 3221,
	CMDID_PostSosInfo		= 3222,
	CMDID_PostDeviceState	= 3223

} enNetCmd;

#pragma pack(1)
typedef struct _GxxGmDevMsgHead_V1_
{
	unsigned int magic_;
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
