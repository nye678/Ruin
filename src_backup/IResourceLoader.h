#ifndef __I_RESOURCE_LOADER_H_
#define __I_RESOURCE_LOADER_H_

#include <string>
#include <memory>

namespace Ruin
{
	class IResourceLoader;
	typedef std::shared_ptr<IResourceLoader> ResourceLoaderPtr;

	/* IResourceLoader 
	 * Interface for Resource Loaders. A resource loader loads or unloads resources.*/
	class IResourceLoader
	{
	public:
		/* loadResource
		 * Loads a resource into memory. Returns the resource ID that was generator
		 * for the resource. */
		virtual unsigned int loadResource(const std::string &filepath) = 0;

		/* unloadResource
		 * Unloads a resource from memory. Requires the resource ID that was generated
		 * when the resource was originally loaded. */
		virtual void unloadResource(unsigned int resourceId) = 0;
	};
}

#endif