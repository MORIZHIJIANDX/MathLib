#include "LogManager.h"
#include "ThreadSafeQueue.h"
#include "LogStream.h"

#include <vector>
#include <atomic>


namespace Dash
{
	struct LogMessage
	{
		LogLevel m_Level = LogLevel::Info;
		std::wstring m_Message;
	};

	static std::vector<std::shared_ptr<LogStream>> gsLogStreams;
	static std::mutex gsLogStreamMutex;
	static std::thread gsLogThread;
	static std::atomic_bool gsProcessMessage = true;
	static ThreadSafeQueue<LogMessage> gsMessageQueue;

	// ******************
	// LogString
	//

	LogString::~LogString()
	{
		mLogger.Log(mLogLevel, str());
	}


	LogString LogManager::operator()(LogLevel level)
	{
		return LogString(*this, level);
	}


	// ******************
	// LogManager
	//

	void ProcessMessagesFunc()
	{
		while (gsProcessMessage)
		{
			LogMessage msg;
			while (gsMessageQueue.TryPop(msg))
			{
				std::scoped_lock lock(gsLogStreamMutex);
				for (auto log : gsLogStreams)
				{
					log->Write(msg.m_Level, msg.m_Message);
				}
			}

			std::this_thread::yield();
		}
	}

	void LogManager::Init()
	{
		gsLogThread = std::thread(&ProcessMessagesFunc);
	}

	void LogManager::Shutdown()
	{
		while (!gsMessageQueue.Empty())
		{
		}

		gsProcessMessage = false;

		if (gsLogThread.joinable())
		{
			gsLogThread.join();
		}

		UnregisterAllStreams();
	}

	void LogManager::RegisterLogStream(std::shared_ptr<LogStream> logStream)
	{
		std::scoped_lock lock(gsLogStreamMutex);
		gsLogStreams.push_back(logStream);
	}

	void LogManager::UnregisterLogStream(std::shared_ptr<LogStream> logStream)
	{
		std::scoped_lock lock(gsLogStreamMutex);

		auto iter = std::find(gsLogStreams.begin(), gsLogStreams.end(), logStream);

		if (iter != gsLogStreams.end())
		{
			gsLogStreams.erase(iter);
		}
	}

	void LogManager::UnregisterAllStreams()
	{
		std::scoped_lock lock(gsLogStreamMutex);
		gsLogStreams.clear();
	}

	void LogManager::Log(LogLevel level, std::wstring logInfo)
	{
		std::wstring msg = logInfo + L"\n";
		gsMessageQueue.Push({ level, msg });
	}

}