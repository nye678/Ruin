#ifndef __RUIN_ID_SERVICE_H_
#define __RUIN_ID_SERVICE_H_

#include <string>
#include <memory>
#include <unordered_map>

#include "Id.h"

namespace Ruin
{
	/* Id Service
	 * Service for acquring unique ids. */
	class IdService
	{
		size_t _idSource;

	public:
		const size_t FreeRangeLowerBound = 10000000;

		/* Create Id
		 * Creates a new id from the free id range.
		 *\return	Id		The new id. */
		Id createId();
	
		IdService();
	};

	typedef std::shared_ptr<IdService> IdServicePtr;
}

#endif