#include "RenderMaterial.h"
#include "DDSTextureLoader.h"
#include "Renderer.h"


RenderMaterial::RenderMaterial()
{
}


RenderMaterial::~RenderMaterial()
{
}

void RenderMaterial::RenderFunc(RenderNode& node)
{
	Renderer::devicecontext->PSSetShaderResources(0, 1, &ShaderResourceView);
	Renderer::Render(renderShapes);
}

void RenderMaterial::CreateTexture(const WCHAR *buffer)
{
	CreateDDSTextureFromFile(Renderer::device, buffer, (ID3D11Resource**)&texture, &ShaderResourceView);
}

