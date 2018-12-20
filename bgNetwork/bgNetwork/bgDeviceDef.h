#ifndef _bgDeviceDef_H_
#define _bgDeviceDef_H_

#define ERR_HANDLEMSG_SUCCESS		0
//#define ERR_HANDLEMSG_PROTOCOL_HEADER
#define ERR_HANDLEMSG_NOT_A_PACKET	1

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
#define GxxGmDevMsgHead_V1_Size sizeof(GxxGmDevMsgHead_V1)

typedef struct _GxxGmDevMsgHeatBeat_V1_
{
	unsigned char csq_type_;	// 15:3G  17:4G 0:368 1:tk1
	unsigned char csq_value_;	// 
	unsigned char con_status_;	// 1:未注册 2:拨号中; 3:连接正常 此为无线连接状态 不是和中心的通信状态
	unsigned char mode_status_;	// 0成功 1失败
	char device_serial_[32];	// 
};

#pragma pack()

#endif//_bgDeviceDef_H_
