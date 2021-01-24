#include "stdafx.h"
#include "NetPlayerController.h"
#include "Collider.h"
#include "NavMeshAgent.h"
#include "NavNode.h"
#include "Unit.h"

NetPlayerController::NetPlayerController(GameObject* owner)
    :IComponent(owner)
{
    net = Network::GetInstance();
    unit = (Unit*)owner;
    agent = (NavMeshAgent*)owner->GetComponent<NavMeshAgent>();
}

NetPlayerController::NetPlayerController(const NetPlayerController& rhs)
    :IComponent(rhs)
{
}

NetPlayerController::~NetPlayerController()
{
    net = nullptr;
    unit = nullptr;
    agent = nullptr;
}

void NetPlayerController::Update()
{
    if (gameObject == nullptr)return;


    if (InputManager::GetKeyDown('W'))
    {

    }
    if (InputManager::GetKeyDown('A'))
    {
        SetTargetMode(!targetMode);
    }
    if (InputManager::GetKeyDown('S'))
    {

    }
    if (InputManager::GetKeyDown('D'))
    {

    }

    if (InputManager::GetKeyDown('Q'))
    {

    }
    if (InputManager::GetKeyDown('E'))
    {

    }



    if (InputManager::GetMouseWheelUp())
    {
        Camera* cam = Camera::GetInstance();
        cam->Move(cam->transform->look, 10.f);
    }
    else if (InputManager::GetMouseWheelDown())
    {
        Camera* cam = Camera::GetInstance();
        cam->Move(-cam->transform->look, 10.f);
    }

    if (unit->IsDead())
    {
        return;
    }

    if (InputManager::GetMouseLButtonDown())
    {
        Ray ray = Camera::main->ScreenPointToRay(InputManager::GetMousePosition());
        RaycastHit hit;
        int groundMask = LayerMask::GetMask(Layer::Ground);
        if (Physics::Raycast(ray, &hit, INFINITY, groundMask))
        {

        }
        RaycastHit info;
        int unitMask = LayerMask::GetMask(Layer::Unit, Layer::Building);
        if (Physics::Raycast(ray, &info, INFINITY, unitMask))
        {
            if (targetMode)
            {
                Unit* target = (Unit*)info.collider->gameObject;
                //if (target->team != unit->team && !target->IsDead())
                //{
                //    ReqAttack(target);
                //}
                if (!target->IsDead())
                {
                    ReqAttack(target);
                }
            }
        }


        SetTargetMode(false);
    }
    else if (InputManager::GetMouseRButtonDown())
    {
        SetTargetMode(false);

        Ray ray = Camera::main->ScreenPointToRay(InputManager::GetMousePosition());
        RaycastHit hit;

        int mask = LayerMask::GetMask(Layer::Ground);
        if (Physics::Raycast(ray, &hit, INFINITY, mask))
        {
            Vector3 direction = hit.point - transform->position;
            Vector3::Normalize(&direction);

            // �̵�
            Ray ray2;
            RaycastHit hit2;
            ray2.origin = transform->position;
            ray2.origin.y += 0.1f;
            ray2.direction = direction;
            float dist = Vector3(hit.point - ray2.origin).Length();
            int mask2 = LayerMask::GetMask(Layer::Wall);
            if (Physics::Raycast(ray2, &hit2, dist, mask2))
            {
                // ������ ���� ���� ���
                ReqMove(hit.point);
            }
            else
            {
                // ������ ���� ���� ���
                ReqMove(hit.point, true);
            }
            
            
        }

    }
}

IComponent* NetPlayerController::Clone()
{
    return new NetPlayerController(*this);
}

void NetPlayerController::SetTargetMode(bool _mode)
{
    targetMode = _mode;
    if (targetMode)
    {
        Cursor::SetMode(CursorMode::SingleTarget);
    }
    else
    {
        Cursor::SetMode(CursorMode::Normal);
    }
}

void NetPlayerController::ReqMove(Vector3 _dest, bool _noSearch)
{
    list<Vector3> path;
    int pathCount = 0;

    CPacket* pack = new CPacket();
    pack->Clear();
    *pack << (WORD)GAME_REQ_MOVE << net->number;

    if (_noSearch)
    {
        pathCount = 1;
        *pack << pathCount << _dest.x << _dest.y << _dest.z;

    }
    else
    {
        bool res = agent->Search(_dest, &path);
        if (res)
        {
            pathCount = path.size();
            *pack << pathCount;
            
            for (auto iter : path)
            {
                *pack << iter.x << iter.y << iter.z;
            }

            
        }
        else
        {
            pathCount = 1;
            *pack << pathCount << _dest.x << _dest.y << _dest.z;
        }
    }
    Network::SendPacket(pack);
    delete pack;
    Debug::PrintLine("[Debug] ReqMove ��û / ������ : %d", pathCount);
    
}

void NetPlayerController::ReqAttack(Unit* _target)
{
    INT unitID = _target->GetID();
    CPacket* pack = new CPacket();
    pack->Clear();

    *pack << (WORD)GAME_REQ_ATTACK << net->number << unitID;

    Network::SendPacket(pack);
    delete pack;
    Debug::PrintLine("[Debug] ReqAttack ��û / ������ID : %d / Ÿ��ID : %d", net->number, unitID);
}
