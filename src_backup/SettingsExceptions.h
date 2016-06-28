#ifndef __RUIN_SETTING_EXCEPTIONS_H_
#define __RUIN_SETTING_EXCEPTIONS_H_

#include <exception>
#include <string>

namespace Ruin {
namespace Exceptions {

	class SettingNotFoundException : public std::exception
	{
		std::string _settingName;

	public:
		const std::string &getSettingName() const;

		SettingNotFoundException(const std::string &message);

		static SettingNotFoundException CreateException(
			const std::string &settingName);
	};

	class InvalidSettingTypeException : public std::exception
	{
		std::string _settingName;
		std::string _requestedType;
		std::string _actualType;

	public:
		const std::string &getSettingName() const;
		const std::string &getRequestedType() const;
		const std::string &getActualType() const;

		InvalidSettingTypeException(const std::string &message);

		static InvalidSettingTypeException CreateException(
			const std::string &settingName, 
			const std::string &requestedType, 
			const std::string &actualType);
	};

}}

#endif