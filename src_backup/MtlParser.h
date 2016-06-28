#ifndef __RUIN_MTL_PARSER_H_
#define __RUIN_MTL_PARSER_H_

#include "MtlFile.h"
#include <string>
#include <sstream>

namespace Ruin {
namespace FileTypes {

	class MtlParser
	{
		typedef void(*ParseFunc)(std::stringstream &, MtlFile &);

		struct ParseCommand {
			const std::string Command;
			const ParseFunc Execute;

			ParseCommand(const char* command, ParseFunc execute)
				: Command(command), Execute(execute)
			{}
		};

		static const ParseCommand Commands[];

		static const char* AddAmbientCommand;
		static const char* AddDiffuseCommand;
		static const char* AddSpecularCommand;
		static const char* AddIlluminationModelCommand;
		static const char* AddDissolveCommand;
		static const char* AddSpecularCoeffCommand;
		static const char* MapAmbientTextureCommand;
		static const char* MapDiffuseTextureCommand;
		static const char* MapSpecularTextureCommand;
		static const char* NewMaterialCommand;

		static void AddAmbient(std::stringstream &parameters, MtlFile &mtlFile);
		static void AddDiffuse(std::stringstream &parameters, MtlFile &mtlFile);
		static void AddSpecular(std::stringstream &parameters, MtlFile &mtlFile);
		static void AddIlluminationModel(std::stringstream &parameters, MtlFile &mtlFile);
		static void AddDissolve(std::stringstream &parameters, MtlFile &mtlFile);
		static void AddSpecularCoeff(std::stringstream &parameters, MtlFile &mtlFile);
		static void MapAmbientTexture(std::stringstream &parameters, MtlFile &mtlFile);
		static void MapDiffuseTexture(std::stringstream &parameters, MtlFile &mtlFile);
		static void MapSpecularTexture(std::stringstream &parameters, MtlFile &mtlFile);
		static void NewMaterial(std::stringstream &parameters, MtlFile &mtlFile);
		static void NullCommand(std::stringstream &parameters, MtlFile &mtlFile);

		static std::fstream OpenFileForReading(const std::string &filename);
		static ParseFunc FindParseCommand(const std::string &command);

	public:
		static MtlFile CreateMaterialFromMtl(const std::string &filename);
	};

}}

#endif