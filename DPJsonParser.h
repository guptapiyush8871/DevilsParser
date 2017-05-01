#ifndef DPJsonParser_h
#define DPJsonParser_h

#include "DPBase.h"
#include "DPJsonStrategy.h"
#include "DPJsonNodeData.h"
#include <string>

namespace DevilsParser 
{
	class DPJsonTree;
	class DPJsonQuery;

	class DPJsonParser
	{
	public:
		struct S_ParserMode 
		{
			enum EParserMode 
			{
				eRead = 0,
				eWrite
			};
		};
		typedef S_ParserMode::EParserMode ParserMode;

		DPJsonParser(const char* iPath, ParserMode iMode);
		DPJsonParser(std::string iData, ParserMode iMode);

		virtual ~DPJsonParser();

		/*
		* This method works on the read mode only.
		*/
		ReturnStatus Parse();

		ReturnStatus Query(const std::string iQuery, DPJsonNodeData& oValue);
		
	private:
		DPJsonStrategy* GetStrategy();
		std::string ReadFile(std::string iPath);

		/*
		*	Return size in bytes
		*/
		long long unsigned int GetJsonDataSize() const;
		DPJsonStrategy::StrategyCriteria GetStrategyCriteria(long long unsigned iDataSize) const;

		ParserMode m_parserMode;
		std::string m_data;
		std::string m_path;
		DPJsonStrategy* m_strategy;
		DPJsonTree* m_domTree;
		DPJsonQuery* m_query;
	};
}

#endif
