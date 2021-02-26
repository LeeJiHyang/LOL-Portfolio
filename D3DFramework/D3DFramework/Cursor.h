#pragma once
#include "UI.h"

namespace Engine
{
	enum class CursorMode { Normal, SingleTarget };

	class Cursor : public UI
	{
	private:
		Cursor();
		~Cursor();
	public:
		static Cursor* GetInstance();
		static void Destroy();

		static Vector3 GetMousePos();
		static void Show();
		static void Show(bool on);
		static void Hide();


		virtual void Update() override;

		virtual void Initialize() override;
		virtual void Release() override;

		static bool IsVisible();
		static void SetRenderCenter(bool isCenter);

		static void SetMode(CursorMode _mode);
		static void ChangeMode();

	private:
		bool isRenderCenter = false;
		CursorMode mode = CursorMode::Normal;
	};
}


