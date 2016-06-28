#ifndef __RUIN_NUMBER_RANGE_H_
#define __RUIN_NUMBER_RANGE_H_

#include <memory>

namespace Ruin
{
	template <typename T>
	class NumberRange
	{
		std::shared_ptr<T> _data;

		std::shared_ptr<NumberRange> _next;
		std::shared_ptr<NumberRange> _prev;

		size_t _lowerBounds;
		size_t _upperBounds;

	public:
		std::shared_ptr<NumberRange> InsertRange(size_t lowerBounds, size_t upperBounds);

		NumberRange()
	};

}

#endif