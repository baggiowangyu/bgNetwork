// bgNetServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>

#include "Poco\AutoPtr.h"
#include "Poco\Util\IniFileConfiguration.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// ���ȶ�ȡ�����ļ�����������¼�
	// ��ȡ��Ҫ���صĲ��ģ��
	// ��ȡ����״̬��Ϣ���ù����ѯ�˿�
	Poco::AutoPtr<Poco::Util::IniFileConfiguration> config(new Poco::Util::IniFileConfiguration("config.ini"));
	int http_port = config->getInt("HTTPSERVER.PORT", 8080);

	return 0;
}

