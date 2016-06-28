#ifndef __RUIN_I_SETTINGS_SERVICE_H_
#define __RUIN_I_SETTINGS_SERVICE_H_

#include <string>
#include <memory>

namespace Ruin {

	class ISettingsService;
	typedef std::shared_ptr<ISettingsService> ISettingsServicePtr;

	class ISettingsService
	{
	public:
		/* Load Settings From File
		* Populates settings by reading a file. If the file does not exist, or
		* contains malformed data, the default settings will be set.
		*\param		string		The full file name of the settings file. */
		virtual void loadSettingsFromFile(const std::string &filename)=0;

		/* Register Setting
		 * Adds a new setting.
		 *\param	string		The name of the new setting.
		 *\param	string		The initial value of the setting.
		 *\param	string		The default value of the setting.*/
		virtual void registerSetting(
			const std::string &name, 
			const std::string &initialValue, 
			const std::string &defaultValue)=0;

		/* Get Setting
		* Gets the specified setting from the settings document and returns
		* the value as a string.
		*\param		string		The name of the setting to get.
		*\return	string		The value of the setting as a string. */
		virtual std::string getSetting(const std::string &setting)=0;

		/* Get Int Setting
		* Gets the specified setting from the settings document and returns
		* the value as an integer.
		*\param	string		The name of the setting to get.
		*\return	int			The value of the setting as an integer. */
		virtual int getIntSetting(const std::string &setting)=0;

		/* Get Double Setting
		* Gets the specified setting from the settings document and returns
		* the value as a double.
		*\param		string		The name of the setting to get.
		*\return	double		The value of the setting as a double. */
		virtual double getDoubleSetting(const std::string &setting)=0;

		/* Get Bool Setting
		* Gets the specified setting from the settings document and returns
		* the value as a boolean.
		*\param		string		The name of the setting to get.
		*\return	bool		The value of the setting as a boolean. */
		virtual bool getBoolSetting(const std::string &setting)=0;
	};

}

#endif