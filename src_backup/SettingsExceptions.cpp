#include <sstream>
#include "SettingsExceptions.h"

using namespace std;
using namespace Ruin::Exceptions;

SettingNotFoundException::SettingNotFoundException(const string &message)
	: exception(message.c_str())
{}

SettingNotFoundException SettingNotFoundException::CreateException(
	const string &settingName)
{
	stringstream errorMessage;
	errorMessage << "Setting, " << settingName << ", could not be found.";
	
	auto exception = SettingNotFoundException(errorMessage.str());
	exception._settingName = settingName;

	return exception;
}

const string &SettingNotFoundException::getSettingName() const
{
	return _settingName;
}

InvalidSettingTypeException::InvalidSettingTypeException(const string &message)
	: exception(message.c_str())
{}

InvalidSettingTypeException InvalidSettingTypeException::CreateException(
	const std::string &settingName,
	const std::string &requestedType,
	const std::string &actualType)
{
	stringstream errorMessage;
	errorMessage << "Setting, " << settingName << ", is a " << actualType
		<< " but a " << requestedType << " was requested.";

	auto exception = InvalidSettingTypeException(errorMessage.str());
	exception._settingName = settingName;
	exception._actualType = actualType;
	exception._requestedType = requestedType;

	return exception;
}

const string &InvalidSettingTypeException::getSettingName() const
{
	return _settingName;
}

const string &InvalidSettingTypeException::getActualType() const
{
	return _actualType;
}

const string &InvalidSettingTypeException::getRequestedType() const
{
	return _requestedType;
}