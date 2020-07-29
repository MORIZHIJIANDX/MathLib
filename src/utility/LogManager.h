#pragma once

#include <sstream>
#include "LogEnums.h"
#include "../design_patterns/Singleton.h"

namespace Dash
{
	class LogManager;
	class LogStream;

	class LogString : public std::wostringstream
	{
	public:
		LogString(LogManager& logger, LogLevel level)
			: mLogger(logger), mLogLevel(level)
		{}

		LogString(const LogString& ls)
			: mLogger(ls.mLogger), mLogLevel(ls.mLogLevel)
		{}

		~LogString();

	private:
		LogManager& mLogger;
		LogLevel mLogLevel;
	};

	class LogManager : public Singleton<LogManager>
	{
		friend class LogString;
	public:
		LogManager() {}

		virtual ~LogManager() {};

		LogString operator()(LogLevel level);

		void Init();
		void Shutdown();

		void RegisterLogStream(std::shared_ptr<LogStream> logStream);
		void UnregisterLogStream(std::shared_ptr<LogStream> logStream);
		void UnregisterAllStreams();

	private:
		void Log(LogLevel level, std::wstring logInfo);
	};
}

#define LOG_INFO (Dash::LogManager::Get()->operator()(Core::LogLevel::Info)) << "[File]: " << __FILE__ << "  [Line]: " << __LINE__ << "  [Function]: " << __FUNCTION__ << "  [Info]: "
#define LOG_WARNING (Dash::LogManager::Get()->operator()(Core::LogLevel::Warning)) << "[File]: " << __FILE__ << "  [Line]: " << __LINE__ << "  [Function]: " << __FUNCTION__ << "  [Warning]: "
#define LOG_ERROR (Dash::LogManager::Get()->operator()(Core::LogLevel::Error)) << "[File]: " << __FILE__ << "  [Line]: " << __LINE__ << "  [Function]: " << __FUNCTION__ << "  [Error]: "

