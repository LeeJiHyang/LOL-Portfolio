#include "stdafx.h"
#include "GameObject.h"
#include "IComponent.h"
#include "transform.h"
#include "StaticMesh.h"
#include "Collider.h"

using namespace PKH;

PKH::GameObject::GameObject()
{
	transform = (Transform*)AddComponent<Transform>(L"Transform");
}

PKH::GameObject::~GameObject()
{
	ReleaseComponents();
}

void PKH::GameObject::Update()
{
	for (auto& comp : components)
	{
		comp.second->Update();
	}
}

void PKH::GameObject::PostUpdate()
{
}

void PKH::GameObject::Render()
{
	if (isVisible == false) return;

	for (auto& comp : components)
	{
		CustomMesh* mesh = dynamic_cast<CustomMesh*>(comp.second);
		if (mesh != nullptr)
		{
			mesh->Render();
			continue;
		}

		StaticMesh* smesh = dynamic_cast<StaticMesh*>(comp.second);
		if (smesh != nullptr)
		{
			smesh->Render();
			continue;
		}
		
	}
}

void PKH::GameObject::Destroy()
{
	if (dontDestroy) return;
	isDead = true;
}

void PKH::GameObject::PostRender()
{
	for (auto& comp : components)
	{
		Collider* collider = dynamic_cast<Collider*>(comp.second);
		if (collider != nullptr)
		{
			collider->Render();
			continue;
		}

	}
}

void PKH::GameObject::OnCollisionEnter(const Collider* target)
{


}

void PKH::GameObject::Move(Vector3 _direction)
{
	Vector3::Normalize(&_direction);
	transform->position.x += _direction.x * TimeManager::DeltaTime();
	transform->position.y += _direction.y * TimeManager::DeltaTime();
	transform->position.z += _direction.z * TimeManager::DeltaTime();
}

void PKH::GameObject::MoveToTarget(Vector3 _target)
{
	Vector3 dir = _target - transform->position;
	Vector3::Normalize(&dir);
	transform->position.x += dir.x * TimeManager::DeltaTime();
	transform->position.y += dir.y * TimeManager::DeltaTime();
	transform->position.z += dir.z * TimeManager::DeltaTime();
}

void PKH::GameObject::FollowTarget(const GameObject* _target)
{
	MoveToTarget(_target->transform->position);
}

void PKH::GameObject::FollowTarget(const Transform& _targetTransform)
{
	MoveToTarget(_targetTransform.position);
}

void PKH::GameObject::FollowTarget(const Vector3& _targetPos)
{
	MoveToTarget(_targetPos);
}

void PKH::GameObject::FaceTarget(const GameObject* _target)
{
	transform->LookAt(_target->transform->position);
}

void PKH::GameObject::FaceTarget(const Transform& _targetTransform)
{
	transform->LookAt(_targetTransform.position);
}

void PKH::GameObject::FaceTarget(const Vector3& _targetPos)
{
	transform->LookAt(_targetPos);
}

void PKH::GameObject::Billboard()
{
	D3DXMATRIX matScale, matView;
	D3DXMatrixIdentity(&matView);
	matView = Camera::GetViewMatrix();

	memset(&matView._41, 0, sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVECTOR3 BillPos = transform->position;
	D3DXMatrixScaling(&matScale, transform->scale.x, transform->scale.y, transform->scale.z);

	

	//�̵� �κ�
	memcpy(&matView._41, &BillPos, sizeof(D3DXVECTOR3));
	//�̵��κ��� �ݿ����ݴϴ�. �ٽ� ��ǥ�� ��ġ�� �̵������ִ� ó���Դϴ�.

	//RenderManager::SetTransform(D3DTS_WORLD, &matView);
	transform->world = matScale*matView;
}

void PKH::GameObject::BillboardYaw()
{
	D3DXMATRIX matScale, matView, matBill;
	
	matView = Matrix::identity;
	matView = Camera::GetViewMatrix();
	memset(&matBill._41, 0, sizeof(D3DXVECTOR3));

	matBill = Matrix::identity;
	// ������� Y�� ȸ����İ��� ���������
	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	D3DXMatrixInverse(&matBill, 0, &matBill);

	
	//������
	D3DXMatrixScaling(&matScale, transform->scale.x, transform->scale.y, transform->scale.z);


	// �̵�
	D3DXVECTOR3 BillPos = transform->position;
	memcpy(&matBill._41, &BillPos, sizeof(D3DXVECTOR3));
	transform->world = matScale * matBill;


}


void PKH::GameObject::SetPosition(Vector3 _vPos)
{
	transform->position = _vPos;
}

IComponent* PKH::GameObject::AddComponent(const wstring& _key, IComponent* _component)
{
	if (_component == nullptr) return nullptr;
	_component->gameObject = this;
	_component->transform = this->transform;
	components[_key] = _component;
	return _component;
}

void PKH::GameObject::ReleaseComponents()
{
	auto iter = components.begin();
	auto end = components.end();
	for (; iter != end;++iter)
	{
		delete iter->second;
	}
	components.clear();
}

IComponent* PKH::GameObject::GetComponent(const wstring& _key)
{
	auto f = components.find(_key);
	if (f == components.end()) return nullptr;
	return f->second;
}

bool PKH::GameObject::IsDead()
{
	return isDead;
}

void PKH::GameObject::SetLayer(Layer _layer)
{
	if (this->layer == _layer) return;

	this->layer = _layer;
	ObjectManager::RemoveObject(this);
	ObjectManager::AddObject(this, _layer);
}



