#ifndef DPLogger_h
#define DPLogger_h

#include <string>
#include <fstream>

namespace DevilsParser 
{
#define LOG_TO_FILE(MESSAGE)	\
	DPLogger::GetLogger().LogToFile(#MESSAGE);

#define LOG_TO_CONSOLE(MESSAGE)	\
	DPLogger::GetLogger().LogToConsole(#MESSAGE);

#define LOG_TO_FILE_NEW(MESSAGE)	\
	DPLogger::GetLogger().LogToFile(MESSAGE);

	class DPLogger
	{
	public:
		~DPLogger();
		static DPLogger& GetLogger();
		void LogToConsole(const std::string iString);
		void LogToConsole(const int iData);
		void LogToConsole(const float iData);
		void LogToConsole(const double iData);

		void LogToFile(const std::string iString);
		void LogToFile(const int iData);
		void LogToFile(const float iData);
		void LogToFile(const double iData);

	private:
		DPLogger();
		DPLogger(const DPLogger&);
		DPLogger& operator=(const DPLogger&);

		std::ofstream m_stream;
	};

}

#endif
