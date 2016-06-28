#ifndef __RUIN_RESOURCE_CACHE_H_
#define __RUIN_RESOURCE_CACHE_H_

#include <memory>
#include <string>
#include <unordered_map>

#include "IResourceLoader.h"

namespace Ruin
{
	// Resource Type
	// Generic resource types. Determines which resource loader is used to load the resource.
	enum ResourceType
	{
		Model,
		Material,
		Texture,
		Data,
	};

	// Resource Cache
	// Resource cache helps facilitate the loading and referencing of resources. Various loader modules
	// will need to be registered with the resource cache for each type of resource.
	class ResourceCache
	{
		struct Resource
		{
			unsigned int id;
			ResourceType type;
			std::string tagname;
			std::string filepath;
			bool loaded;
		};

		typedef std::unordered_map<ResourceType, ResourceLoaderPtr> ResourceLoaderMap;
		typedef std::unordered_map<std::string, Resource> ResourceTagMap;

		ResourceLoaderMap loaders;
		ResourceTagMap resources;

	public:
		const unsigned int NULL_RESOURCE_ID = 0;

		// getResource
		// Gets a resource from the resource tag map. If the resource does not exist
		// then a zero is returned. If the resource is not loaded, then it will be loaded.
		unsigned int getResource(const std::string &resourceName);

		// loadResource
		// Loads a resource if the resource has already been loaded it is reloaded. The resource
		// will use the appropriate loader based on it's resource type. Will return the
		// id of the loaded resource.
		unsigned int loadResource(const std::string &resourceName);

		// registerResourceLoader
		// Registers the resource loader with the resource cache. A loader of a particular
		// type will need to be registered before resources of that type can be loaded.
		void registerResourceLoader(ResourceLoaderPtr loader, ResourceType type);

		// readManifestFromFile
		// Populates the tag map with resources from a manifest file.
		// This should be done before trying to get or load any resources.
		void readManifestFromFile(const std::string &filepath);
	};
}

#endif