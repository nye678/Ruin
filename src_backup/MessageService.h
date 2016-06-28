#ifndef __RUIN_MESSAGE_SERVICE_H_
#define __RUIN_MESSAGE_SERVICE_H_

#ifdef _WIN32
	#include <Windows.h>
#elif __APPLE__
	// TODO: Include something for Apple.
#elif __linux
	// TODO: Include something for Linux.
#endif

#include <string>
#include "Singleton.h"

namespace Ruin
{
	// Service for displaying error and debug messages.
	class MessageService
	{
		friend class Singleton<MessageService>;

	public:
		/* Show Message
		 * Displays a error or debug message to the user. Not suitable for general
		 * in game use. Messages are displayed use the native OS api's.*/
		static void DisplayNativeDialog(const std::string &title, const std::string &text);

	private:
		MessageService();
		MessageService(const MessageService &) = delete;
		MessageService &operator= (const MessageService &) = delete;
	};
}

#endif