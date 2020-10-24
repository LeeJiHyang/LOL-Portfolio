#include "stdafx.h"
#include "Monster.h"
#include "Plane.h"
#include "Rectangle.h"

Monster::Monster()
{
	Mesh* mesh = (Mesh*)AddComponent<PKH::Rectangle>(L"Mesh2");
	
	mesh->SetTexture(TextureKey::SKY_F);
	
}

Monster::~Monster()
{
}

void Monster::Update()
{
	//FaceTarget(Camera::GetInstance());
	GameObject::Update();

	Billboard();
	
}

void Monster::Render()
{
	
	GameObject::Render();
}

void Monster::Billboard()
{

	D3DXMATRIX matView;
	D3DXMatrixIdentity(&matView);
	matView = Camera::GetViewMatrix();

	memset(&matView._41, 0, sizeof(D3DXVECTOR3));
	D3DXMatrixInverse(&matView, 0, &matView);

	D3DXVECTOR3 BillPos = transform->position;


	//�̵� �κ�
	memcpy(&matView._41, &BillPos, sizeof(D3DXVECTOR3));
	//�̵��κ��� �ݿ����ݴϴ�. �ٽ� ��ǥ�� ��ġ�� �̵������ִ� ó���Դϴ�.

	//D2DRenderManager::GetDevice()->SetTransform(D3DTS_WORLD, &matView);
	transform->world = matView;
	//������ �̵����� �״�� �ݿ��ϰ�, ȸ������ ���ؼ� ������� ����ķ�
	//������ ����� ������ķ� �����մϴ�.
}
