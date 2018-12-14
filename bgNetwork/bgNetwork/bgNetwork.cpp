// bgNetwork.cpp : �������̨Ӧ�ó������ڵ㡣
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
// �������ÿһ���ͻ�������
// ����run���ִ�����
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

				// �����������һ��ҵ������ڣ������յ������ݴ���ȥ����ҵ��ģ�鸺�𻺳����ݰ������Э���������ַ�

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
// ���������һ��TCP����������
// ͬʱҲ��TCP�ͻ������ӹ��������𴴽�һ�����Ӷ���ר�Ŵ���һ���ͻ������ӵ���������
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
	// �趨���е���󳤶�
	param->setMaxQueued(2000);

	// �����Ҫ��������һ�����ӵĻ���������߳�������ͬʱ������豸��
	// ��TCP������棬��������һ���̳߳������д����
	param->setMaxThreads(2000);

	Poco::Net::TCPServer server(new bgTCPServer, server_socket);

	server.start();

	getchar();

	server.stop();
	return 0;
}

