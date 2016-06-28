#include "SettingsService.h"

using namespace std;
using namespace rapidjson;
using namespace Ruin;
using namespace Ruin::Utility;

const char* SettingsService::DefaultSettings = 
R"json({
	"WindowWidth": 800,
	"WindowHeight": 600,
	"Fullscreen": false,
	"Title": "Ruin"
})json";

SettingsService::SettingsService()
{}

void SettingsService::Initialize()
{
	LoadDefaultSettings();
}

void SettingsService::LoadSettings(const string &filepath)
{
	char* buffer;
	size_t read = LoadTextFile(filepath.c_str(), buffer);

	if (read > 0)
	{
		_doc.Parse<0>(buffer);
		delete buffer;
	}
	else
	{
		_doc.Parse<0>(DefaultSettings);
	}
}

void SettingsService::SaveSettings(const string &filepath)
{
	//TODO: Figure out how to serialize with rapidjson.
}

void SettingsService::RegisterSetting(
	const std::string &name,
	const std::string &initialValue,
	const std::string &defaultValue)
{
	if (!_doc.HasMember(name.c_str()))
	{
		_doc.AddMember<const char*>(name.c_str(), initialValue.c_str(), _doc.GetAllocator());
	}
	else
	{
		_doc[name.c_str()].SetString(initialValue.c_str(), initialValue.length());
	}

	if (!_defaults.HasMember(name.c_str()))
	{
		_defaults.AddMember<const char*>(name.c_str(), defaultValue.c_str(), _doc.GetAllocator());
	}
	else
	{
		_defaults[name.c_str()].SetString(defaultValue.c_str(), defaultValue.length());
	}
}

string SettingsService::GetSetting(const string &setting)
{
	if (HasSetting(setting))
	{
		return _doc[setting.c_str()].GetString();
	}
	else
	{
		return _defaults[setting.c_str()].GetString();
	}
}

int SettingsService::GetIntSetting(const std::string &setting)
{
	if (HasSetting(setting))
	{
		return _doc[setting.c_str()].GetInt();
	}
	else
	{
		return _defaults[setting.c_str()].GetInt();
	}
}

double SettingsService::GetDoubleSetting(const string &setting)
{
	if (HasSetting(setting))
	{
		return _doc[setting.c_str()].GetDouble();
	}
	else
	{
		return _defaults[setting.c_str()].GetDouble();
	}
}

bool SettingsService::GetBoolSetting(const string &setting)
{
	if (HasSetting(setting))
	{
		return _doc[setting.c_str()].GetBool();
	}
	else
	{
		return _defaults[setting.c_str()].GetBool();
	}
}

bool SettingsService::HasSetting(const string &setting)
{
	return _doc.HasMember(setting.c_str());
}

bool SettingsService::HasDefaultSetting(const string &setting)
{
	return _defaults.HasMember(setting.c_str());
}

void SettingsService::LoadDefaultSettings()
{
	_defaults.Parse<0>(DefaultSettings);
}