// bgNetworkClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include "Poco/LocalDateTime.h"


int _tmain(int argc, _TCHAR* argv[])
{
	Poco::Net::StreamSocket stream_socket;
	Poco::Net::SocketAddress server_addr("127.0.0.1", 9901);
	Poco::Timespan timeout(30, 0);

	try
	{
		// 连接服务器
		stream_socket.connect(server_addr, timeout);
	}
	catch (Poco::Exception &e)
	{
		std::cout<<e.what()<<std::endl;
	}

	// 设置发送与接收超时时间
	stream_socket.setReceiveTimeout(timeout);
	stream_socket.setSendTimeout(timeout);

	// 设置接收缓冲区长度和发送缓冲区长度
	stream_socket.setReceiveBufferSize(4096);
	stream_socket.setSendBufferSize(4096);

	stream_socket.setNoDelay(true);

	while (true)
	{
		try
		{
			//if (stream_socket.available())
			{
				Poco::LocalDateTime current_datetime;
				char buffer[4096] = {0};
				sprintf_s(buffer, 4096, "[%d-%02d-%02d %02d:%02d:%02d.%03d] Client Data ...",
					current_datetime.year(), current_datetime.month(), current_datetime.day(),
					current_datetime.hour(), current_datetime.minute(), current_datetime.second(),
					current_datetime.microsecond());

				// 取得当前时间，发送给服务器
				int send_len = stream_socket.sendBytes(buffer, strlen(buffer) + 1);

				char recv_buffer[4096] = {0};
				int recv_len = stream_socket.receiveBytes(recv_buffer, 4096);
				std::cout<<"Recv "<<recv_len<<"bytes data : "<<recv_buffer<<std::endl;
			}
		}
		catch (Poco::Exception &e)
		{
			std::cout<<e.what()<<std::endl;
		}
	}

	stream_socket.close();

	return 0;
}

