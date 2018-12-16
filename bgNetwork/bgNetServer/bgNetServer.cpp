// bgNetServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>

#include "Poco\AutoPtr.h"
#include "Poco\Util\IniFileConfiguration.h"


int _tmain(int argc, _TCHAR* argv[])
{
	// 首先读取配置文件，完成以下事件
	// 读取需要加载的插件模块
	// 读取服务状态信息配置管理查询端口
	Poco::AutoPtr<Poco::Util::IniFileConfiguration> config(new Poco::Util::IniFileConfiguration("config.ini"));
	int http_port = config->getInt("HTTPSERVER.PORT", 8080);

	return 0;
}

