// bgNetwork.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Poco\Net\ServerSocket.h"
#include "Poco\Net\TCPServer.h"
#include "Poco\Net\TCPServerConnectionFactory.h"
#include "Poco\Net\TCPServerConnection.h"
#include "Poco\Net\StreamSocket.h"
#include "Poco\Timespan.h"


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

	}

	void run(void)
	{
		char recv_buf[4096] = {0};
		std::string client_ip = socket().peerAddress().toString();
		std::cout<<"Client From : "<<client_ip.c_str()<<" connected !"<<std::endl;
		
		while (true)
		{
			try
			{
				int recv_len = socket().receiveBytes(recv_buf, 4096);
				//std::cout<<"Recv "<<recv_len<<" bytes data : "<<recv_buf<<std::endl;

				// 这里可以增加一个业务处理入口，将接收到的数据传进去，由业务模块负责缓冲数据包，拆分协议包，任务分发

				int send_len = socket().sendBytes(recv_buf, recv_len);
				//std::cout<<"Send "<<recv_len<<" bytes data : "<<recv_buf<<std::endl;
			}
			catch (Poco::Exception &e)
			{
				std::cout<<client_ip.c_str()<<" "<<e.what()<<std::endl;
				break;
			}
		}
	}
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
		return new bgTCPServerConnection(socket);
	}
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

