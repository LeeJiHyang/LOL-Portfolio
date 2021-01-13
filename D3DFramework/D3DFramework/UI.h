#pragma once
#include "GameObject.h"
#include "Rectangle.h"

namespace PKH
{
    class UI : public GameObject
    {
    public:
        UI();
        virtual ~UI();

        virtual void Update() override;
        virtual void Render() override;

        void UpdateEvent();
        void ClearEvent();

        // �̺�Ʈ ������
        virtual void OnHover();
        virtual void OnLeave();
        virtual void OnLButtonDown();
        virtual void OnLButtonUp();
        virtual void OnRButtonDown();
        virtual void OnRButtonUp();
        virtual void OnClick();
        virtual void OnChangedText();

        Vector3 GetSize();
        Vector3 GetLocation();

        void SetSize(int w, int h);
        void SetSizeByTexture();
        // �»�� ����

        void SetLocation(int x, int y);
        void SetTexture(const wstring& _key);


    public:
        
        //wstring textureKey;
        Rectangle* mesh = nullptr;
        Texture* texture = nullptr;

        // �ؽ�Ʈ ����
        wstring text; // ����� �ؽ�Ʈ (RW)
        D3DCOLOR foreColor;
        Vector3 textOffsetPosition;// �ؽ�Ʈ�� ����� local(offset)��ġ (RW) 
        bool textRenderFlag = false;// �ؽ�Ʈ�� ����� UI�ΰ�? (RW)

        // ���� ���� (R)
        int width = 0;
        int height = 0;

    public:
        // �̺�Ʈ �ݹ� (RW)
        void (*Click)() = nullptr;
        void (*LButtonDown)() = nullptr;
        void (*LButtonUp)() = nullptr;
        void (*RButtonDown)() = nullptr;
        void (*RButtonUp)() = nullptr;
        void (*Hover)() = nullptr;
        void (*Leave)() = nullptr;
        void (*ChangedText)() = nullptr;

    private:
        // �̺�Ʈ ���� 
        bool isHover = false;
        bool isLeave = false;
        bool isLButtonDown = false;
        bool isRButtonDown = false;

        wstring oldText;

    };



}
