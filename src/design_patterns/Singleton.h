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

		static std::unique_ptr<T> m_Instance;
		static std::once_flag m_Once;

	public:
		virtual ~Singleton<T>() = default;

		template<typename... Args>
		static T* Get(Args&& ...args)
		{
			std::call_once(m_Once, [&]() {
				m_Instance.reset(new T(std::forward<Args>(args)...));
			});
			return m_Instance.get();
		}
	};

	template<typename T> std::unique_ptr<T> Singleton<T>::m_Instance;
	template<typename T> std::once_flag Singleton<T>::m_Once;

}