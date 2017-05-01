#ifndef DPJsonTree_h
#define DPJsonTree_h

#include "DPJsonNode.h"

namespace DevilsParser 
{
	class DPJsonTree
	{
	public:
		DPJsonTree();
		virtual ~DPJsonTree();

		DPJsonNode* CreateNode(DPJsonNode::NodeType iNodeType);
		DPJsonNode* CreateNode(DPJsonNode::NodeType iNodeType, DPJsonNodeData iNodeData);

		ReturnStatus AddChildNode(DPJsonNode* iNode, DPJsonNode* iParent);
		ReturnStatus AddChildNode(DPJsonNode::NodeType iNodeType, DPJsonNodeData iNodeData, DPJsonNode* iParent);
		ReturnStatus AddChildren(std::vector<DPJsonNode*> iNode, std::vector<DPJsonNode*> iParent);
		ReturnStatus AddChildren(std::vector<DPJsonNode*> iNode, DPJsonNode* iParent);

		DPJsonNode* GetRootNode() const;
		DPJsonNode* GetNextSibling(const DPJsonNode* iNode) const;
		DPJsonNode* GetFirstChild(const DPJsonNode* iParent) const;
		DPJsonNode* GetLastChild(const DPJsonNode* iParent) const;
		DPJsonNode* GetParent(const DPJsonNode* iChild) const;
		
		int GetTotalNodes() const;
		int GetTotalNodesInSubTree(DPJsonNode* iNode) const;

		/*
		* Remove itself with all its descendentes.
		*/
		ReturnStatus RemoveNode(DPJsonNode* iNode);

		/*
		* Destroy itself with its sub tree.
		*/
		void DestroySubTree(DPJsonNode* iNode);

		void DestroyTree();

		bool IsEmpty() const;

		bool IsNodeExists(const DPJsonNode* iNode) const;

	private:
		DPJsonNode* GetParent_Internal(DPJsonNode* iRoot, const DPJsonNode* iChild) const;
		int GetTotalNodesInSubTree_Internal(DPJsonNode* iNode) const;
		void DestroySubTree_Internal(DPJsonNode* iNode);

		DPJsonNode* m_root;
		unsigned int m_numNodes;
	};
}

#endif
