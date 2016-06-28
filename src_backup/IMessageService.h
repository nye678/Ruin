#ifndef __RUIN_I_MESSAGE_SERVICE_H_
#define __RUIN_I_MESSAGE_SERVICE_H_

#include <memory>
#include <string>

namespace Ruin
{
	class IMessageService
	{
	public:
		virtual void showMessage(const std::string &title, const std::string &text) = 0;
	};

	typedef std::shared_ptr<IMessageService> IMessageServicePtr;
}

#endif