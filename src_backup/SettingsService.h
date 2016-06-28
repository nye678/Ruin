#ifndef __RUIN_SETTINGS_H_
#define __RUIN_SETTINGS_H_

#include <sstream>
#include <memory>
#include <rapidjson\document.h>

#include "Singleton.h"
#include "Utility.h"

namespace Ruin {

	/* Settings
	 * Settings service implementation. Using json documents as a backing
	 * for saving and loading settings. */
	class SettingsService
	{
		friend class Singleton<SettingsService>;

		static const char* DefaultSettings;

		rapidjson::Document _doc;
		rapidjson::Document _defaults;

	public:
		/*Initializes the settings with the default values. Settings will need to be
		  initialized before they can be used. */
		void Initialize();

		/*Populates settings by reading a json file. If the file does not exist, or contains malformed data,
		  the default settings will be set. 
		  param		string		The full file path of the settings file. */
		void LoadSettings(const std::string &filepath);

		/*Writes the current settings to a json file. If the file does not exist it will be created. If the
		  file does exist then it will be over written.
		  param		string		The full file path of the settings file. */
		void SaveSettings(const std::string &filepath);

		/*Adds a new setting value. New settings will be recorded when the settings are saved.
		  param		string		The name of the new setting.
		  param		string		The initial value of the setting.
		  param		string		The default value of the setting. */
		void RegisterSetting(
			const std::string &name,
			const std::string &initialValue,
			const std::string &defaultValue);

		/*Gets the specified setting from the settings document and returns the value as a string.
		  param		string		The name of the setting to get.
		  return	string		The value of the setting as a string. */
		std::string GetSetting(const std::string &setting);
		
		/*Gets the specified setting from the settings document and returns the value as an integer.
		  param		string		The name of the setting to get.
		  return	int			The value of the setting as an integer. */
		int GetIntSetting(const std::string &setting);

		/*Gets the specified setting from the settings document and returns
		  the value as a double.
		  param		string		The name of the setting to get.
		  return	double		The value of the setting as a double. */
		double GetDoubleSetting(const std::string &setting);

		/*Gets the specified setting from the settings document and returns
		  the value as a boolean.
		  param		string		The name of the setting to get.
		  return	bool		The value of the setting as a boolean. */
		bool GetBoolSetting(const std::string &setting);

	private:
		/*Tests of the given setting exists or not.
		  param		string		The name of the setting.
		  return	bool		True if the setting exists. */
		bool HasSetting(const std::string &setting);

		/*Tests of the given setting has a default or not.
		param		string		The name of the setting.
		return		bool		True if the setting default exists. */
		bool HasDefaultSetting(const std::string &setting);

		/*Loads the default settings. */
		void LoadDefaultSettings();

		SettingsService();
		SettingsService(const SettingsService &) = delete;
		SettingsService &operator= (const SettingsService &) = delete;
	};
}

#endif