/*

	Copyright (C) 2017 by Sergey A Kryukov: derived work
	http://www.SAKryukov.org
	http://www.codeproject.com/Members/SAKryukov

	Based on original work by Sergey Ryazanov:
	"The Impossibly Fast C++ Delegates", 18 Jul 2005
	https://www.codeproject.com/articles/11015/the-impossibly-fast-c-delegates

	MIT license:
	http://en.wikipedia.org/wiki/MIT_License

	Original publication: https://www.codeproject.com/Articles/1170503/The-Impossibly-Fast-Cplusplus-Delegates-Fixed

*/

#pragma once
#include "Delegate.h"
#include <list>
#include <functional>

namespace Dash
{
	template<typename RET, typename ...PARAMS>
	class MulticastDelegate<RET(PARAMS...)> final : private DelegateBase<RET(PARAMS...)> {
	public:

		MulticastDelegate() = default;
		~MulticastDelegate() {
			for (auto& element : mInvocation) delete element;
			mInvocation.clear();
		} //~MulticastDelegate

		bool IsNull() const { return mInvocation.size() < 1; }
		bool operator ==(void* ptr) const {
			return (ptr == nullptr) && this->IsNull();
		} //operator ==
		bool operator !=(void* ptr) const {
			return (ptr != nullptr) || (!this->IsNull());
		} //operator !=

		size_t size() const { return mInvocation.size(); }

		MulticastDelegate& operator =(const MulticastDelegate&) = delete;
		MulticastDelegate(const MulticastDelegate&) = delete;

		bool operator ==(const MulticastDelegate& another) const {
			if (mInvocation.size() != another.mInvocation.size()) return false;
			auto anotherIt = another.mInvocation.begin();
			for (auto it = mInvocation.begin(); it != mInvocation.end(); ++it)
				if (**it != **anotherIt) return false;
			return true;
		} //==
		bool operator !=(const MulticastDelegate& another) const { return !(*this == another); }

		bool operator ==(const Delegate<RET(PARAMS...)>& another) const {
			if (IsNull() && another.IsNull()) return true;
			if (another.IsNull() || (size() != 1)) return false;
			return (another.m_Invocation == **mInvocation.begin());
		} //==
		bool operator !=(const Delegate<RET(PARAMS...)>& another) const { return !(*this == another); }

		MulticastDelegate& operator +=(const MulticastDelegate& another) {
			for (auto& item : another.mInvocation) // clone, not copy; flattens hierarchy:
				this->mInvocation.push_back(new typename DelegateBase<RET(PARAMS...)>::InvocationElement(item->mObject, item->mStub));
			return *this;
		} //operator +=

		template <typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
		MulticastDelegate& operator +=(const LAMBDA& lambda) {
			Delegate<RET(PARAMS...)> d = Delegate<RET(PARAMS...)>::template Create<LAMBDA>(lambda);
			return *this += d;
		} //operator +=

		MulticastDelegate& operator +=(const Delegate<RET(PARAMS...)>& another) {
			if (another.IsNull()) return *this;
			this->mInvocation.push_back(new typename DelegateBase<RET(PARAMS...)>::InvocationElement(another.m_Invocation.mObject, another.m_Invocation.mStub));
			return *this;
		} //operator +=


		template <typename LAMBDA> // template instantiation is not neededm, will be deduced/inferred:
		MulticastDelegate& operator -=(const LAMBDA& lambda) {
			Delegate<RET(PARAMS...)> d = Delegate<RET(PARAMS...)>::template Create<LAMBDA>(lambda);
			return *this -= d;
		} //operator +=

		MulticastDelegate& operator -=(const Delegate<RET(PARAMS...)>& another) {
			if (another.IsNull()) return *this;

			auto it = mInvocation.begin();
			for (; it != mInvocation.end(); ++it)
			{
				if ((**it) == another.m_Invocation)
				{
					break;
				}
			}
			delete* it;
			mInvocation.erase(it);

			return *this;
		} //operator -=


		// will work even if RET is void, return values are ignored:
		// (for handling return values, see operator(..., handler))
		void operator()(PARAMS... arg) const {
			for (auto& item : mInvocation)
				(*(item->mStub))(item->mObject, arg...);
		} //operator()

		template<typename HANDLER>
		void operator()(PARAMS... arg, HANDLER handler) const {
			size_t index = 0;
			for (auto& item : mInvocation) {
				RET value = (*(item->mStub))(item->mObject, arg...);
				handler(index, &value);
				++index;
			} //loop
		} //operator()

		void operator()(PARAMS... arg, Delegate<void(size_t, RET*)> handler) const {
			operator() < decltype(handler) > (arg..., handler);
		} //operator()
		void operator()(PARAMS... arg, std::function<void(size_t, RET*)> handler) const {
			operator() < decltype(handler) > (arg..., handler);
		} //operator()

	private:

		std::list<typename DelegateBase<RET(PARAMS...)>::InvocationElement*> mInvocation;

	}; //class MulticastDelegate

} /* namespace Core */

