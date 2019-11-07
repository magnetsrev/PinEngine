#include "Sprite.h"
#include "../ResourceManager.h"
#include "../PipelineManager.h"

using namespace PinEngine;
using namespace PinEngine::UI;
using namespace std;

shared_ptr<Texture> Sprite::GetTexture()
{
	return texture;
}

void Sprite::AssignTexture(wstring path)
{
	shared_ptr<Texture> temp;
	if (ResourceManager::GetResource(path, temp)) //If found and exists
	{
		texture = temp;
	}
	else //If not found, try to generate
	{
		shared_ptr<Texture> newTexture = make_shared<Texture>(path);
		if (newTexture->IsValid())
		{
			texture = newTexture;
			ResourceManager::RegisterResource(path, newTexture);
		}
		else //If not valid, attempt to set to the default missingtexture
		{
			if (!ResourceManager::GetResource(L"missingtexture", texture))
			{
				ErrorLogger::Log(L"Something went wrong. Missing texture not in resource manager.");
				return;
			}
		}
	}
}

void Sprite::Render()
{
	PipelineManager::SetPipelineState(pipelineState);
	deviceContext->PSSetShaderResources(0, 1, GetTexture()->GetTextureResourceViewAddress());
	UINT offsets = 0;
	deviceContext->IASetVertexBuffers(0, 1, vertexBuffer->GetAddressOf(), vertexBuffer->StridePtr(), &offsets);
	deviceContext->Draw(vertexBuffer->VertexCount(), 0);
}

void Sprite::OnInitialize()
{
	widgetType = WidgetType::Sprite;
	if (!ResourceManager::GetResource(L"missingtexture", texture))
	{
		ErrorLogger::Log(L"Something went wrong. Missing texture not in resource manager.");
	}
}
