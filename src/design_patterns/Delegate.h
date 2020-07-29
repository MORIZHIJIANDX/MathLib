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
#include "DelegateBase.h"

namespace Dash
{
	template <typename T> class Delegate;
	template <typename T> class MulticastDelegate;

	template<typename RET, typename ...PARAMS>
	class Delegate<RET(PARAMS...)> final : private DelegateBase<RET(PARAMS...)> {
	public:

		Delegate() = default;

		bool IsNull() const { return mInvocation.mStub == nullptr; }
		bool operator ==(void* ptr) const {
			return (ptr == nullptr) && this->IsNull();
		} //operator ==
		bool operator !=(void* ptr) const {
			return (ptr != nullptr) || (!this->IsNull());
		} //operator !=

		Delegate(const Delegate& another) { another.mInvocation.Clone(mInvocation); }

		template <typename LAMBDA>
		Delegate(const LAMBDA& lambda) {
			Assign((void*)(&lambda), LambdaStub<LAMBDA>);
		} //Delegate

		Delegate& operator =(const Delegate& another) {
			another.mInvocation.Clone(mInvocation);
			return *this;
		} //operator =

		template <typename LAMBDA> // template instantiation is not needed, will be deduced (inferred):
		Delegate& operator =(const LAMBDA& instance) {
			Assign((void*)(&instance), LambdaStub<LAMBDA>);
			return *this;
		} //operator =

		bool operator == (const Delegate& another) const { return mInvocation == another.mInvocation; }
		bool operator != (const Delegate& another) const { return mInvocation != another.mInvocation; }

		bool operator ==(const MulticastDelegate<RET(PARAMS...)>& another) const { return another == (*this); }
		bool operator !=(const MulticastDelegate<RET(PARAMS...)>& another) const { return another != (*this); }

		template <class T, RET(T::* TMethod)(PARAMS...)>
		static Delegate Create(T* instance) {
			return Delegate(instance, MethodStub<T, TMethod>);
		} //Create

		template <class T, RET(T::* TMethod)(PARAMS...) const>
		static Delegate Create(T const* instance) {
			return Delegate(const_cast<T*>(instance), ConstMethodStub<T, TMethod>);
		} //Create

		template <RET(*TMethod)(PARAMS...)>
		static Delegate Create() {
			return Delegate(nullptr, FunctionStub<TMethod>);
		} //Create

		template <typename LAMBDA>
		static Delegate Create(const LAMBDA& instance) {
			return Delegate((void*)(&instance), LambdaStub<LAMBDA>);
		} //Create

		RET operator()(PARAMS... arg) const {
			return (*mInvocation.mStub)(mInvocation.mObject, arg...);
		} //operator()

	private:

		Delegate(void* anObject, typename DelegateBase<RET(PARAMS...)>::stub_type aStub) {
			mInvocation.mObject = anObject;
			mInvocation.mStub = aStub;
		} //Delegate

		void Assign(void* anObject, typename DelegateBase<RET(PARAMS...)>::stub_type aStub) {
			this->mInvocation.mObject = anObject;
			this->mInvocation.mStub = aStub;
		} //Assign

		template <class T, RET(T::* TMethod)(PARAMS...)>
		static RET MethodStub(void* this_ptr, PARAMS... params) {
			T* p = static_cast<T*>(this_ptr);
			return (p->*TMethod)(params...);
		} //MethodStub

		template <class T, RET(T::* TMethod)(PARAMS...) const>
		static RET ConstMethodStub(void* this_ptr, PARAMS... params) {
			T* const p = static_cast<T*>(this_ptr);
			return (p->*TMethod)(params...);
		} //ConstMethodStub

		template <RET(*TMethod)(PARAMS...)>
		static RET FunctionStub(void* this_ptr, PARAMS... params) {
			return (TMethod)(params...);
		} //FunctionStub

		template <typename LAMBDA>
		static RET LambdaStub(void* this_ptr, PARAMS... arg) {
			LAMBDA* p = static_cast<LAMBDA*>(this_ptr);
			return (p->operator())(arg...);
		} //LambdaStub

		friend class MulticastDelegate<RET(PARAMS...)>;
		typename DelegateBase<RET(PARAMS...)>::InvocationElement mInvocation;

	}; //class Delegate

} /* namespace Core */
