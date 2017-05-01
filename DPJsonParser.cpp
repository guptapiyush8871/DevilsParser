#include "DPJsonParser.h"
#include "DPJsonSimpleStrategy.h"
#include "DPJsonTree.h"
#include "DPJsonQuery.h"
#include <fstream>
#include <string>
#include <iostream>

namespace DevilsParser
{
	namespace 
	{
		enum EFileSize 
		{
			eSmallSize = 1024 * 1024 * 10,		//size less than 10MB
			eMediumSize = 1024 * 1024 * 500,	//10MB < size <= 500MB
		};
	}

	DPJsonParser::DPJsonParser(const char* iPath, ParserMode iMode)
	{
		m_parserMode = iMode;
		m_path = iPath;
		m_data = ReadFile(m_path);
		m_strategy = GetStrategy();
		m_domTree = NULL;
		m_query = NULL;
	}

	DPJsonParser::DPJsonParser(std::string iData, ParserMode iMode)
	{
		m_parserMode = iMode;
		m_data = iData;
		m_strategy = GetStrategy();
		m_domTree = NULL;
		m_query = NULL;
	}

	DPJsonParser::~DPJsonParser()
	{
		m_domTree = NULL;

		if (m_strategy)
			delete m_strategy;
		m_strategy = NULL;

		if(m_query)
			delete m_query;
		m_query = NULL;
	}

	ReturnStatus DPJsonParser::Parse()
	{
		if (!m_strategy || "" == m_data || m_parserMode != ParserMode::eRead)
			return ReturnStatus::eError;

		ReturnStatus rs = m_strategy->Parse(m_data);
		if (ReturnStatus::eOK == rs)
			m_domTree = m_strategy->GetDomTree();

		return rs;
	}

	DPJsonStrategy* DPJsonParser::GetStrategy()
	{
		long long unsigned int dataSize = GetJsonDataSize();
		DPJsonStrategy::StrategyCriteria criteria = GetStrategyCriteria(dataSize);
		m_strategy = DPJsonStrategy::GetStrategy(criteria);
		return m_strategy;
	}

	std::string DPJsonParser::ReadFile(std::string iPath)
	{
		//long long unsigned int fileSize = GetJsonDataSize();	//According to size file reading strategy can be decided.

		if ("" == iPath)
			return "";

		std::string data = "";
		std::string line = "";

		std::ifstream inFile(iPath);
		if (inFile.is_open())
		{
			while (std::getline(inFile, line))
			{
				data += line;
			}
		}

		return data;
	}

	long long unsigned int DPJsonParser::GetJsonDataSize() const
	{
		if ("" == m_path)
			return 0;

		if (m_data != "")
			return m_data.size();

		std::ifstream inFile(m_path, std::ios::ate | std::ios::binary);
		return inFile.tellg();
	}

	ReturnStatus DPJsonParser::Query(const std::string iQuery, DPJsonNodeData& oValue)
	{
		if ("" == iQuery || !m_domTree)
			return ReturnStatus::eError;

		if (!m_query)
			m_query = new DPJsonQuery(m_domTree);

		ReturnStatus rs = m_query->Query(iQuery, oValue);

		return rs;
	}

	DPJsonStrategy::StrategyCriteria DPJsonParser::GetStrategyCriteria(long long unsigned iDataSize) const
	{
		if (iDataSize <= EFileSize::eSmallSize)	//size less than 10MB
		{
			return DPJsonStrategy::StrategyCriteria::eSmallFile;
		}
		else if (iDataSize > EFileSize::eSmallSize && iDataSize <= EFileSize::eMediumSize)	//10MB < size <= 500MB
		{
			return DPJsonStrategy::StrategyCriteria::eMediumFile;
		}
		else // size > 500MB
		{
			return DPJsonStrategy::StrategyCriteria::eLargeFile;
		}
	}
}
