#ifndef __RUIN_PNG_PARSER_H_
#define __RUIN_PNG_PARSER_H_

#include "PngFile.h"

#include <string>

namespace Ruin {
namespace FileTypes {

	class PngParser
	{
	public:
		PngFile ParsePngFile(const std::string &filepath);
	};

}}

#endif