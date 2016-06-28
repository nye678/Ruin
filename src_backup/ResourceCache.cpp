#include "ResourceCache.h"
#include "Utility.h"

#include <rapidjson\document.h>

using namespace std;
using namespace rapidjson;
using namespace Ruin;

unsigned int ResourceCache::getResource(const string &resourceName)
{
	if (resources.count(resourceName) > 0)
	{
		auto resource = resources.at(resourceName);
		if (resource.loaded)
		{
			return resource.id;
		}
		else
		{
			return loadResource(resourceName);
		}
	}

	return NULL_RESOURCE_ID;
}

unsigned int ResourceCache::loadResource(const string &resourceName)
{
	if (resources.count(resourceName) > 0)
	{
		auto resource = resources.at(resourceName);
		auto loader = loaders.at(resource.type);

		auto resourceId = loader->loadResource(resource.filepath);
		if (resourceId != NULL_RESOURCE_ID)
		{
			resource.id = resourceId;
			resource.loaded = true;
			return resourceId;
		}
	}

	return NULL_RESOURCE_ID;
}

void ResourceCache::registerResourceLoader(ResourceLoaderPtr loader, ResourceType type)
{
	loaders[type] = loader;
}

ResourceType stringToType(const string &typeString)
{
	if (typeString == "model")
	{
		return ResourceType::Model;
	}
	else if (typeString == "material")
	{
		return ResourceType::Material;
	}
	else if (typeString == "texture")
	{
		return ResourceType::Texture;
	}
	else if (typeString == "data")
	{
		return ResourceType::Data;
	}
}

void ResourceCache::readManifestFromFile(const std::string &filepath)
{
	char* buffer;
	size_t read = Utility::LoadTextFile(filepath.c_str(), buffer);

	assert(read > 0);

	Document doc;
	doc.Parse<0>(buffer);
	delete buffer;

	const Value &manifest = doc["manifest"];
	for (SizeType i = 0; i < manifest.Size(); i++)
	{
		const Value &entry = manifest[i];
		Resource resource;
		resource.tagname = entry["tag"].GetString();
		resource.filepath = entry["filepath"].GetString();

		string typeString = entry["type"].GetString();
		resource.type = stringToType(typeString);
		resource.id = 0;
		resource.loaded = false;
		
		resources.insert(make_pair(resource.tagname, resource));
	}
}

