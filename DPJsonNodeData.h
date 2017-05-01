#ifndef DPNodeData_h
#define DPNodeData_h

#include <string>

namespace DevilsParser
{
	class DPJsonNodeData
	{
	public:
		struct S_NodeDataType
		{
			enum ENodeDataType
			{
				eNone = 0,
				eInt,
				eDouble,
				eChar,
				eBoolean,
				eString,
				eNull,
				eEnd
			};
		};
		typedef S_NodeDataType::ENodeDataType NodeDataType;

		union PrimitiveData
		{
			int m_intData;
			double m_doubleData;
			char m_charData;
			bool m_boolData;
		};

		DPJsonNodeData();
		DPJsonNodeData(const int iData);
		DPJsonNodeData(const double iData);
		DPJsonNodeData(const char iData);
		DPJsonNodeData(const bool iData);
		DPJsonNodeData(const std::string iData);
		virtual ~DPJsonNodeData();

		void SetInteger(const int iData);
		bool GetInteger(int& oData) const;

		void SetDouble(const double iData);
		bool GetDouble(double& oData) const;

		void SetCharacter(const char iData);
		bool GetCharacter(char& oData) const;

		void SetBool(const bool iData);
		bool GetBool(bool& oData) const;

		void SetString(const std::string iData);
		bool GetString(std::string& oData) const;

		void SetNull();
		bool GetNull(void* oData) const;

		NodeDataType IsNodeType() const;

		DPJsonNodeData& operator=(const DPJsonNodeData& iData);

	private:
		void SetAllDataFlag(bool iFlag);

		PrimitiveData m_primitiveData;
		std::string m_stringData;
		const void* const m_null;
		short unsigned int m_intDataType : 1;
		short unsigned int m_dobleDataType : 1;
		short unsigned int m_charDataType : 1;
		short unsigned int m_boolDataType : 1;
		short unsigned int m_stringDataType : 1;
		short unsigned int m_nullDataType : 1;
	};
}

#endif
