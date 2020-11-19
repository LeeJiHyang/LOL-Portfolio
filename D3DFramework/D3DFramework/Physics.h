#pragma once

namespace PKH
{
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
		// ������ �浹ü�� ���� ��������� �浹 ����.
		Vector3 point;

	};

	class Physics
	{
	public:
		static bool Raycast(Vector3 origin, Vector3 direction, float maxDistance = INFINITY, int layerMask = 0);
	};

}

