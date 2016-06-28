#ifndef __RUIN_SERVICES_H_
#define __RUIN_SERVICES_H_

#include <cassert>
#include <memory>

#include "Singleton.h"
#include "SettingsService.h"
#include "ResourceCache.h"
#include "MessageService.h"
#include "CameraService.h"

namespace Ruin
{
	/* Services
	 * Services assists in finding global services. Services are initially
	 * stubbed out as proxies until a real service is registered. */
	static class Services
	{
		static Singleton<SettingsService> SettingsInstance;
		static Singleton<ResourceCache> ResourceCacheInstance;
		static Singleton<MessageService> MessageServiceInstance;
		static Singleton<CameraService> CameraServiceInstance;

	public:
		/* Creates the singleton instance of services. */
		static void Initialize();

		/* Gets the settings service. */
		static inline std::shared_ptr<SettingsService> Settings()
		{
			return SettingsInstance.Get();
		}

		/* Gets the resource cache service. */
		static inline std::shared_ptr<ResourceCache> ResourceCache()
		{
			return ResourceCacheInstance.Get();
		}

		/* Gets the messaging service. */
		static inline std::shared_ptr<MessageService> Messages()
		{
			return MessageServiceInstance.Get();
		}

		static inline std::shared_ptr<CameraService> Cameras()
		{
			return CameraServiceInstance.Get();
		}

		~Services();
	};

}

#endif