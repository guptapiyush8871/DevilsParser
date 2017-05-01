#ifndef DPJsonQuery_h
#define DPJsonQuery_h

#include "DPBase.h"
#include "DPJsonNodeData.h"
#include <string>

namespace DevilsParser 
{
	class DPJsonTree;
	class DPJsonNode;

	class DPJsonQuery 
	{
	public :
		DPJsonQuery(DPJsonTree* iTree);
		virtual ~DPJsonQuery();
		ReturnStatus Query(const std::string iQuery, DPJsonNodeData& oResult);
		void Reset();

	private :
		ReturnStatus RefineString(std::string& ioString) const;
		ReturnStatus ParseQuery(const std::string iQuery, DPJsonNodeData& oResult);
		ReturnStatus ParseToken(const std::string iToken);
		bool ValidateData(const DPJsonNodeData oResult) const;

		DPJsonTree* m_tree;
		DPJsonNode* m_currentNode;
		int m_queryParsePointer;
	};
}

#endif
