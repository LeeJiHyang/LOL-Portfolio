#include "stdafx.h"
#include "Astar.h"

using namespace Engine;

Engine::Astar::Astar()
{
    type = PathFinderType::Astar;
}

Engine::Astar::~Astar()
{
}

bool Engine::Astar::Search(const Vector3& start, const Vector3& dest)
{
    path.clear();
    // ���¸���Ʈ
    priority_queue<Node*, vector<Node*>, ComparePFNode> openList;
    set<Node*> closeList;
    // ���� ��� ����
    PathFinder::Node* sNode = FindStartNode(start, 5.f);
    if (sNode == nullptr) return false;
    sNode->parent = nullptr;
    openList.push(sNode);
    
    // ������ ����
    PathFinder::Node* eNode = FindDestinationNode(dest, 5.f);
    if (eNode == nullptr) return false;
    eNode->parent = nullptr;
    // ���� ��� �ʱ�ȭ
    resultNode = eNode;

    SetDestination(eNode);

    while (!openList.empty())
    {
        // OpenList���� �ϳ� �̰� (������ F�� �۱� ������ ��ó���� ������ ��)
        PathFinder::Node* current = openList.top();
        openList.pop();

        // ���� ��尡 ���� ��尡 ������ return true;
        if (current == eNode)
        {
            // ������ ���� �߰���ο� ���� �ĺ��� �ִ��� Ȯ��
            OptimizeStartNode();
            
            SetPath();
            OptimizeDestinationNode(dest);
            Optimize();

            return true;
        }

        // �ƴϸ� CloseList�� �߰�
        closeList.insert(current);

        // ��� ���� ����� ���� ������ OpenList�� �߰�(CloseList�� �̹� �ִٸ� �߰� ����)
        for (auto adjNode : current->adjacency)
        {
            auto end = closeList.end();
            auto find = closeList.find(adjNode);
            if (find == end)
            {
                // Close List�� ������尡 ������ Open List�� �߰�
                adjNode->parent = current;
                openList.push(adjNode);
            }
            
            // ��������߿� ������尡 �ִٸ�?
        }


    }



    return false;
}
