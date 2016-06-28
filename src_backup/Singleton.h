#ifndef __RUIN_SINGLETON_H_
#define __RUIN_SINGLETON_H_

#include <cassert>
#include <memory>

namespace Ruin
{
	/* Templated Singleton pattern implementation. To use create an
	   instance with the class you want to singletonize. If you want
	   you can make that class' constructor private and add the
	   Singleton class as a friend. */
	template<typename T>
	class Singleton
	{
		static std::shared_ptr<T> Instance;

	public:
		/* Creates the singleton instance. */
		static void Create();

		/* Gets a shared pointer to the singleton object. */
		static std::shared_ptr<T> Get();

	private:
		Singleton();
		Singleton(const Singleton &) = delete;
		Singleton &operator= (const Singleton &) = delete;

	public:
		~Singleton();
	};

	template<typename T>
	std::shared_ptr<T> Singleton<T>::Instance = nullptr;

	template<typename T>
	Singleton<T>::Singleton()
	{}

	template<typename T>
	Singleton<T>::~Singleton()
	{}

	template<typename T>
	void Singleton<T>::Create()
	{
		if (Instance == nullptr)
		{
			Instance = std::shared_ptr<T>(new T());
		}
	}

	template<typename T>
	std::shared_ptr<T> Singleton<T>::Get()
	{
		assert(Instance != nullptr);
		return Instance;
	}
}

#endif