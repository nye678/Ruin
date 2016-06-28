#ifndef __RUIN_RGO_FILE_H_
#define __RUIN_RGO_FILE_H_

#include <string>
#include <rapidjson\document.h>
#include "Entity.h"

namespace Ruin {
namespace FileTypes {

	class RgoFile
	{
		std::string _fileName;
		rapidjson::Document _rgoDoc;

	public:
		void LoadRgoFile(const std::string &filename);

		Entity* CreateEntityFromData();
	};

}}

#endif