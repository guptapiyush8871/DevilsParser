#include "DPJsonTree.h"

namespace DevilsParser
{
	DPJsonTree::DPJsonTree()
	{
		DPJsonNodeData nodeData;
		nodeData.SetString("ROOT");

		m_root = CreateNode(DPJsonNode::NodeType::eRootNode, nodeData);
		m_root->AttachWithTree();
		m_numNodes = 0;
	}

	DPJsonTree::~DPJsonTree()
	{
		if(m_numNodes)
			DestroyTree();
	}

	DPJsonNode* DPJsonTree::CreateNode(DPJsonNode::NodeType iNodeType)
	{
		DPJsonNode* node = new DPJsonNode(iNodeType);
		return node;
	}

	DPJsonNode* DPJsonTree::CreateNode(DPJsonNode::NodeType iNodeType, DPJsonNodeData iNodeData)
	{
		DPJsonNode* node = new DPJsonNode(iNodeType, iNodeData);
		return node;
	}

	ReturnStatus DPJsonTree::AddChildNode(DPJsonNode* iNode, DPJsonNode* iParent)
	{
		if (!iNode || !iParent || !IsNodeExists(iParent))
			return ReturnStatus::eError;

		DPJsonNode* lastNode = this->GetLastChild(iParent);
		if (lastNode)
			lastNode->SetNextSibling(iNode);

		iParent->AddChild(iNode);
		iNode->AttachWithTree();

		m_numNodes++;

		return ReturnStatus::eOK;
	}

	ReturnStatus DPJsonTree::AddChildNode(DPJsonNode::NodeType iNodeType, DPJsonNodeData iNodeData, DPJsonNode* iParent)
	{
		if (!iParent)
			return ReturnStatus::eError;

		DPJsonNode* node = CreateNode(iNodeType, iNodeData);
		ReturnStatus rs = AddChildNode(node, iParent);
		if (ReturnStatus::eError == rs)
		{
			delete node;
			node = NULL;
		}

		return rs;
	}

	ReturnStatus DPJsonTree::AddChildren(std::vector<DPJsonNode*> iNode, std::vector<DPJsonNode*> iParent)
	{
		if (iNode.size() != iParent.size())
			return ReturnStatus::eError;

		int size = iNode.size();

		for (int i = 0; i < size; i++)
		{
			if (!iNode[i] || !iParent[i] || !IsNodeExists(iParent[i]))
				return ReturnStatus::eError;
		}

		for (int i = 0; i < size; i++)
		{
			AddChildNode(iNode[i], iParent[i]);
		}

		return ReturnStatus::eOK;
	}

	ReturnStatus DPJsonTree::AddChildren(std::vector<DPJsonNode*> iNode, DPJsonNode* iParent)
	{
		if (!iParent || !IsNodeExists(iParent))
			return ReturnStatus::eError;

		int size = iNode.size();
		for (int i = 0; i < size; i++)
		{
			if (!iNode[i])
				return ReturnStatus::eError;
		}

		for (int i = 0; i < size; i++)
		{
			AddChildNode(iNode[i], iParent);
		}

		return ReturnStatus::eOK;
	}

	DPJsonNode* DPJsonTree::GetRootNode() const 
	{
		return m_root;
	}

	DPJsonNode* DPJsonTree::GetNextSibling(const DPJsonNode* iNode) const
	{
		if (!iNode || !IsNodeExists(iNode))
			return NULL;

		return iNode->GetNextSibling();
	}

	DPJsonNode* DPJsonTree::GetFirstChild(const DPJsonNode* iParent) const
	{
		if (!iParent || !IsNodeExists(iParent))
			return NULL;

		return iParent->GetFirstChild();
	}

	DPJsonNode* DPJsonTree::GetLastChild(const DPJsonNode* iParent) const
	{
		if (!iParent || !IsNodeExists(iParent))
			return NULL;

		return iParent->GetLastChild();
	}

	DPJsonNode* DPJsonTree::GetParent(const DPJsonNode* iChild) const
	{
		if (!iChild || !IsNodeExists(iChild))
			return NULL;

		DPJsonNode* parentNode = GetParent_Internal(m_root, iChild);

		return parentNode;
	}

	DPJsonNode* DPJsonTree::GetParent_Internal(DPJsonNode* iRoot, const DPJsonNode* iChild) const
	{
		if (!iRoot || !iChild)
			return NULL;

		if (iRoot == iChild)
			return iRoot;

		DPJsonNode* localNode = NULL;
		static bool parentFindFlag = false;

		for (int i = 0; i < iRoot->GetNumChild(); i++)
		{
			localNode = GetParent_Internal(iRoot->GetChild(i), iChild);

			if (localNode && !parentFindFlag)
			{
				localNode = iRoot;
			}

			if (localNode)
				return localNode;
		}

		return localNode;
	}

	int DPJsonTree::GetTotalNodes() const
	{
		return m_numNodes;
	}

	int DPJsonTree::GetTotalNodesInSubTree(DPJsonNode* iNode) const
	{
		if (!iNode || !IsNodeExists(iNode))
			return -1;

		if (DPJsonNode::NodeType::eLeafValue == iNode->GetNodeType())
			return 0;

		int numNodes = GetTotalNodesInSubTree_Internal(iNode);

		return numNodes;
	}

	int DPJsonTree::GetTotalNodesInSubTree_Internal(DPJsonNode* iNode) const 
	{
		if (!iNode)
			return 0;

		int totalNodes = iNode->GetNumChild();

		for (int i = 0; i < iNode->GetNumChild(); i++) 
		{
			totalNodes = totalNodes + GetTotalNodesInSubTree_Internal(iNode);
		}

		return totalNodes;
	}

	ReturnStatus DPJsonTree::RemoveNode(DPJsonNode* iNode)
	{
		if (!iNode || !IsNodeExists(iNode))
			return ReturnStatus::eError;

		DPJsonNode* parent = GetParent(iNode);
		DestroySubTree(iNode);
		parent->RemoveChildBinding(iNode);
		delete iNode;
		iNode = NULL;
		m_numNodes--;

		return ReturnStatus::eOK;
	}

	void DPJsonTree::DestroySubTree(DPJsonNode* iNode)
	{
		if (!iNode || iNode->GetNodeType() == DPJsonNode::NodeType::eLeafValue)
			return;

		DestroySubTree_Internal(iNode);
	}
	
	void DPJsonTree::DestroySubTree_Internal(DPJsonNode* iNode) 
	{
		if (!iNode)
			return;
				
		while(iNode->GetNumChild()) 
		{
			DPJsonNode* child = iNode->GetFirstChild();
			DestroySubTree_Internal(child);
			iNode->RemoveChildBinding(child);
			child = NULL;
		}

		if (0 == iNode->GetNumChild())
		{
			m_numNodes--;
			delete iNode;
			return;
		}
	}

	void DPJsonTree::DestroyTree()
	{
		DestroySubTree(m_root);
		m_numNodes = 0;
	}

	bool DPJsonTree::IsEmpty() const
	{
		return (m_numNodes == 0) ? true : false;
	}

	bool DPJsonTree::IsNodeExists(const DPJsonNode* iNode) const
	{
		if (!iNode || (!iNode->IsRootNode() && IsEmpty()))
			return false;

		return iNode->IsInTree();
	}
}
