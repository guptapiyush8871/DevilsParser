#include "DPJsonNode.h"

namespace DevilsParser
{
	DPJsonNode::DPJsonNode(NodeType iNodeType)
	{
		m_nodeType = iNodeType;
		m_numChild = 0;
		m_nextSibling = NULL;
		m_isInTree = false;
	}

	DPJsonNode::DPJsonNode(NodeType iNodeType, DPJsonNodeData iData)
	{
		m_nodeType = iNodeType;
		m_data = iData;
		m_numChild = 0;
		m_nextSibling = NULL;
		m_isInTree = false;
	}

	DPJsonNode::~DPJsonNode()
	{
		m_nextSibling = NULL;

		RemoveAllChildrenBinding();
	}

	void DPJsonNode::ModifyNodeType(const NodeType iNodeType)
	{
		m_nodeType = iNodeType;
	}

	DPJsonNode::NodeType DPJsonNode::GetNodeType() const
	{
		return m_nodeType;
	}

	DPJsonNodeData::NodeDataType DPJsonNode::GetNodeDataType() const
	{
		return m_data.IsNodeType();
	}

	void DPJsonNode::SetData(const int iData)
	{
		m_data.SetInteger(iData);
	}

	bool DPJsonNode::GetData(int& oData) const
	{
		bool flag = m_data.GetInteger(oData);
		return flag;
	}

	void DPJsonNode::SetData(const double iData)
	{
		m_data.SetDouble(iData);
	}

	bool DPJsonNode::GetData(double& oData) const
	{
		bool flag = m_data.GetDouble(oData);
		return flag;
	}

	void DPJsonNode::SetData(const char iData)
	{
		m_data.SetCharacter(iData);
	}

	bool DPJsonNode::GetData(char& oData) const
	{
		bool flag = m_data.GetCharacter(oData);
		return flag;
	}

	void DPJsonNode::SetData(const std::string iData)
	{
		m_data.SetString(iData);
	}

	bool DPJsonNode::GetData(std::string& oData) const
	{
		bool flag = m_data.GetString(oData);
		return flag;
	}

	DPJsonNodeData DPJsonNode::GetData() const 
	{
		return m_data;
	}

	int DPJsonNode::GetNumChild() const
	{
		return m_numChild;
	}

	ReturnStatus DPJsonNode::SetNextSibling(DPJsonNode* iSibling)
	{
		if (!iSibling)
			return ReturnStatus::eError;

		m_nextSibling = iSibling;

		return ReturnStatus::eOK;
	}

	DPJsonNode* DPJsonNode::GetNextSibling() const
	{
		return m_nextSibling;
	}

	void DPJsonNode::RemoveNextSiblingBinding()
	{
		m_nextSibling = NULL;
	}

	ReturnStatus DPJsonNode::AddChild(DPJsonNode* iChild)
	{
		if (!iChild)
			return ReturnStatus::eError;

		m_child.push_back(iChild);
		m_numChild++;

		return ReturnStatus::eOK;
	}

	DPJsonNode* DPJsonNode::GetFirstChild() const
	{
		if (!m_child.empty())
			return m_child.front();

		return NULL;
	}

	DPJsonNode* DPJsonNode::GetLastChild() const
	{
		if (!m_child.empty())
			return m_child.back();

		return NULL;
	}

	DPJsonNode* DPJsonNode::GetChild(const int iIndex) const 
	{
		if (iIndex >= m_numChild)
			return NULL;

		return m_child[iIndex];
	}

	std::vector<DPJsonNode*> DPJsonNode::GetAllChildren() const
	{
		return m_child;
	}

	void DPJsonNode::RemoveChildBinding(DPJsonNode* iChild)
	{
		for (unsigned int i = 0; i < m_child.size(); i++)
		{
			if (m_child[i] == iChild)
			{
				m_child.erase(m_child.begin() + i);
				m_numChild--;
			}
		}
	}

	void DPJsonNode::RemoveAllChildrenBinding()
	{
		for (unsigned int i = 0; i < m_child.size(); i++)
		{
			m_child[i] = NULL;
		}

		m_child.clear();
		m_numChild = 0;
	}

	void DPJsonNode::AttachWithTree()
	{
		m_isInTree = true;
	}

	void DPJsonNode::DetachWithTree()
	{
		m_isInTree = false;
	}

	bool DPJsonNode::IsInTree() const
	{
		return m_isInTree;
	}

	bool DPJsonNode::IsRootNode() const 
	{
		return (m_nodeType == NodeType::eRootNode) ? true : false;
	}
}
