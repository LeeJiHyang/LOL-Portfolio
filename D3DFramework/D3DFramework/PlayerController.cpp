#include "stdafx.h"
#include "PlayerController.h"
#include "Collider.h"


PlayerController::PlayerController(GameObject* owner)
    :IComponent(owner)
{
}

PlayerController::PlayerController(const PlayerController& rhs)
    :IComponent(rhs)
{
}

PlayerController::~PlayerController()
{
}

void PlayerController::Update()
{
    if (gameObject == nullptr)return;
    
    if (InputManager::GetKey('W'))
    {
        //gameObject->transform->position.z += 10.f* TimeManager::DeltaTime();
        transform->position += transform->look * 10.f * TimeManager::DeltaTime();
    }
    if (InputManager::GetKey('A'))
    {
        //gameObject->transform->position.x -= 10.f * TimeManager::DeltaTime();
        gameObject->transform->eulerAngles.y -= D3DXToRadian(10);// *TimeManager::DeltaTime();
    }
    if (InputManager::GetKey('S'))
    {
        gameObject->transform->position.z -= 10.f * TimeManager::DeltaTime();
    }
    if (InputManager::GetKey('D'))
    {
        //gameObject->transform->position.x += 10.f * TimeManager::DeltaTime();
        gameObject->transform->eulerAngles.y += D3DXToRadian(10);// *TimeManager::DeltaTime();
    }

    if (InputManager::GetKey('Q'))
    {
        Camera::GetInstance()->transform->position.x -= TimeManager::DeltaTime();
    }
    if (InputManager::GetKey('E'))
    {
        Camera::GetInstance()->transform->position.x += TimeManager::DeltaTime();
    }

    if (InputManager::GetMouseWheelUp())
    {
        Camera* cam = Camera::GetInstance();
        cam->Move(cam->transform->look ,10.f);
    }
    else if (InputManager::GetMouseWheelDown())
    {
        Camera* cam = Camera::GetInstance();
        cam->Move(-cam->transform->look ,10.f);
    }

    if (InputManager::GetMouseLButton())
    {
        Ray ray = Camera::main->ScreenPointToRay(InputManager::GetMousePosition());
        RaycastHit hit;
        if (Physics::Raycast(ray, &hit))
        {
            printf("%d,%d,%d\n", (int)hit.point.x, (int)hit.point.y, (int)hit.point.z);

            //hit.collider->SetColor(D3DCOLOR_ARGB(255, 255, 0, 0));
            //hit.collider->gameObject->Destroy();

        }
    }
    else if (InputManager::GetMouseRButton())
    {
        Ray ray = Camera::main->ScreenPointToRay(InputManager::GetMousePosition());
        RaycastHit hit;
        int mask = LayerMask::GetMask(Layer::Ground);
        if (Physics::Raycast(ray, &hit, INFINITY, mask))
        {
            //Vector3 direction = hit.point - transform->position;
            //Vector3::Normalize(&direction);

            //float angle = Vector3::AngleY(transform->look, direction);
            //angle = D3DXToRadian(angle);
            //gameObject->transform->eulerAngles.y -= angle *TimeManager::DeltaTime();

            // ȸ��
            Vector3 direction = hit.point - transform->position;
            Vector3::Normalize(&direction);

            //float angle = Vector3::Dot(transform->look, direction); // == cos@
            //angle = acosf(angle);
            float angle = Vector3::AngleY(Vector3(0,0,1), direction);
            //angle = D3DXToRadian(angle);
            

            gameObject->transform->eulerAngles.y = angle;
            
            gameObject->MoveToTarget(hit.point,5.f);

        }
    }
}

IComponent* PlayerController::Clone()
{
    return new PlayerController(*this);
}
