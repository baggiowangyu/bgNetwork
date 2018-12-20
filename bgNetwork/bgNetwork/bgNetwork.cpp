// bgNetwork.cpp : �������̨Ӧ�ó������ڵ㡣
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
		// ����Ӧ�û�����һ���̣߳����ڷ�����������
		// ���ˣ����ǲ�����������
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
					// ������Ϣ
					int recv_len = socket().receiveBytes(recv_buf, 4096);

					// ������Ϣ
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
				// �����쳣�ˣ���¼��ص��쳣��Ϣ����־
				// �����豸����ģ�飬�Ͽ����ӡ��豸����
				std::cout<<client_ip.c_str()<<" "<<e.what()<<std::endl;
				break;
			}
		}

		// ��client_ip.c_str()�����豸����Ȼ���ҵ��豸ID
	}

public:
	bgDeviceManager *device_manager_;
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
		bgTCPServerConnection *device_connection = new bgTCPServerConnection(socket);
		device_connection->SetDeviceManager(&device_manager_);

		return (Poco::Net::TCPServerConnection*)device_connection;
	}

public:
	// ������Ҫ��һ���豸����ģ�飬����ͳ�������豸����
	// �Լ���Ҫһ���߳�������豸�����Ƿ�ʱ
	bgDeviceManager device_manager_;
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

