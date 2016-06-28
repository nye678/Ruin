#ifndef __RUIN_SETTINGS_PROXY
#define __RUIN_SETTINGS_PROXY

#include <memory>

#include "ISettingsService.h"
#include "ServiceProxy.h"

namespace Ruin
{
	class SettingsProxy;
	typedef std::shared_ptr<SettingsProxy> SettingsProxyPtr;

	/* Settings Proxy
	 * Provides a proxy interface to the settings service if no settings service
	 * has been registered yet. */
	class SettingsProxy : public ServiceProxy<ISettingsService>, public ISettingsService
	{
		/* Load Settings From File */
		void loadSettingsFromFile(const std::string &filename);

		/* Register Setting */
		void registerSetting(
			const std::string &name,
			const std::string &initialValue,
			const std::string &defaultValue);

		/* Get Setting */
		std::string getSetting(const std::string &setting);

		/* Get Int Setting */
		int getIntSetting(const std::string &setting);

		/* Get Double Setting */
		double getDoubleSetting(const std::string &setting);

		/* Get Bool Setting */
		bool getBoolSetting(const std::string &setting);
	};
}

#endif