#include "DPJsonNodeData.h"

namespace DevilsParser
{
	DPJsonNodeData::DPJsonNodeData() :
		m_null(NULL)
	{
		m_primitiveData.m_doubleData = 0.0;
		m_stringData = "";
		SetAllDataFlag(false);
	}

	DPJsonNodeData::DPJsonNodeData(const int iData) :
		m_null(NULL)
	{
		m_stringData = "";
		SetInteger(iData);
	}

	DPJsonNodeData::DPJsonNodeData(const double iData) :
		m_null(NULL)
	{
		m_stringData = "";
		SetDouble(iData);
	}

	DPJsonNodeData::DPJsonNodeData(const char iData) :
		m_null(NULL)
	{
		m_stringData = "";
		SetCharacter(iData);
	}

	DPJsonNodeData::DPJsonNodeData(const bool iData) :
		m_null(NULL)
	{
		m_stringData = "";
		SetBool(iData);
	}

	DPJsonNodeData::DPJsonNodeData(const std::string iData) :
		m_null(NULL)
	{
		SetString(iData);
	}
	
	DPJsonNodeData::~DPJsonNodeData()
	{
	}

	void DPJsonNodeData::SetInteger(const int iData)
	{
		m_primitiveData.m_intData = iData;
		SetAllDataFlag(false);
		m_intDataType = true;
	}

	bool DPJsonNodeData::GetInteger(int& oData) const
	{
		if (!m_intDataType)
			return false;

		oData = m_primitiveData.m_intData;
		return true;
	}

	void DPJsonNodeData::SetDouble(const double iData)
	{
		m_primitiveData.m_doubleData = iData;
		SetAllDataFlag(false);
		m_dobleDataType = true;
	}

	bool DPJsonNodeData::GetDouble(double& oData) const
	{
		if (!m_dobleDataType)
			return false;

		oData = m_primitiveData.m_doubleData;
		return true;
	}

	void DPJsonNodeData::SetCharacter(const char iData)
	{
		m_primitiveData.m_charData = iData;
		SetAllDataFlag(false);
		m_charDataType = true;
	}

	bool DPJsonNodeData::GetCharacter(char& oData) const
	{
		if (!m_charDataType)
			return false;

		oData = m_primitiveData.m_charData;
		return true;
	}

	void DPJsonNodeData::SetBool(const bool iData) 
	{
		m_primitiveData.m_boolData = iData;
		SetAllDataFlag(false);
		m_boolDataType = true;
	}

	bool DPJsonNodeData::GetBool(bool& oData) const 
	{
		if (!m_boolDataType)
			return false;

		oData = m_primitiveData.m_boolData;
		return true;
	}

	void DPJsonNodeData::SetString(const std::string iData)
	{
		m_stringData = iData;
		SetAllDataFlag(false);
		m_stringDataType = true;
	}

	bool DPJsonNodeData::GetString(std::string& oData) const
	{
		if (!m_stringDataType)
			return false;

		oData = m_stringData;
		return true;
	}

	void DPJsonNodeData::SetNull() 
	{
		SetAllDataFlag(false);
		m_nullDataType = true;
	}
	
	bool DPJsonNodeData::GetNull(void* oData) const
	{
		if (!m_nullDataType)
			return false;

		oData = NULL;
		return true;
	}

	DPJsonNodeData::NodeDataType DPJsonNodeData::IsNodeType() const
	{
		if (m_intDataType)
			return NodeDataType::eInt;

		else if (m_dobleDataType)
			return NodeDataType::eDouble;

		else if (m_charDataType)
			return NodeDataType::eChar;

		else if (m_boolDataType)
			return NodeDataType::eBoolean;

		else if (m_stringDataType)
			return NodeDataType::eString;

		else if (m_nullDataType)
			return NodeDataType::eNull;

		else
			return NodeDataType::eNone;
	}

	DPJsonNodeData& DPJsonNodeData::operator=(const DPJsonNodeData& iData)
	{
		if (this == &iData)
			return *this;
		
		NodeDataType nodeType = iData.IsNodeType();

		switch (nodeType) 
		{
		case NodeDataType::eInt:
			SetInteger(iData.m_primitiveData.m_intData);
			break;

		case NodeDataType::eDouble:
			SetDouble(iData.m_primitiveData.m_doubleData);
			break;

		case NodeDataType::eChar:
			SetCharacter(iData.m_primitiveData.m_charData);
			break;

		case NodeDataType::eBoolean:
			SetBool(iData.m_primitiveData.m_boolData);
			break;

		case NodeDataType::eString:
			SetString(iData.m_stringData);
			break;

		case NodeDataType::eNull:
			SetNull();
			break;
		}

		return *this;
	}

	void DPJsonNodeData::SetAllDataFlag(bool iFlag)
	{
		m_intDataType = iFlag;
		m_dobleDataType = iFlag;
		m_charDataType = iFlag;
		m_boolDataType = iFlag;
		m_stringDataType = iFlag;
		m_nullDataType = iFlag;
	}
}
