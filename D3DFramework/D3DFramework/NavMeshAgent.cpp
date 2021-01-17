#include "stdafx.h"
#include "NavMeshAgent.h"
#include "NavMeshMap.h"
#include "Unit.h"

PKH::NavMeshAgent::NavMeshAgent(GameObject* owner)
    :IComponent(owner)
{
    unit = dynamic_cast<EnemyUnit*>(owner);
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
            // ������ ��ΰ� �ִٸ�
            if (path.empty() == false)
            {
                // nextPosition ���� ����
                nextPosition = path.front();
                path.erase(path.begin());
            }
            else // ������ο� �����ߴٸ�
            {
                isDestination = true;
                return;
            }
        }
        else // ���� �Ÿ��� �ִٸ�
        {
            // ����
            unit->state = UnitState::RUN;
            // ȸ��
            //float angle = Vector3::AngleY(Vector3(0, 0, 1), direction);
            //gameObject->transform->eulerAngles.y = angle;
            unit->LookRotation(direction);
            // �̵�
            transform->position += direction * speed * TimeManager::DeltaTime();
        }
    }
}

IComponent* PKH::NavMeshAgent::Clone()
{
    return new NavMeshAgent(*this);
}

bool PKH::NavMeshAgent::SetDestination(const Vector3& target, bool noSearch)
{
    bool result = false;
    isDestination = false;
    ResetPath();

    if (noSearch)
    {
        destination = target;
        path.push_back(target);
        nextPosition = target;
    }
    else
    {
        // ��ã�� ���� ����
        result = navMeshMap->Search(transform->position, target);
        if (result == true)
        {
            // ��ã�� ��� ����
            list<PathFinder::Node*>* resultPath = navMeshMap->GetPath();
            auto iter = resultPath->begin();
            auto end = resultPath->end();
            for (; iter != end; ++iter)
            {
                path.push_back((*iter)->position);
            }
            // destination ����
            destination = target;
            path.push_back(target);
            // nextPosition ����
            nextPosition = path.front();
            path.erase(path.begin());
            
        }
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
    path.clear();
    
}

void PKH::NavMeshAgent::SetStoppingDistance(float _dist)
{
    stoppingDistance = _dist;
}
