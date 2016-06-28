#include "ObjParser.h"

#include <fstream>
#include <limits>

using namespace std;
using namespace Ruin::FileTypes;

const char* ObjParser::AddVertexCommand = "v";
const char* ObjParser::AddNormalCommand = "vn";
const char* ObjParser::AddFaceCommand = "f";
const char* ObjParser::CreateObjectCommand = "o";
const char* ObjParser::LoadMaterialLibraryCommand = "mtllib";
const char* ObjParser::UseMaterialCommand = "usemtl";
const char* ObjParser::SetSmoothShadingCommmand = "s";

const ObjParser::ParseCommand ObjParser::Commands[] = 
{
	ObjParser::ParseCommand(ObjParser::AddVertexCommand, ObjParser::AddVertex),
	ObjParser::ParseCommand(ObjParser::AddNormalCommand, ObjParser::AddNormal),
	ObjParser::ParseCommand(ObjParser::AddFaceCommand, ObjParser::AddFace),
	ObjParser::ParseCommand(ObjParser::CreateObjectCommand, ObjParser::CreateObject),
	ObjParser::ParseCommand(ObjParser::LoadMaterialLibraryCommand, ObjParser::LoadMaterialLibrary),
	ObjParser::ParseCommand(ObjParser::UseMaterialCommand, ObjParser::UseMaterial),
	ObjParser::ParseCommand(ObjParser::SetSmoothShadingCommmand, ObjParser::SetSmoothShading),
};

void ObjParser::AddVertex(std::stringstream &parameters, ObjFile &objFile)
{
	if (!objFile.CurrentObject)
	{
		return;
	}

	float x, y, z;
	parameters >> x >> y >> z;

	objFile.CurrentObject->Vertices.push_back(glm::vec3(x, y, z));
}

void ObjParser::AddNormal(std::stringstream &parameters, ObjFile &objFile)
{
	if (!objFile.CurrentObject)
	{
		return;
	}

	float i, j, k;
	parameters >> i >> j >> k;

	objFile.CurrentObject->Normals.push_back(glm::vec3(i, j, k));
}

void ObjParser::AddFace(std::stringstream &parameters, ObjFile &objFile)
{
	if (!objFile.CurrentObject)
	{
		return;
	}

	const char seperator = '/';

	std::vector<unsigned int> face;
	
	int vertexIndex, textureIndex, normalIndex;

	while (parameters.good())
	{
		parameters >> vertexIndex;
		face.push_back(vertexIndex - 1);

		parameters.ignore(1);
		if (parameters.peek() != seperator)
		{
			parameters >> textureIndex;
		}
		
		parameters.ignore(1);
		parameters >> normalIndex;
	}

	objFile.CurrentObject->Faces.push_back(face);
	objFile.CurrentObject->FaceNormalIndices.push_back(normalIndex);
}

void ObjParser::CreateObject(std::stringstream &parameters, ObjFile &objFile)
{
	string objectName;
	parameters >> objectName;

	ObjFileObject newObject;
	newObject.Name = objectName;

	objFile.Objects.push_back(newObject);
	objFile.CurrentObject = &objFile.Objects.back();
}

void ObjParser::LoadMaterialLibrary(std::stringstream &parameters, ObjFile &objFile)
{
	string filename;
	parameters >> filename;
	// Not finished.
}

void ObjParser::UseMaterial(std::stringstream &parameters, ObjFile &objFile)
{
	// Not finished.
}

void ObjParser::SetSmoothShading(std::stringstream &parameters, ObjFile &objFile)
{
	const char* OnValue = "on";

	string value;
	parameters >> value;

	objFile.CurrentObject->SmoothShading = value == OnValue;
}

void ObjParser::NullCommand(std::stringstream &parameters, ObjFile &objFile)
{}

ObjFile ObjParser::CreateModelFromObj(const std::string &filename)
{
	const size_t LineBufferSize = 256;

	fstream objFileStream = OpenFileForReading(filename);

	ObjFile objFile;
	objFile.Filename = filename;

	string commandName;
	stringstream commandStream;
	char lineBuffer[LineBufferSize];

	while (objFileStream.good())
	{
		objFileStream.getline(lineBuffer, LineBufferSize);

		commandStream.clear();
		commandStream.str(lineBuffer);
		commandStream >> commandName;
		if (commandStream.good())
		{
			auto parseCommand = FindParseCommand(commandName);
			parseCommand(commandStream, objFile);
		}
	}

	objFileStream.close();

	return objFile;
}

fstream ObjParser::OpenFileForReading(const string &filename)
{
	fstream objFileStream;
	objFileStream.open(filename, fstream::in);

	if (!objFileStream.is_open())
	{
		stringstream errorMessage;
		errorMessage << "Failed to open file " << filename << endl;

		throw new exception(errorMessage.str().c_str());
	}

	return objFileStream;
}

ObjParser::ParseFunc ObjParser::FindParseCommand(const string &command)
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