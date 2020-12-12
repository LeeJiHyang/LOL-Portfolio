#pragma once
#include "IComponent.h"



namespace PKH
{
	class NavMeshMap;

	class NavMeshAgent : public IComponent
	{
	public:
		explicit NavMeshAgent(GameObject* owner);
		explicit NavMeshAgent(const NavMeshAgent& rhs);
		virtual ~NavMeshAgent();

		virtual void Update();

		// IComponent��(��) ���� ��ӵ�
		virtual IComponent* Clone() override;

	public:
		bool SetDestination(const Vector3& target);
		void Stop();
		void Resume();
		void Move(const Vector3& offset);
		void ResetPath();

	private:
		NavMeshMap* navMeshMap = nullptr;
		stack<Vector3> path;
		Vector3 nextPosition;
		bool isDestination = true;

		float stoppingDistance = 0.1f;
		Vector3 destination;
		float radius = 0.f;
		float speed = 5.f;
		
		bool isMoving = true;

	};
}


