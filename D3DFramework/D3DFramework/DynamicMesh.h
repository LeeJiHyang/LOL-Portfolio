#pragma once
#include "Mesh.h"

namespace PKH
{
	class DynamicMesh :
		public Mesh
	{
		// Mesh��(��) ���� ��ӵ�
		virtual IComponent * Clone() override;
	};
}


