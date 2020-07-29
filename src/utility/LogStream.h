#pragma once

#include "LogEnums.h"
#include <string>
#include <mutex>
#include <fstream>

namespace Dash
{
	class LogStream
	{
	public:
		virtual void Write(LogLevel level, std::wstring logInfo) = 0;
	};


	class LogStreamFile : public LogStream
	{
	public:
		LogStreamFile(std::wstring fileName);
		virtual ~LogStreamFile();

		virtual void Write(LogLevel level, std::wstring logInfo) override;

	private:
		std::wofstream mFileStream;
		std::mutex mFileMutex;
	};


	class LogStreamConsole : public LogStream
	{
	public:
		LogStreamConsole();
		~LogStreamConsole() = default;

		virtual void Write(LogLevel level, std::wstring logInfo) override;
	};


	class LogStreamVS : public LogStream
	{
	public:
		LogStreamVS() = default;
		~LogStreamVS() = default;

		virtual void Write(LogLevel level, std::wstring logInfo) override;
	};


	class LogStreamMessageBox : public LogStream
	{
	public:
		LogStreamMessageBox(LogLevel filter);
		~LogStreamMessageBox() = default;

		virtual void Write(LogLevel level, std::wstring logInfo) override;

	private:
		LogLevel mLevelFilter;
	};

}