#ifndef __RUIN_SERVICE_PROXY
#define __RUIN_SERVICE_PROXY

#include <memory>

namespace Ruin
{
	/* Service Proxy
	 * Templated base class for a service proxy. A service proxy provides an interface
	 * to use a service that will never fail. Implementing classes should always do proper
	 * null checking before defererencing the service pointer and provide reasonable 
	 * error reponses. Implementing classes will want to inherit from both this class and the
	 * interface which it is proxying.
	 *\type		ServiceType		An interface to a service.*/
	template<typename ServiceType>
	class ServiceProxy
	{
	protected:
		std::shared_ptr<ServiceType> _service;

	public:
		/* Change Service Provider
		 * Changes the current service provider. The service proxy should work regardless
		 * of what service (null or otherwise) is passed in.
		 *\param	ServiceType*	A pointer to the service. */
		void changeServiceProvider(std::shared_ptr<ServiceType> service)
		{
			_service = service;
		}

		/* Change Service Provider
		* Same as above but wraps the naked pointer into a shared ptr. Do not try to delete
		* the pointer after calling this method!.
		*\param		ServiceType*	A naked pointer to the service. */
		void changeServiceProvider(ServiceType* service)
		{
			_service = std::shared_ptr<ServiceType>(service);
		}

		/* Service Exists
		 * A simple check to see if the service exists. Mostly for readability. Can be
		 * overridden if more indepth checking is required.
		 *\return	bool	Returns true if the service is good to use. */
		virtual bool serviceExists()
		{ 
			return (bool)_service;
		}
	};

}

#endif