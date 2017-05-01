#include "DPJsonStrategy.h"
#include "DPJsonSimpleStrategy.h"
#include <sstream>

namespace DevilsParser
{
	DPJsonStrategy* DPJsonStrategy::GetStrategy(StrategyCriteria iCriteria)
	{
		DPJsonStrategy* strategy = 0;

		switch (iCriteria) 
		{
		case StrategyCriteria::eSmallFile:
			strategy = new DPJsonSimpleStrategy();
			break;

		case StrategyCriteria::eMediumFile:
			strategy = new DPJsonSimpleStrategy();	//Extend for medium size files.
			break;

		case StrategyCriteria::eLargeFile:
			strategy = new DPJsonSimpleStrategy();	//Extend for large size files.
			break;

		default:
			strategy = new DPJsonSimpleStrategy();
			break;
		}

		return strategy;
	}

	DPJsonStrategy::DPJsonStrategy()
	{
	}

	DPJsonStrategy::~DPJsonStrategy()
	{
	}

	template <typename T>
	bool DPJsonStrategy::IsPrimittiveValueValid(std::string iValue)
	{
		std::istringstream iss(iValue);
		T val;
		char c;
		return ((iss >> val) && !(iss >> c));
	}

	template bool DPJsonStrategy::IsPrimittiveValueValid<int>(std::string iValue);
	template bool DPJsonStrategy::IsPrimittiveValueValid<double>(std::string iValue);
}
