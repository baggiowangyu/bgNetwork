#ifndef _bgPluginTemplate_H_
#define _bgPluginTemplate_H_

class bgPluginTemplate
{
public:
	virtual int Initialize(Poco::AutoPtr<Poco::Util::IniFileConfiguration> config) = 0;
	virtual int Start();
	virtual int Stop();
};

#endif//_bgPluginTemplate_H_
