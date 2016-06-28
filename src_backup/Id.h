#ifndef __RUIN_ID_H_
#define __RUIN_ID_H_

#include <memory>
#include <functional>

namespace Ruin
{
	class Id;
	typedef std::shared_ptr<Id> IdPtr;

	class Id
	{
		size_t _id;

	public:
		Id(size_t id) : _id(id) {}

		operator size_t() const { return _id; }
	};
}

namespace std
{
	template<>
	struct hash<Ruin::Id>
	{
		size_t operator() (Ruin::Id const &id) const
		{
			return (size_t)id;
		}
	};
}

#endif