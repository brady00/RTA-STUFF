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

}

void RenderMaterial::CreateTexture(const WCHAR *buffer)
{
	//CreateDDSTextureFromFile(Renderer::device, buffer, &texture)
}

