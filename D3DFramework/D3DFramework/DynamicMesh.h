#pragma once
#include "Mesh.h"

namespace PKH
{
	class DynamicMesh :
		public Mesh
	{
	public:
		explicit DynamicMesh(GameObject* owner);
		explicit DynamicMesh(const DynamicMesh& rhs);
		virtual ~DynamicMesh();
		// Mesh��(��) ���� ��ӵ�
		virtual IComponent * Clone() override;
	};
}


