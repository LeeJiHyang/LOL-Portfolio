#pragma once
#include "Layer.h"

using namespace PKH;

enum class State
{
	IDLE, WALK, ATTACK, SKILL, SKILL2, SKILL3, HURT, READY, END
};
enum class Direction
{
	D, LD, L, LU, U, RU, R, RD, END
};

enum class Team
{
	NEUTRAL,PLAYERTEAM,MONSTERTEAM,END
};

namespace PKH
{
	class IComponent;
	class Transform;
	class Collider;

	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();
		
		virtual void Initialize() = 0;
		virtual void Release() = 0;
		virtual void Update() = 0;
		virtual void PostUpdate();
		virtual void Render();
		virtual void Destroy();
		virtual void PostRender();
		virtual void OnCollisionEnter(const Collider* target);

		// 방향으로 이동
		// 내부에서 정규화 해줌.
		void Move(Vector3 _direction);
		// 위치로 이동
		void MoveToTarget(Vector3 _target);
		// 타겟 따라가기
		void FollowTarget(const GameObject* _target);
		void FollowTarget(const Transform& _targetTransform);
		void FollowTarget(const Vector3& _targetPos);
		// 타겟 오브젝트 방향으로 회전
		void FaceTarget(const GameObject* _target);
		void FaceTarget(const Transform& _targetTransform);
		void FaceTarget(const Vector3& _targetPos);
		// 빌보드
		// GameObject::Update() 호출 후에 호출
		void Billboard();
		void BillboardYaw();


		void SetPosition(Vector3 _vPos);

		template<class T>
		IComponent* AddComponent(const wstring& _key);
		IComponent* AddComponent(const wstring& _key, IComponent* _component);
		void ReleaseComponents();

		// getter
		IComponent* GetComponent(const wstring& _key);
		template<class T>
		IComponent* GetComponent();
		Transform* GetTransform() const { return transform; } //수정
		STAT GetStat() const { return stat; }
		Layer GetLayer() { return this->layer; }

		bool IsDead();
		
		// setter
		void MinusHp(float _damage);
		void SetHp(float _hp);
		void SetLayer(Layer _layer);
		

	public:
		wstring name;
		Transform* transform = nullptr;
		STAT stat;
		Team team = Team::NEUTRAL;
		
		
		bool isVisible = true;
		bool isEnable = true;
		bool dontDestroy = false;
		
		map<wstring, PKH::IComponent*> components;
	protected:
		bool isDead = false;
	private:
		Layer layer = Layer::Default;
	};

	template<class T>
	inline IComponent* GameObject::AddComponent(const wstring & _key)
	{
		T* comp = new T(this);
		if (dynamic_cast<IComponent*>(comp) == nullptr)
		{
			delete comp;
			return nullptr;
		}

		comp->gameObject = this;
		comp->transform = this->transform;
		components[_key] = comp;

		return comp;
	}
	template<class T>
	inline IComponent* GameObject::GetComponent()
	{
		for (auto& iter : components)
		{
			if (dynamic_cast<T*>(iter.second))
			{
				return iter.second;
			}
		}
		return nullptr;
	}
}


