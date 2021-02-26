#pragma once
#include "GameObject.h"

using namespace Engine;

class Environment :
	public GameObject
{
public:
	Environment();
	virtual ~Environment();
	// GameObject��(��) ���� ��ӵ�
	virtual void Initialize() override;
	virtual void Release() override;
	virtual void Update() override;
};

