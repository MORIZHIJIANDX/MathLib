#pragma once

#include <memory>
#include <mutex>

namespace Dash
{
	template<typename T>
	class Singleton
	{
	private:
		Singleton<T>(const Singleton<T>&) = delete;
		Singleton<T>& operator = (const Singleton<T>&) = delete;

	protected:
		Singleton<T>() = default;

		static std::unique_ptr<T> mInstance;
		static std::once_flag mOnce;

	public:
		virtual ~Singleton<T>() = default;

		template<typename... Args>
		static T* Get(Args&& ...args)
		{
			std::call_once(mOnce, [&]() {
				mInstance.reset(new T(std::forward<Args>(args)...));
			});
			return mInstance.get();
		}
	};

	template<typename T> std::unique_ptr<T> Singleton<T>::mInstance;
	template<typename T> std::once_flag Singleton<T>::mOnce;

}