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
		// GameObject을(를) 통해 상속됨
		virtual void Update() override;
		virtual void Render() override;
		virtual void Initialize() override;
		virtual void Release() override;

		Vector3 GetPosition();
		float GetX();
		float GetY();
		Matrix GetViewMatrix();
		Matrix GetProjectionMatrix();
		bool GetProjection3D();
		void SetProjection3D(bool ProjectionSet);

		Vector3 ScreenToWorldPoint(const Vector3& position, float zPos = 1.f);
		Vector3 WorldToScreenPoint(const Vector3& position);
		Ray ScreenPointToRay(Vector3 pos);

		void SetTarget(GameObject* _target);
		void SlowChaseTarget(GameObject* tar);

		// 카메라 흔들기
		void Shake(float _duration = 0.05f, float _magnitude = 0.03f);
		void UpdateShake();
		void SetShakeDuration(float _duration);

	private:
		void PerspectiveProjection();
		void OrthogonalProjection();

	public:
		static Camera* CreateCamera(wstring key);
		static void DeleteCamera(wstring key);

	private:
		float nearClipPlane = 1.f;
		float farClipPlane = 1000.f;
		bool isProjection3D;
		Matrix viewMatrix;
		Matrix projectionMatrix;
		GameObject* target = nullptr;

		// 카메라 천천히 따라가기
		bool isSlowChase = false;
		float slowTime = 0.f;

		// 카메라 흔들기
		bool isShake = false;
		float shakeDuration;
		float shakeMagnitude;
		Vector3 originCamPos;

		// 카메라 줌인 아웃
		float zoomSpeed = 3.f;
		Vector3 offset = { 0,6,-3 };
	public:
		static map<wstring, Camera*> cams;
		static Camera* main;
	};
}