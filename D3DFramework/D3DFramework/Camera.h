#pragma once
#include "GameObject.h"

namespace PKH
{
	class Camera : public GameObject
	{
	private:
		Camera();
		virtual ~Camera();
	public:
		static Camera* GetInstance();
		static void Destroy();
		// GameObject��(��) ���� ��ӵ�
		virtual void Update() override;
		virtual void Render() override;

		static Vector3 GetPosition();
		static float GetX();
		static float GetY();

	};
}


