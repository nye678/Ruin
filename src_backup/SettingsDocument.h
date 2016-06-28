#ifndef __RUIN_SETTINGSDOCUMENT_H_
#define __RUIN_SETTINGSDOCUMENT_H_

#include <string>
#include <rapidjson\document.h>
#include "Utility.h"

namespace Ruin
{
	/* A class which facilitates reading and writing settings and values
	   to and from json files. */
	class SettingsDocument
	{
		rapidjson::Document _doc;
		rapidjson::Document _defaults;
		std::string _filepath;
		bool _dirty;

	public:
		/* Determines if this settings document is a in memory document.
		   A memory document cannot be saved unless the save is explicitly
		   given a filepath. */
		bool IsMemoryDocument();

		/* Indicates that the document has been chance sense the last time it
		   was saved. */
		bool IsDirty();

		/* Loads the settings data from a json file. The filepath should be
		   the full filepath to locate the file. */
		void Load(const std::string &filepath);

		/* Saves the document writing out the current data to the original
		   file used to load the data. */
		void Save();

		/* Saves the document writing out the current data to the given file
		   file path. If the file already exists it will be overridden. */
		void Save(const std::string &filepath);

		/* Adds a new setting key value pair to the document. */
		void Add(const std::string &key, const std::string &value);

		/* Removes a setting from the document if it exists in the document. */
		void Remove(const std::string &key);

		SettingsDocument();

	private:
		SettingsDocument(const SettingsDocument &) = delete;
		SettingsDocument &operator= (const SettingsDocument &) = delete;
	};
}

#endif