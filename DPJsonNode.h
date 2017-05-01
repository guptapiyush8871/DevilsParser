#ifndef DPJsonNode_h
#define DPJsonNode_h

#include "DPBase.h"
#include "DPJsonNodeData.h"
#include <vector>

namespace DevilsParser
{
	class DPJsonNode
	{
	public:
		struct S_NodeType
		{
			enum ENodeType
			{
				eRootNode = 0,
				eKeyValue,
				eArrayValue,
				eObjectValue,
				eLeafValue
			};
		};
		typedef S_NodeType::ENodeType NodeType;

		DPJsonNode(NodeType iNodeType);
		DPJsonNode(NodeType iNodeType, DPJsonNodeData iData);
		
		virtual ~DPJsonNode();

		void ModifyNodeType(const NodeType iNodeType);
		NodeType GetNodeType() const;
		DPJsonNodeData::NodeDataType GetNodeDataType() const;

		void SetData(const int iData);
		bool GetData(int& oData) const;

		void SetData(const double iData);
		bool GetData(double& oData) const;

		void SetData(const char iData);
		bool GetData(char& oData) const;

		void SetData(const std::string iData);
		bool GetData(std::string& oData) const;

		DPJsonNodeData GetData() const;

		int GetNumChild() const;

		ReturnStatus SetNextSibling(DPJsonNode* iSibling);
		DPJsonNode* GetNextSibling() const;
		void RemoveNextSiblingBinding();

		ReturnStatus AddChild(DPJsonNode* iChild);
		DPJsonNode* GetFirstChild() const;
		DPJsonNode* GetLastChild() const;
		DPJsonNode* GetChild(const int iIndex) const;
		std::vector<DPJsonNode*> GetAllChildren() const;
		void RemoveChildBinding(DPJsonNode* iChild);
		void RemoveAllChildrenBinding();

		void AttachWithTree();
		void DetachWithTree();
		bool IsInTree() const;
		bool IsRootNode() const;

	private:
		NodeType m_nodeType;
		DPJsonNodeData m_data;
		int m_numChild;
		DPJsonNode* m_nextSibling;
		std::vector<DPJsonNode*> m_child;
		bool m_isInTree;
	};
}
#endif
