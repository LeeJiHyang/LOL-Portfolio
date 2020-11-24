#pragma once

namespace PKH
{
	class Collider;

	struct Ray
	{
		Ray() {  };
		Ray(Vector3 origin, Vector3 direction)
		{
			this->origin = origin;
			this->direction = direction;
		}

		Vector3 GetPoint(float distance)
		{
			Vector3 result = origin + (direction * distance);
			return result;
		}

		Vector3 origin;
		Vector3 direction;
	};

	struct RaycastHit
	{
		// ������ ������������ �浹�� �������� �Ÿ�.
		float distance;
		// ������ �浹ü�� ���� ��������� �浹 ����.
		Vector3 point;
		// ������ ���� �浹ü.
		Collider* collider = nullptr;

	};

	class Physics
	{
	public:
		static bool Raycast(Ray ray, RaycastHit* outHit, float maxDistance = INFINITY, int layerMask = 0xFFFFFFFF);
		static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance = INFINITY, int layerMask = 0xFFFFFFFF);
	};

}

