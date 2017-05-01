#include "DPJsonQuery.h"
#include "DPJsonSymbols.h"
#include "DPJsonTree.h"
#include "DPJsonNode.h"
#include <sstream>

namespace DevilsParser
{
	DPJsonQuery::DPJsonQuery(DPJsonTree* iTree)
	{
		m_tree = iTree;
		Reset();
	}

	DPJsonQuery::~DPJsonQuery()
	{
		m_tree = NULL;
		m_currentNode = NULL;
	}

	ReturnStatus DPJsonQuery::Query(const std::string iQuery, DPJsonNodeData& oResult)
	{
		if (!m_tree || "" == iQuery)
			return ReturnStatus::eError;

		std::string query = iQuery;
		ReturnStatus rs = RefineString(query);
		if (ReturnStatus::eError == rs)
			return rs;

		DPJsonNodeData data;

		if (query[0] != DPJsonSymbols::s_dot)	//Refer to first JSON object
			query = '.' + query;

		rs = ParseQuery(query, data);
		if (ReturnStatus::eError == rs)
			return rs;

		if (!ValidateData(data))
			return ReturnStatus::eError;

		oResult = data;

		return ReturnStatus::eOK;
	}

	ReturnStatus DPJsonQuery::RefineString(std::string& ioString) const
	{
		if ("" == ioString)
			return ReturnStatus::eError;

		//Remove leading and trailing spaces.
		const char* trimData = " \t\n\r\f\v";
		ioString.erase(0, ioString.find_first_not_of(trimData));
		ioString.erase(ioString.find_last_not_of(trimData) + 1);

		return ReturnStatus::eOK;
	}

	ReturnStatus DPJsonQuery::ParseQuery(const std::string iQuery, DPJsonNodeData& oResult)
	{
		if ("" == iQuery || !m_tree)
			return ReturnStatus::eError;

		ReturnStatus rs = ReturnStatus::eOK;
		int queryLength = iQuery.length();
		m_queryParsePointer = 0;
		std::string token = "";

		//Root Node Manipulation
		m_currentNode = m_currentNode->GetFirstChild();

		while (m_queryParsePointer < queryLength)
		{
			char data = iQuery[m_queryParsePointer];
			m_queryParsePointer++;
			if (DPJsonSymbols::s_dot == data)
			{
				int nextDot = iQuery.find(DPJsonSymbols::s_dot, m_queryParsePointer);

				if (std::string::npos == nextDot) 
				{
					token = iQuery.substr(m_queryParsePointer, queryLength - m_queryParsePointer);
					m_queryParsePointer = queryLength;
				}
				else 
				{
					token = iQuery.substr(m_queryParsePointer, nextDot - m_queryParsePointer);
					m_queryParsePointer = nextDot;
				}
				
				rs = RefineString(token);
				if (ReturnStatus::eError == rs)
				{
					Reset();
					return rs;
				}

				//Parse Token
				rs = ParseToken(token);
				if (ReturnStatus::eError == rs)
				{
					Reset();
					return rs;
				}
			}

			else
			{
				Reset();
				return ReturnStatus::eError;
			}
		}

		if (DPJsonNode::NodeType::eKeyValue == m_currentNode->GetNodeType())
		{
			m_currentNode = m_currentNode->GetFirstChild();
		}

		oResult = m_currentNode->GetData();

		return ReturnStatus::eOK;
	}

	ReturnStatus DPJsonQuery::ParseToken(const std::string iToken)
	{
		if ("" == iToken)
			return ReturnStatus::eError;

		ReturnStatus rs = ReturnStatus::eOK;
		std::string keyValue = "";
		int index = -1;

		int tokenParsePointer = iToken.find(DPJsonSymbols::s_openSquareBraces);
		if (std::string::npos == tokenParsePointer)
		{
			keyValue = iToken;
		}
		else
		{
			keyValue = iToken.substr(0, tokenParsePointer);
			int closedSqBrace = iToken.find(DPJsonSymbols::s_closedSquareBraces);

			tokenParsePointer++;
			std::string val = iToken.substr(tokenParsePointer, closedSqBrace - tokenParsePointer);
			rs = RefineString(val);
			if (ReturnStatus::eError == rs)
				return rs;

			std::istringstream iss(val);
			iss >> index;
		}

		//Find child with matching key node. If it is an array, go to its index.
		std::string nodeData = "";
		DPJsonNode* child = m_currentNode->GetFirstChild();
		if (DPJsonNode::NodeType::eArrayValue == child->GetNodeType() && -1 == index)
			index = 0;
		
		if (DPJsonNode::NodeType::eObjectValue == child->GetNodeType())
			child = child->GetFirstChild();

		while (NULL != child)
		{
			child->GetData(nodeData);
			if (nodeData == keyValue)
				break;

			child = child->GetNextSibling();
		}
		if (!child)
			return ReturnStatus::eError;

		m_currentNode = child;
		if (index != -1)
		{
			m_currentNode = m_currentNode->GetChild(index);
			if (!m_currentNode)
				return ReturnStatus::eError;

			return ReturnStatus::eOK;
		}

		return ReturnStatus::eOK;
	}

	bool DPJsonQuery::ValidateData(const DPJsonNodeData oResult) const
	{
		if (DPJsonNodeData::NodeDataType::eNone == oResult.IsNodeType())
			return false;

		return true;
	}

	void DPJsonQuery::Reset()
	{
		m_queryParsePointer = -1;
		if (m_tree)
			m_currentNode = m_tree->GetRootNode();
	}
}
