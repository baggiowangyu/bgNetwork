// bgNetwork.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Poco\Net\ServerSocket.h"
#include "Poco\Net\TCPServer.h"
#include "Poco\Net\TCPServerConnectionFactory.h"
#include "Poco\Net\TCPServerConnection.h"
#include "Poco\Net\StreamSocket.h"
#include "Poco\Timespan.h"

#include "bgDeviceManager.h"
#include "bgDeviceBusiness.h"
#include "bgDeviceDef.h"

//////////////////////////////////////////////////////////////////////////
//
// 这里代表每一个客户端连接
// 其中run部分代表了
//
//////////////////////////////////////////////////////////////////////////
class bgTCPServerConnection : public Poco::Net::TCPServerConnection
{
public:
	bgTCPServerConnection(const Poco::Net::StreamSocket& s)
		: Poco::Net::TCPServerConnection(s)
	{
		// 这里应该还创建一个线程，用于发送心跳请求
		// 算了，我们不发心跳请求
	}

	void SetDeviceManager(bgDeviceManager *dev_manager) { device_manager_ = dev_manager; }

	void run(void)
	{
		char recv_buf[4096] = {0};
		std::string client_ip = socket().peerAddress().toString();
		std::cout<<"Client From : "<<client_ip.c_str()<<" connected !"<<std::endl;
		
		while (true)
		{
			try
			{
				char *response_data = NULL;
				int response_data_len = 0;
				bool need_response = false;

				while (true)
				{
					// 接收消息
					int recv_len = socket().receiveBytes(recv_buf, 4096);

					// 处理消息
					int errCode = device_manager_->HandleMessage(client_ip.c_str(), recv_buf, recv_len, &response_data, &response_data_len, &need_response);
					
					if (need_response)
					{
						socket().sendBytes(response_data, response_data_len);
						delete [] response_data;
						response_data = 0;
						need_response = false;
					}
				}
			}
			catch (Poco::Exception &e)
			{
				// 遇到异常了，记录相关的异常信息到日志
				// 告诉设备管理模块，断开连接、设备下线
				std::cout<<client_ip.c_str()<<" "<<e.what()<<std::endl;
				break;
			}
		}

		// 用client_ip.c_str()查找设备对象，然后找到设备ID
	}

public:
	bgDeviceManager *device_manager_;
};


//////////////////////////////////////////////////////////////////////////
//
// 这里代表了一个TCP服务器对象
// 同时也是TCP客户端连接工厂，负责创建一个连接对象，专门处理一个客户端连接的所有事务
//
//////////////////////////////////////////////////////////////////////////
class bgTCPServer : public Poco::Net::TCPServerConnectionFactory
{
public:
	inline Poco::Net::TCPServerConnection* createConnection(const Poco::Net::StreamSocket& socket)
	{
		bgTCPServerConnection *device_connection = new bgTCPServerConnection(socket);
		device_connection->SetDeviceManager(&device_manager_);

		return (Poco::Net::TCPServerConnection*)device_connection;
	}

public:
	// 这里需要有一个设备管理模块，用于统计在线设备数量
	// 以及需要一个线程来检查设备心跳是否超时
	bgDeviceManager device_manager_;
};


int _tmain(int argc, _TCHAR* argv[])
{
	Poco::Net::ServerSocket server_socket(9901);

	Poco::Timespan timeout(30, 0);
	server_socket.setReceiveTimeout(timeout);
	
	Poco::Net::TCPServerParams::Ptr param = new Poco::Net::TCPServerParams;
	// 设定队列的最大长度
	param->setMaxQueued(2000);

	// 如果需要持续处理一个连接的话，这里的线程数就是同时处理的设备数
	// 在TCP框架里面，这里是用一个线程池来进行处理的
	param->setMaxThreads(2000);

	Poco::Net::TCPServer server(new bgTCPServer, server_socket);

	server.start();

	getchar();

	server.stop();
	return 0;
}

