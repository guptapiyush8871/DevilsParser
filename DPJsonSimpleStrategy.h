#ifndef DPJsonSimpleStrategy_h
#define DPJsonSimpleStrategy_h

#include "DPJsonStrategy.h"
#include "DPBase.h"
#include "DPJsonTree.h"
#include <string>
#include <stack>

namespace DevilsParser
{
	class DPJsonSimpleStrategy : public DPJsonStrategy
	{
	public:
		DPJsonSimpleStrategy();
		virtual ~DPJsonSimpleStrategy();

		virtual ReturnStatus Parse(std::string iData);
		virtual DPJsonTree* GetDomTree() const;
		
	private:
		struct S_JsonDataValSet
		{
			union PrimittiveData
			{
				int m_intData;
				double m_doubleData;
				bool m_boolData;
				bool m_invalidFlag;
			};

			std::string m_stringData;
			PrimittiveData m_primittiveData;
		};

		ReturnStatus Parse_Internal();

		/*
		* ParseKey will always make a node of eKeyValue type because at the creation time, key node is not aware of its corresponding value.
		* So for eArrayValue, node type should be modified to eArrayValue.
		*/
		ReturnStatus ParseKey(std::string& oKey);
		
		JsonValueType RecognizeValue(const int iStreamePointer, S_JsonDataValSet& oValue);
		ReturnStatus ParseValue(const JsonValueType iValueType, const S_JsonDataValSet iValue);
		ReturnStatus RefineString(std::string& ioString) const;
		ReturnStatus CheckStreamPointer(int iStreamPointer);
		void Destroy();
		
		std::string m_data;
		DPJsonTree* m_root;
		DPJsonNode* m_currentParentNode;
		int m_streamPointer;
		std::stack<char> m_symbolStack;
	};
}

#endif
