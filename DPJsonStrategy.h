#ifndef DPJsonStrategy_h
#define DPJsonStrategy_h

#include "DPBase.h"
#include <string>

namespace DevilsParser
{
	class DPJsonTree;

	class DPJsonStrategy
	{
	public:
		struct S_StrategyCriteria 
		{
			enum ESizeStrategyCriteria 
			{
				eSmallFile = 0,
				eMediumFile,
				eLargeFile
			};
		};
		typedef S_StrategyCriteria::ESizeStrategyCriteria StrategyCriteria;

		struct S_JsonValueType
		{
			enum EJsonValueType
			{
				eString = 0,
				eInteger,
				eDouble,
				eObject,
				eArray,
				eTrue,
				eFalse,
				eNull,
				eInvalidValue,
				eEnd
			};
		};
		typedef S_JsonValueType::EJsonValueType JsonValueType;

		static DPJsonStrategy* GetStrategy(StrategyCriteria iCriteria);

		DPJsonStrategy();
		virtual ~DPJsonStrategy() = 0;

		virtual ReturnStatus Parse(std::string iData) = 0;
		virtual DPJsonTree* GetDomTree() const = 0;

	protected:
		template <typename T> 
		bool IsPrimittiveValueValid(std::string iValue);
	};
}

#endif
