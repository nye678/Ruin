#include "MtlParser.h"
#include <fstream>
#include <glm\glm.hpp>

using namespace std;
using namespace glm;
using namespace Ruin;
using namespace Ruin::FileTypes;

const char* MtlParser::AddAmbientCommand = "Ka";
const char* MtlParser::AddDiffuseCommand = "Kd";
const char* MtlParser::AddDissolveCommand = "d";
const char* MtlParser::AddIlluminationModelCommand = "illum";
const char* MtlParser::AddSpecularCoeffCommand = "Ns";
const char* MtlParser::AddSpecularCommand = "Ks";
const char* MtlParser::MapAmbientTextureCommand = "map_Ka";
const char* MtlParser::MapDiffuseTextureCommand = "map_Kd";
const char* MtlParser::MapSpecularTextureCommand = "map_Ks";

const MtlParser::ParseCommand MtlParser::Commands[] = {
	MtlParser::ParseCommand(MtlParser::AddAmbientCommand, MtlParser::AddAmbient),
	MtlParser::ParseCommand(MtlParser::AddDiffuseCommand, MtlParser::AddDiffuse),
	MtlParser::ParseCommand(MtlParser::AddDissolveCommand, MtlParser::AddDissolve),
	MtlParser::ParseCommand(MtlParser::AddIlluminationModelCommand, MtlParser::AddIlluminationModel),
	MtlParser::ParseCommand(MtlParser::AddSpecularCoeffCommand, MtlParser::AddSpecularCoeff),
	MtlParser::ParseCommand(MtlParser::AddSpecularCommand, MtlParser::AddSpecular),
	MtlParser::ParseCommand(MtlParser::MapAmbientTextureCommand, MtlParser::MapAmbientTexture),
	MtlParser::ParseCommand(MtlParser::MapDiffuseTextureCommand, MtlParser::MapDiffuseTexture),
	MtlParser::ParseCommand(MtlParser::MapSpecularTextureCommand, MtlParser::MapSpecularTexture)
};

void MtlParser::AddAmbient(stringstream &parameters, MtlFile &mtlFile)
{
	float r, g, b;
	parameters >> r >> g >> b;

	mtlFile.Ambient = vec3(r, g, b);
}

void MtlParser::AddDiffuse(stringstream &parameters, MtlFile &mtlFile)
{
	float r, g, b;
	parameters >> r >> g >> b;

	mtlFile.Diffuse = vec3(r, g, b);
}

void MtlParser::AddSpecular(stringstream &parameters, MtlFile &mtlFile)
{
	float r, g, b;
	parameters >> r >> g >> b;

	mtlFile.Specular = vec3(r, g, b);
}

void MtlParser::AddSpecularCoeff(stringstream &parameters, MtlFile &mtlFile)
{
	float coeff;
	parameters >> coeff;

	mtlFile.SpecularCoefficent = coeff;
}

void MtlParser::AddIlluminationModel(stringstream &parameters, MtlFile &mtlFile)
{
	int illumModel;
	parameters >> illumModel;

	mtlFile.IlluminationModel = (MtlIlluminationModel)illumModel;
}

void MtlParser::AddDissolve(stringstream &parameters, MtlFile &mtlFile)
{
	float dissolve;
	parameters >> dissolve;

	mtlFile.Transparency = dissolve;
}

void MtlParser::MapAmbientTexture(stringstream &parameter, MtlFile &mtlFile)
{

}

void MtlParser::MapDiffuseTexture(stringstream &parameter, MtlFile &mtlFile)
{

}

void MtlParser::MapSpecularTexture(stringstream &parameter, MtlFile &mtlFile)
{

}

void MtlParser::NewMaterial(stringstream &parameters, MtlFile &mtlFile)
{
	string name;
	parameters >> name;

	mtlFile.Name = name;
}

void MtlParser::NullCommand(stringstream &parameters, MtlFile &mtlFile)
{}

MtlFile MtlParser::CreateMaterialFromMtl(const string &filename)
{
	const size_t LineBufferSize = 256;

	fstream mtlFileStream = OpenFileForReading(filename);

	MtlFile mtlFile;

	string commandName;
	stringstream commandStream;
	char lineBuffer[LineBufferSize];

	while (mtlFileStream.good())
	{
		mtlFileStream.getline(lineBuffer, LineBufferSize);

		commandStream.clear();
		commandStream.str(lineBuffer);
		commandStream >> commandName;
		if (commandStream.good())
		{
			auto parseCommand = FindParseCommand(commandName);
			parseCommand(commandStream, mtlFile);
		}
	}

	mtlFileStream.close();

	return mtlFile;
}

fstream MtlParser::OpenFileForReading(const string &filename)
{
	fstream mtlFileStream;
	mtlFileStream.open(filename, fstream::in);

	if (!mtlFileStream.is_open())
	{
		stringstream errorMessage;
		errorMessage << "Failed to open file " << filename << endl;

		throw new exception(errorMessage.str().c_str());
	}

	return mtlFileStream;
}

MtlParser::ParseFunc MtlParser::FindParseCommand(const string &command)
{
	for (auto parseCommand : Commands)
	{
		if (command == parseCommand.Command)
		{
			return parseCommand.Execute;
		}
	}

	return NullCommand;
}