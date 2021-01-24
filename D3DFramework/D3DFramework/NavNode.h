#pragma once
#include "GameObject.h"


// ���� ������ ���� �� (���� �÷��̿��� ������ ����)
class NavNode :
    public GameObject
{
public:
    NavNode();
    virtual ~NavNode();
    virtual void Initialize() override;
    virtual void Release() override;
    virtual void Update() override;
    virtual void PostRender() override;

    virtual void OnCollisionEnter(Collider* target) override;


public:
    int uniqueID = -1;
    vector<int> adjacencyArr;
    SphereCollider* collider = nullptr;
};

