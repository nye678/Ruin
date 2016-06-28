#ifndef __RUIN_OBJ_PARSER_H_
#define __RUIN_OBJ_PARSER_H_

#include <glm\glm.hpp>
#include <vector>
#include <string>
#include <sstream>
#include "ObjFile.h"

namespace Ruin {
namespace FileTypes {

	class ObjParser
	{
		typedef void(*ParseFunc)(std::stringstream &, ObjFile &);

		struct ParseCommand {
			const std::string Command;
			const ParseFunc Execute;

			ParseCommand(const char* command, ParseFunc execute)
				: Command(command), Execute(execute)
			{}
		};

		static const ParseCommand Commands[];

		static const char* AddVertexCommand;
		static const char* AddNormalCommand;
		static const char* AddFaceCommand;
		static const char* CreateObjectCommand;
		static const char* LoadMaterialLibraryCommand;
		static const char* UseMaterialCommand;
		static const char* SetSmoothShadingCommmand;

		static void AddVertex(std::stringstream &parameters, ObjFile &objFile);
		static void AddNormal(std::stringstream &parameters, ObjFile &objFile);
		static void AddFace(std::stringstream &parameters, ObjFile &objFile);
		static void CreateObject(std::stringstream &parameters, ObjFile &objFile);
		static void LoadMaterialLibrary(std::stringstream &parameters, ObjFile &objFile);
		static void UseMaterial(std::stringstream &parameters, ObjFile &objFile);
		static void SetSmoothShading(std::stringstream &parameters, ObjFile &objFile);
		static void NullCommand(std::stringstream &parameters, ObjFile &objFile);

		static std::fstream OpenFileForReading(const std::string &filename);
		static ParseFunc FindParseCommand(const std::string &command);

	public:
		static ObjFile CreateModelFromObj(const std::string &filename);
	};

}}

#endif