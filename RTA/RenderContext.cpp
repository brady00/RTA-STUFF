#include "RenderContext.h"


RenderContext::RenderContext()
{
	renderMaterials = 0;
	ID3D11InputLayout* inputLayout = 0;
	D3D11_PRIMITIVE_TOPOLOGY topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT stride = 0;
	UINT offset = 0;
	ID3D11Buffer* VertexBuffer = 0;
	ID3D11Buffer* IndexBuffer = 0;
	ID3D11VertexShader* VertexShader = 0;
	ID3D11PixelShader* PixelShader = 0;
}


RenderContext::~RenderContext()
{
	if (renderMaterials)
		delete renderMaterials;
	ReleaseCOM(inputLayout);
	ReleaseCOM(VertexBuffer);
	ReleaseCOM(IndexBuffer);
	ReleaseCOM(VertexShader);
	ReleaseCOM(PixelShader);
}

void RenderContext::RenderFunc(RenderNode& node)
{
	RenderContext &context = (RenderContext &)node;
	Renderer::devicecontext->IASetInputLayout(inputLayout);
	Renderer::devicecontext->IASetPrimitiveTopology(topology);
	Renderer::devicecontext->IASetVertexBuffers(0, 1, &VertexBuffer, &stride, &offset);
	Renderer::devicecontext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	Renderer::devicecontext->VSSetShader(VertexShader, NULL, 0);
	Renderer::devicecontext->PSSetShader(PixelShader, NULL, 0);
	Renderer::Render(context.getRenderMaterials());
}


