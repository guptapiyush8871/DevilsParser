#include "DPLogger.h"
#include <iostream>

namespace DevilsParser
{
	DPLogger::DPLogger()
	{
		std::ofstream clearFile;
		clearFile.open("Log.txt", std::ios::out | std::ios::trunc);
		clearFile.close();

		m_stream.open("Log.txt", std::ios::out | std::ios::app);
	}

	DPLogger::~DPLogger()
	{
	}

	DPLogger& DPLogger::GetLogger()
	{
		static DPLogger s_logger;
		return s_logger;
	}

	void DPLogger::LogToConsole(const std::string iString)
	{
		std::cout << iString << std::endl;
	}

	void DPLogger::LogToConsole(const int iData)
	{
		std::cout << iData << std::endl;
	}

	void DPLogger::LogToConsole(const float iData)
	{
		std::cout << iData << std::endl;
	}

	void DPLogger::LogToConsole(const double iData)
	{
		std::cout << iData << std::endl;
	}

	void DPLogger::LogToFile(const std::string iString)
	{
		m_stream << iString << std::endl;
	}

	void DPLogger::LogToFile(const int iData)
	{
		m_stream << iData << std::endl;
	}

	void DPLogger::LogToFile(const float iData)
	{
		m_stream << iData << std::endl;
	}

	void DPLogger::LogToFile(const double iData)
	{
		m_stream << iData << std::endl;
	}

}
