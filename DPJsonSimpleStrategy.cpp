#include "DPJsonSimpleStrategy.h"
#include "DPJsonSymbols.h"
#include "DPLogger.h"
#include <sstream>

namespace DevilsParser
{
	template <typename T>
	T GetPrimittiveData(const std::string iValue)
	{
		std::istringstream iss(iValue);
		T val;
		iss >> val;
		return val;
	}

	DPJsonSimpleStrategy::DPJsonSimpleStrategy() 
	{
		m_root = new DPJsonTree();
		m_currentParentNode = m_root->GetRootNode();
		m_streamPointer = 0;
		m_data = "";
	}

	DPJsonSimpleStrategy::~DPJsonSimpleStrategy()
	{
		Destroy();

		if (m_root)
			delete m_root;
		m_root = NULL;
	}

	ReturnStatus DPJsonSimpleStrategy::Parse(std::string iData)
	{
		if ("" == iData)
			return ReturnStatus::eError;

		m_data = iData;

		if (ReturnStatus::eError == RefineString(m_data))
			return ReturnStatus::eError;
				
		unsigned int length = m_data.length();
		if (m_data[0] != DPJsonSymbols::s_openCurlyBraces || m_data[length - 1] != DPJsonSymbols::s_closedCurlyBraces)
			return ReturnStatus::eError;
		
		ReturnStatus rs = Parse_Internal();
		
		return ReturnStatus::eOK;
	}

	ReturnStatus DPJsonSimpleStrategy::Parse_Internal() 
	{
		ReturnStatus rs = ReturnStatus::eOK;
		unsigned int length = m_data.length();
		char topStackChar;
		char atomicChar;
		DPJsonNode* prevNodeForObject = NULL;

		while (m_streamPointer != length)
		{
			atomicChar = m_data[m_streamPointer];

			switch (atomicChar)
			{
			case DPJsonSymbols::s_openCurlyBraces: 
			{
				m_symbolStack.push(DPJsonSymbols::s_openCurlyBraces);
				DPJsonNode* node = m_root->CreateNode(DPJsonNode::NodeType::eObjectValue);
				ReturnStatus rs = m_root->AddChildNode(node, m_currentParentNode);
				if (rs != ReturnStatus::eOK)
				{
					if (node)
						delete node;
					node = NULL;
				}
				prevNodeForObject = m_currentParentNode;
				m_currentParentNode = node;
				m_streamPointer++;
				break;
			}
			
			case DPJsonSymbols::s_closedCurlyBraces: 
			{
				topStackChar = m_symbolStack.top();
				if (topStackChar != DPJsonSymbols::s_openCurlyBraces)
				{
					Destroy();
					return ReturnStatus::eError;
				}
				m_symbolStack.pop();
				m_currentParentNode = prevNodeForObject;
				m_streamPointer++;
				return ReturnStatus::eOK;
			}
			
			case DPJsonSymbols::s_doubleQuotes:
			{
				std::string key = "";
				const char* skipData = " \t\n\r\f\v";
				S_JsonDataValSet dataSet;
				JsonValueType valType;

				DPJsonNode* prevParentNode = m_currentParentNode;

				//Parse key
				rs = ParseKey(key);
				if (ReturnStatus::eError == rs)
				{
					Destroy();
					return rs;
				}

				//Skip colon and spcaes and first double quote
				m_streamPointer = m_data.find(DPJsonSymbols::s_colan, m_streamPointer);
				m_streamPointer++;
				m_streamPointer = m_data.find_first_not_of(skipData, m_streamPointer);
	
				//Parse value
				valType = RecognizeValue(m_streamPointer, dataSet);
				if (DPJsonStrategy::JsonValueType::eInvalidValue == valType)
				{
					Destroy();
					return ReturnStatus::eError;
				}
				
				rs = ParseValue(valType, dataSet);
				if (ReturnStatus::eError == rs)
					return rs;
				
				m_currentParentNode = prevParentNode;

				break;
			}

			default:
				m_streamPointer++;
				break;
			}
		}

		return rs;
	}

	ReturnStatus DPJsonSimpleStrategy::ParseKey(std::string& oKey) 
	{
		int first = m_data.find(DPJsonSymbols::s_doubleQuotes, m_streamPointer);
		if (ReturnStatus::eError == CheckStreamPointer(first))
			return ReturnStatus::eError;

		int second = m_data.find(DPJsonSymbols::s_doubleQuotes, first+1);
		if (ReturnStatus::eError == CheckStreamPointer(first))
			return ReturnStatus::eError;
		
		oKey = m_data.substr(first + 1, second - (first + 1));
		if ("" == oKey)
			return ReturnStatus::eError;

		m_streamPointer = second + 1;

		DPJsonNode* node = m_root->CreateNode(DPJsonNode::NodeType::eKeyValue, oKey);
		ReturnStatus rs = m_root->AddChildNode(node, m_currentParentNode);
		if (rs != ReturnStatus::eOK)
		{
			if (node)
				delete node;
			node = NULL;
		}
		m_currentParentNode = node;

		return rs;
	}

	DPJsonStrategy::JsonValueType DPJsonSimpleStrategy::RecognizeValue(const int iStreamePointer, S_JsonDataValSet& oValue)
	{
		if (DPJsonSymbols::s_openCurlyBraces == m_data[iStreamePointer])
			return DPJsonStrategy::JsonValueType::eObject;

		else if (DPJsonSymbols::s_openSquareBraces == m_data[iStreamePointer])
			return DPJsonStrategy::JsonValueType::eArray;

		//String recognaisation
		if (DPJsonSymbols::s_doubleQuotes == m_data[m_streamPointer]) 
		{
			m_streamPointer++;
			int nextQuotePos = m_data.find(DPJsonSymbols::s_doubleQuotes, m_streamPointer);
			if (std::string::npos == nextQuotePos)
				return DPJsonStrategy::JsonValueType::eInvalidValue;

			std::string val = m_data.substr(m_streamPointer, nextQuotePos - m_streamPointer);
			m_streamPointer += val.length();
			m_streamPointer++;
			oValue.m_stringData = val;
			return DPJsonStrategy::JsonValueType::eString;
		}
		
		int nextQuotePos = m_data.find(DPJsonSymbols::s_doubleQuotes, m_streamPointer);
		if (std::string::npos == nextQuotePos)
			nextQuotePos = INT_MAX;

		int nextCommaPos = m_data.find(DPJsonSymbols::s_comma, m_streamPointer);
		if (std::string::npos == nextCommaPos)
			nextCommaPos = INT_MAX;

		int nextClosedCurlyBraces = m_data.find(DPJsonSymbols::s_closedCurlyBraces, m_streamPointer);
		if (std::string::npos == nextClosedCurlyBraces)
			nextClosedCurlyBraces = INT_MAX;

		int nextClosedsquareBraces = m_data.find(DPJsonSymbols::s_closedSquareBraces, m_streamPointer);
		if (std::string::npos == nextClosedsquareBraces)
			nextClosedsquareBraces = INT_MAX;

		int valEnd = std::string::npos;

		int val1 = (nextQuotePos < nextCommaPos) ? nextQuotePos : nextCommaPos;
		int val2 = (nextClosedCurlyBraces < nextClosedsquareBraces) ? nextClosedCurlyBraces : nextClosedsquareBraces;

		valEnd = (val1 < val2) ? val1 : val2;

		if (std::string::npos == valEnd || INT_MAX==valEnd)
		{
			oValue.m_primittiveData.m_invalidFlag = true;
			return DPJsonStrategy::JsonValueType::eInvalidValue;
		}
		
		std::string val = m_data.substr(m_streamPointer, valEnd - m_streamPointer);
		RefineString(val);
		m_streamPointer += val.length();

		if (IsPrimittiveValueValid<int>(val))
		{
			oValue.m_primittiveData.m_intData = GetPrimittiveData<int>(val);
			return DPJsonStrategy::JsonValueType::eInteger;
		}

		else if (IsPrimittiveValueValid<double>(val)) 
		{
			oValue.m_primittiveData.m_doubleData = GetPrimittiveData<double>(val);
			return DPJsonStrategy::JsonValueType::eDouble;
		}
		
		else if (DPJsonSymbols::s_true == val) 
		{
			oValue.m_primittiveData.m_boolData = true;
			return DPJsonStrategy::JsonValueType::eTrue;
		}
		
		else if (DPJsonSymbols::s_false == val) 
		{
			oValue.m_primittiveData.m_boolData = false;
			return DPJsonStrategy::JsonValueType::eFalse;
		}
		
		else if (DPJsonSymbols::s_null == val)
			return DPJsonStrategy::JsonValueType::eNull;

		else 
		{
			oValue.m_stringData = val;
			return DPJsonStrategy::JsonValueType::eString;
		}
	}

	ReturnStatus DPJsonSimpleStrategy::ParseValue(const JsonValueType iValueType, const S_JsonDataValSet iValue)
	{
		ReturnStatus rs = ReturnStatus::eOK;
		DPJsonNodeData data;
		DPJsonNode* node = NULL;

		switch(iValueType)
		{
		case DPJsonStrategy::JsonValueType::eString:
			data.SetString(iValue.m_stringData);
			break;

		case DPJsonStrategy::JsonValueType::eInteger:
			data.SetInteger(iValue.m_primittiveData.m_intData);
			break;

		case DPJsonStrategy::JsonValueType::eDouble:
			data.SetDouble(iValue.m_primittiveData.m_doubleData);
			break;

		case DPJsonStrategy::JsonValueType::eTrue:
		case DPJsonStrategy::JsonValueType::eFalse:
			data.SetBool(iValue.m_primittiveData.m_boolData);
			break;
			
		case DPJsonStrategy::JsonValueType::eNull:
			data.SetNull();
			break;

		case DPJsonStrategy::JsonValueType::eArray: 
		{
			unsigned int length = m_data.length();
			m_currentParentNode->ModifyNodeType(DPJsonNode::NodeType::eArrayValue);
			if (DPJsonSymbols::s_openSquareBraces == m_data[m_streamPointer])
			{
				m_symbolStack.push(DPJsonSymbols::s_openSquareBraces);
				m_streamPointer++;
			}
			else 
			{
				Destroy();
				return ReturnStatus::eError;
			}

			S_JsonDataValSet dataSet;
			JsonValueType valType;
			const char* skipData = " \t\n\r\f\v";
			while (DPJsonSymbols::s_closedSquareBraces != m_data[m_streamPointer] && m_streamPointer != length)
			{
				m_streamPointer = m_data.find_first_not_of(skipData, m_streamPointer);
				valType = RecognizeValue(m_streamPointer, dataSet);
				rs = ParseValue(valType, dataSet);
				if (ReturnStatus::eError == rs)
					return ReturnStatus::eError;

				//Reach to next value to recognize
				int nextClosedSquareBraces = m_data.find(DPJsonSymbols::s_closedSquareBraces, m_streamPointer);
				if (std::string::npos == nextClosedSquareBraces) 
				{
					Destroy();
					return ReturnStatus::eError;
				}
				int nextData = m_data.find(DPJsonSymbols::s_comma, m_streamPointer);
				if (std::string::npos == nextData) 
				{
					nextData = INT_MAX;
				}
				if (nextData < nextClosedSquareBraces) 
				{
					m_streamPointer = nextData;
					m_streamPointer++;
					m_streamPointer = m_data.find_first_not_of(skipData, m_streamPointer);
					if (DPJsonSymbols::s_closedSquareBraces == m_data[m_streamPointer]) 
					{
						Destroy();
						return ReturnStatus::eError;
					}	
				}
				else 
				{
					m_streamPointer = nextClosedSquareBraces;
				}
				
			}

			char topStackChar = m_symbolStack.top();
			if (topStackChar != DPJsonSymbols::s_openSquareBraces)
			{
				Destroy();
				return ReturnStatus::eError;
			}
			m_symbolStack.pop();
			m_streamPointer++;
			
			return ReturnStatus::eOK;
		}
		
		case DPJsonStrategy::JsonValueType::eObject:
			rs = Parse_Internal();
			return rs;
		}

		if (!m_currentParentNode && DPJsonNode::NodeType::eKeyValue == m_currentParentNode->GetNodeType())
			return rs;

		node = m_root->CreateNode(DPJsonNode::NodeType::eLeafValue, data);
		rs = m_root->AddChildNode(node, m_currentParentNode);
		if (rs != ReturnStatus::eOK)
		{
			if (node)
				delete node;
			node = NULL;

			Destroy();
			return ReturnStatus::eError;
		}

		return rs;
	}

	ReturnStatus DPJsonSimpleStrategy::RefineString(std::string& ioString) const
	{
		if ("" == ioString)
			return ReturnStatus::eError;

		//Remove leading and trailing spaces.
		const char* trimData = " \t\n\r\f\v";
		ioString.erase(0, ioString.find_first_not_of(trimData));
		ioString.erase(ioString.find_last_not_of(trimData) + 1);

		return ReturnStatus::eOK;
	}

	ReturnStatus DPJsonSimpleStrategy::CheckStreamPointer(int iStreamPointer)
	{
		if (std::string::npos == iStreamPointer)
		{
			Destroy();

			LOG_TO_FILE(JSON file is not valid);
			return ReturnStatus::eError;
		}

		return ReturnStatus::eOK;
	}

	void DPJsonSimpleStrategy::Destroy()
	{
		if (m_root)
			m_root->DestroyTree();
		
		m_currentParentNode = NULL;
	}

	DPJsonTree* DPJsonSimpleStrategy::GetDomTree() const 
	{
		return m_root;
	}
}
