#include "HighResolutionTimer.h"

namespace Dash
{
	HighResolutionTimer::HighResolutionTimer()
		: mElapsedTime(0), mDeltaTime(0)
	{
		mStart = std::chrono::steady_clock::now();
	}

	void HighResolutionTimer::Update()
	{
		//double currCount = (std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - mStart)).count();
		double currCount = (std::chrono::duration<double>(std::chrono::steady_clock::now() - mStart)).count();
		mDeltaTime = currCount - mElapsedTime;
		mElapsedTime = currCount;
	}

	double HighResolutionTimer::ElapsedSeconds() const
	{
		//return mElapsedTime * 0.000001;
		return mElapsedTime;
	}

	double HighResolutionTimer::ElapsedMilliSeconds() const
	{
		return mElapsedTime * 0.001;
	}

	double HighResolutionTimer::ElapsedMicroSeconds() const
	{
		return mElapsedTime;
	}

	double HighResolutionTimer::DeltaSeconds() const
	{
		//return mDeltaTime * 0.000001;
		return mDeltaTime;
	}

	double HighResolutionTimer::DeltaMilliSeconds() const
	{
		return mDeltaTime * 0.001;
	}

	double HighResolutionTimer::DeltaMicroSeconds() const
	{
		return mDeltaTime;
	}
}