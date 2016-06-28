#include "SettingsProxy.h"

using namespace std;
using namespace Ruin;

void SettingsProxy::loadSettingsFromFile(const string &filename)
{
	if (serviceExists())
	{
		_service->loadSettingsFromFile(filename);
	}
}

void SettingsProxy::registerSetting(
	const std::string &name,
	const std::string &initialValue,
	const std::string &defaultValue)
{
	if (serviceExists())
	{
		_service->registerSetting(name, initialValue, defaultValue);
	}
}

string SettingsProxy::getSetting(const string &setting)
{
	if (serviceExists())
	{
		return _service->getSetting(setting);
	}
	
	return "";
}

int SettingsProxy::getIntSetting(const string &setting)
{
	if (serviceExists())
	{
		return _service->getIntSetting(setting);
	}

	return 0;
}

double SettingsProxy::getDoubleSetting(const std::string &setting)
{
	if (serviceExists())
	{
		return _service->getDoubleSetting(setting);
	}

	return 0.0;
}

bool SettingsProxy::getBoolSetting(const std::string &setting)
{
	if (serviceExists())
	{
		return _service->getBoolSetting(setting);
	}

	return false;
}