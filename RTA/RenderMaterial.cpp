#include "RenderMaterial.h"
#include "DDSTextureLoader.h"
#include "Renderer.h"


RenderMaterial::RenderMaterial()
{
	//RenderFunc = RenderFunction;
}


RenderMaterial::~RenderMaterial()
{
	ReleaseCOM(ShaderResourceView);
	ReleaseCOM(texture);
	if (renderShapes)
		delete renderShapes;
}

void RenderMaterial::RenderFunction(RenderNode& node)
{
	RenderMaterial &material = (RenderMaterial&)node;
	Renderer::devicecontext->PSSetShaderResources(0, 1, &material.ShaderResourceView);
	Renderer::Render(material.renderShapes);
}

void RenderMaterial::CreateTexture(const WCHAR *buffer)
{
	CreateDDSTextureFromFile(Renderer::device, buffer, (ID3D11Resource**)&texture, &ShaderResourceView);
}

