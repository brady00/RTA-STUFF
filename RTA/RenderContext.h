#pragma once
#include "RenderNode.h"
#include "RenderSet.h"
#include "Renderer.h"
class RenderContext : RenderNode
{
protected:
	RenderSet* renderMaterials;
	ID3D11InputLayout* inputLayout;
	D3D11_PRIMITIVE_TOPOLOGY topology;
	UINT stride;
	UINT offset;
	ID3D11Buffer* VertexBuffer;
	ID3D11Buffer* IndexBuffer;
	ID3D11VertexShader* VertexShader;
	ID3D11PixelShader* PixelShader;

private:

public:
	RenderContext();
	~RenderContext();
	inline RenderSet& getRenderMaterials(void){ return *renderMaterials; }
	inline void CreateRenderMaterials(void){ renderMaterials = new RenderSet; }
	inline void AddRenderMaterials(RenderNode* node){ renderMaterials->AddRenderNode(node); }
	inline void ClearRenderMaterials(void){ renderMaterials->ClearRenderSet(); }
	void CreateVertexBuffer();
	void RenderFunc(RenderNode& node);
};

