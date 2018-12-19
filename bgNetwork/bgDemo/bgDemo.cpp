// bgDemo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include "Poco/FIFOBuffer.h"
#include "Poco/FIFOBufferStream.h"

void TestBuffer()
{

	return ;
}

void TestFIFOBuffer()
{
	Poco::FIFOBuffer fifo_buf(12);
	char buf[12] = {0};

	bool bret = fifo_buf.isWritable();
	if (!bret)
	{
		std::cout<<"FIFOBuffer 缓冲区不可写入..."<<std::endl;
		system("pause");
		return ;
	}

	for (int index = 0; index < 10; ++index)
	{
		char input_buf[12] = {0};
		sprintf_s(input_buf, 12, "0x%08d", index);
		int write_len = fifo_buf.write(input_buf, strlen(input_buf));
		int read_len = fifo_buf.read(buf, strlen(input_buf));
		fifo_buf.peek(9);
		fifo_buf.next();
	}

	return ;
}

int _tmain(int argc, _TCHAR* argv[])
{
	TestFIFOBuffer();

	return 0;
}

