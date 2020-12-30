#include "stdafx.h"
#include "Animation2D.h"


PKH::Animation2D::Animation2D(GameObject* owner)
	:IComponent(owner)
{

}

PKH::Animation2D::Animation2D(const Animation2D& rhs)
	:IComponent(rhs)
{
}

PKH::Animation2D::~Animation2D()
{
}

void PKH::Animation2D::Update()
{
	if (nullptr == gameObject)
	{
		return;
	}
		

	tick += TimeManager::DeltaTime();
	if (tick > delay)
	{
		tick = 0;

		if (currentTexture == endTexture)
		{
			isEndFrame = true;

			if (isLoop == true)
			{
				currentTexture = startTexture;
			}
			return;
		}
		else
		{
			isEndFrame = false;
		}
		int i = int(currentTexture);
		i++;
		
		currentTexture = (TextureID)i;
		
	}
	CustomMesh* mesh = (CustomMesh*)gameObject->GetComponent(L"CustomMesh");
	wstring key = RenderManager::GetTextureKey(currentTexture);
	mesh->SetTexture(key.c_str());

}

void PKH::Animation2D::SetSprite(TextureID _startTexture, TextureID _endTexture)
{
	startTexture = _startTexture;
	endTexture = _endTexture;
	if ((int)currentTexture < (int)startTexture
		|| (int)currentTexture >(int)endTexture)
	{
		currentTexture = startTexture;
	}
	
}

void PKH::Animation2D::SetCurrentSprite(TextureID _currentTexture)
{
	currentTexture = _currentTexture;
}

void PKH::Animation2D::SetLoop(bool _isLoop)
{
	isLoop = _isLoop;
}

void PKH::Animation2D::SetDelay(float _delay)
{
	delay = _delay;
}

void PKH::Animation2D::SetTick(float _tick)
{
	tick = _tick;
}

TextureID PKH::Animation2D::GetCurrentSprite()
{
	return currentTexture;
}

TextureID PKH::Animation2D::GetStartSprite()
{
	return startTexture;
}

TextureID PKH::Animation2D::GetEndSprite()
{
	return endTexture;
}

bool PKH::Animation2D::IsEndFrame()
{
	return isEndFrame;
}

IComponent* PKH::Animation2D::Clone()
{
    return nullptr;
}
