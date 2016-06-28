#include "SettingsDocument.h"

using namespace std;
using namespace Ruin;

SettingsDocument::SettingsDocument()
	: _filepath(""), _dirty(false)
{
}

bool SettingsDocument::IsMemoryDocument()
{
	return _filepath.length == 0;
}

bool SettingsDocument::IsDirty()
{
	return _dirty;
}

void SettingsDocument::Load(const string &filepath)
{
	char* buffer;
	size_t read = Utility::LoadTextFile(filepath.c_str(), buffer);

	if (read > 0)
	{
		_doc.Parse<0>(buffer);
		delete buffer;

		_dirty = false;
		_filepath = filepath;
	}
}

void SettingsDocument::Save()
{

}

void SettingsDocument::Save(const string &filepath)
{

}

void Add(const std::string &key, const std::string &value)
{

}

void Remove(const std::string &key)
{

}