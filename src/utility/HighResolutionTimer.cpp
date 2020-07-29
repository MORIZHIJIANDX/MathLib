#include "HighResolutionTimer.h"

namespace Dash
{
	HighResolutionTimer::HighResolutionTimer()
		: mElapsedTimeTime(0), mDeltaTime(0)
	{
		mStart = std::chrono::steady_clock::now();
	}

	void HighResolutionTimer::Update()
	{
		double currCount = (std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - mStart)).count();
		mDeltaTime = currCount - mElapsedTimeTime;
		mElapsedTimeTime = currCount;
	}

	double HighResolutionTimer::ElapsedSeconds() const
	{
		return mElapsedTimeTime * 0.000001;
	}

	double HighResolutionTimer::ElapsedMilliSeconds() const
	{
		return mElapsedTimeTime * 0.001;
	}

	double HighResolutionTimer::ElapsedMicroSeconds() const
	{
		return mElapsedTimeTime;
	}

	double HighResolutionTimer::DeltaSeconds() const
	{
		return mDeltaTime * 0.000001;
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