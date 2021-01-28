#pragma once
#include "CustomMesh.h"

namespace PKH
{
	class Plane : public CustomMesh
	{
	public:
		explicit Plane(GameObject* owner);
		explicit Plane(const Plane& rhs);
		virtual ~Plane();


		// CustomMesh��(��) ���� ��ӵ�
		virtual void CreateCustomMesh() override;


		// CustomMesh��(��) ���� ��ӵ�
		virtual PKH::IComponent* Clone() override;

	};
}


