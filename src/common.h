#pragma once

#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete; \
    TypeName& operator=(const TypeName&) = delete;

namespace kd {

	template <typename T>
	inline void SafeDelete(T * &pointer)
	{
		if (pointer != nullptr)
		{
			delete pointer;
			pointer = nullptr;
		}
	}
	
	template<typename ClassType>
	class Singleton
	{
	public:
		static ClassType * GetInstance();
		static void DeleteInstance();

	protected:
		Singleton();
		virtual ~Singleton();

	private:
		static ClassType * instance;
	};

	template<typename ClassType>
	ClassType * Singleton<ClassType>::instance = nullptr;

	template<typename ClassType>
	Singleton<ClassType>::Singleton() {}

	template<typename ClassType>
	Singleton<ClassType>::~Singleton() {}

	template<typename ClassType>
	ClassType * Singleton<ClassType>::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new ClassType();
		}
		return instance;
	}

	template<typename ClassType>
	void Singleton<ClassType>::DeleteInstance()
	{
		delete instance;
		instance = nullptr;
	}
}
