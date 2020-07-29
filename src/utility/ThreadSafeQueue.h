#pragma once

#include <queue>
#include <mutex>

namespace Dash
{
	template<typename T>
	class ThreadSafeQueue
	{
	public:
		ThreadSafeQueue() noexcept;
		ThreadSafeQueue(const ThreadSafeQueue& copy) noexcept;

		/**
		 * Push a value into the back of the queue.
		 */
		void Push(T value) noexcept;

		/**
		 * Try to pop a value from the front of the queue.
		 * @returns false if the queue is empty.
		 */
		bool TryPop(T& value) noexcept;

		/**
		 * Check to see if there are any items in the queue.
		 */
		bool Empty() const noexcept;

		/**
		 * Retrieve the number of items in the queue.
		 */
		size_t Size() const noexcept;

	private:
		std::queue<T> mQueue;
		mutable std::mutex mMutex;
	};

	template<typename T>
	ThreadSafeQueue<T>::ThreadSafeQueue() noexcept
	{}

	template<typename T>
	ThreadSafeQueue<T>::ThreadSafeQueue(const ThreadSafeQueue<T>& copy) noexcept
	{
		std::lock_guard<std::mutex> lock(copy.mMutex);
		mQueue = copy.mQueue;
	}

	template<typename T>
	void ThreadSafeQueue<T>::Push(T value) noexcept
	{
		std::lock_guard<std::mutex> lock(mMutex);
		mQueue.push(std::move(value));
	}

	template<typename T>
	bool ThreadSafeQueue<T>::TryPop(T& value) noexcept
	{
		std::lock_guard<std::mutex> lock(mMutex);
		if (mQueue.empty())
			return false;

		value = mQueue.front();
		mQueue.pop();

		return true;
	}

	template<typename T>
	bool ThreadSafeQueue<T>::Empty() const noexcept
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mQueue.empty();
	}

	template<typename T>
	size_t ThreadSafeQueue<T>::Size() const noexcept
	{
		std::lock_guard<std::mutex> lock(mMutex);
		return mQueue.size();
	}
}