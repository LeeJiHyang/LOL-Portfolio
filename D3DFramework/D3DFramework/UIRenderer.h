#pragma once
#include "Renderer.h"

namespace KST
{
	class UIRenderer :
		public Renderer
	{
	public:
		explicit UIRenderer(GameObject* owner);
		// Renderer��(��) ���� ��ӵ�
		virtual IComponent* Clone() override;
		virtual void Render() override;

		void SetMesh(PKH::Mesh* mesh);



	private:
		PKH::Mesh* mesh = nullptr;
	};


}