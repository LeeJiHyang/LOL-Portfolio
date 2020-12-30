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

		// ī�޶� ����
		void Shake(float _duration = 0.05f, float _magnitude = 0.03f);
		void UpdateShake();
		void SetShakeDuration(float _duration);

	private:
		void PerspectiveProjection();
		void OrthogonalProjection();

	public:
		static Camera* CreateCamera(wstring id);
		static void DeleteCamera(wstring id);

	private:
		float fovY = D3DXToRadian(90.f);
		float nearClipPlane = 1.f;
		float farClipPlane = 1000.f;
		bool isProjection3D;
		Matrix viewMatrix;
		Matrix projectionMatrix;
		GameObject* target = nullptr;

		// ī�޶� õõ�� ���󰡱�
		bool isSlowChase = false;
		float slowTime = 0.f;

		// ī�޶� ����
		bool isShake = false;
		float shakeDuration;
		float shakeMagnitude;
		Vector3 originCamPos;

		// ī�޶� ���� �ƿ�
		float zoomSpeed = 1.f;
		Vector3 offset = { 0,6,3 };

		// ������ ������
		int screenW = dfCLIENT_WIDTH;
		int screenH = dfCLIENT_HEIGHT;
	public:
		static map<wstring, Camera*> cams;
		static Camera* main;

		// ����׿�
		bool topViewFlag = false;
		Vector3 topViewPos = { 0,100,0 };
		Vector3 topViewLook = { 0,0,1 };
	};
}