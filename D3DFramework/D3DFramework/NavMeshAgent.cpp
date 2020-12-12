#include "stdafx.h"
#include "NavMeshAgent.h"
#include "NavMeshMap.h"

PKH::NavMeshAgent::NavMeshAgent(GameObject* owner)
    :IComponent(owner)
{
    navMeshMap = (NavMeshMap*)ObjectManager::GetInstance()->FindObject<NavMeshMap>();
}

PKH::NavMeshAgent::NavMeshAgent(const NavMeshAgent& rhs)
    :IComponent(rhs)
{
}

PKH::NavMeshAgent::~NavMeshAgent()
{
}

void PKH::NavMeshAgent::Update()
{
    if (isMoving)
    {
        if (isDestination) return;

        Vector3 remain = nextPosition - transform->position;
        Vector3 direction = remain.Normalized();
        float remainDist = remain.Length();
        // ���� �Ÿ��� ���� �Ÿ����� �۰ų� ������(������or �������� ����)
        if (remainDist <= stoppingDistance)
        {
            path.pop();

            // ������ ��ΰ� �ִٸ�
            if (path.empty() == false)
            {
                // nextPosition ���� ����
                nextPosition = path.top();
            }
            else // ������ο� �����ߴٸ�
            {
                isDestination = true;
                return;
            }
        }
        else // ���� �Ÿ��� �ִٸ�
        {
            // �̵�
            transform->position += direction * speed * TimeManager::DeltaTime();
        }
    }
}

IComponent* PKH::NavMeshAgent::Clone()
{
    return new NavMeshAgent(*this);
}

bool PKH::NavMeshAgent::SetDestination(const Vector3& target)
{
    // ��ã�� ���� ����
    bool result = navMeshMap->Search(transform->position, target);
    if (result == true)
    {
        ResetPath();
        // ��ã�� ��� ����
        PathFinder::Node* resNode = navMeshMap->GetResultNode();
        if (resNode == nullptr) return false;

        // destination ����
        destination = resNode->position;
        

        // path ����
        PathFinder::Node* iter = resNode;
        while (iter != nullptr)
        {
            // ���������� �����������ϱ� �տ��� �־��༭ ������ ����
            path.push(iter->position);
            iter = iter->parent;
        }

        // nextPosition ����
        nextPosition = path.top();
        isDestination = false;
    }

    return result;
}

void PKH::NavMeshAgent::Stop()
{
    isMoving = false;
}

void PKH::NavMeshAgent::Resume()
{
    isMoving = true;
}

void PKH::NavMeshAgent::Move(const Vector3& offset)
{
}

void PKH::NavMeshAgent::ResetPath()
{
    while (!path.empty())
    {
        path.pop();
    }
    
}
