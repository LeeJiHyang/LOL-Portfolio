#pragma once
#include "Mesh.h"
class Triangle :
    public Mesh
{
public:
    Triangle();
    virtual ~Triangle();

    // Mesh��(��) ���� ��ӵ�
    virtual void Render() override;

};

