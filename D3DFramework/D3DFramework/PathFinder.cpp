#include "stdafx.h"
#include "PathFinder.h"

using namespace PKH;

//=======================================================
//	PathFinderNode
//=======================================================

PKH::PathFinder::Node::Node()
{
}

PKH::PathFinder::Node::Node(PathFinder::Node* parent, Vector3 position)
{
	this->parent = parent;
	this->position = position;
}

PKH::PathFinder::Node::~Node()
{
}


int PKH::PathFinder::Node::G()
{
	if (parent == nullptr) return 0;
	
	Vector3 dir = parent->position - position;
	float len = dir.Length();
	len = len + parent->G();
	return (int)len;
}

int PKH::PathFinder::Node::H(Node* dest)
{
	Vector3 dir = dest->position - position;
	float len = dir.Length();

	return (int)len;
}

int PKH::PathFinder::Node::F(Node* dest)
{
	return G() + H(dest);
}

//=======================================================
//	PathFinder
//=======================================================

PKH::PathFinder::PathFinder()
{
}

PKH::PathFinder::~PathFinder()
{
	for (auto node : nodes)
	{
		delete node.second;
	}

	nodes.clear();
}

PKH::PathFinder::Node* PKH::PathFinder::FindStartNode(const Vector3& startPos)
{
	// nodes �߿��� startPos�� ���� ����� �������� ��� ã��

	PathFinder::Node* result = nullptr;

	for (auto node : nodes)
	{
		if (node.second->position == startPos)
		{
			// �������� ������ �׳� ����
			return node.second;
		}

		// ���� ����� �������� ��� ã��
		if (result == nullptr)
		{
			//�񱳸� ���ؼ� �ϴ� �ϳ� ����
			result = node.second;
			continue;
		}

		float oldLen = Vector3(startPos - result->position).Length();
		float curLen = Vector3(startPos - node.second->position).Length();
		if (curLen < oldLen)
		{
			result = node.second;
		}
	}
	return result;
}

PKH::PathFinder::Node* PKH::PathFinder::FindDestinationNode(const Vector3& destPos)
{
	// nodes �߿��� startPos�� ���� ����� �������� ��� ã��

	PathFinder::Node* result = nullptr;

	for (auto node : nodes)
	{
		if (node.second->position == destPos)
		{
			// �������� ������ �׳� ����
			return node.second;
		}

		// ���� ����� �������� ��� ã��
		if (result == nullptr)
		{
			//�񱳸� ���ؼ� �ϴ� �ϳ� ����
			result = node.second;
			continue;
		}

		float oldLen = Vector3(destPos - result->position).Length();
		float curLen = Vector3(destPos - node.second->position).Length();
		if (curLen < oldLen)
		{
			result = node.second;
		}
	}
	return result;
}

void PKH::PathFinder::SetDestination(PKH::PathFinder::Node* dest)
{
	// nodes�� ��� ����� ������ ����
	for (auto node : nodes)
	{
		node.second->destination = dest;
	}
}

void PKH::PathFinder::AddNode(DWORD index, PKH::PathFinder::Node* pNode)
{
	nodes[index] = pNode;
}

void PKH::PathFinder::LinkNode(DWORD srcIndex, DWORD destIndex)
{
	nodes[srcIndex]->adjacency.push_back(nodes[destIndex]);
}

PathFinderType PKH::PathFinder::GetType()
{
	return type;
}

PKH::PathFinder::Node* PKH::PathFinder::GetResultNode()
{
	return resultNode;
}
