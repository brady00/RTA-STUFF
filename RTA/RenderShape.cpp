#include "RenderShape.h"
#include "Renderer.h"

void RenderShape::RenderFunc(RenderNode& node)
{
	RenderShape &shape = (RenderShape &)node;
	//RenderContext &context = *shape.GetRenderContext();

	// Set matrices
	DirectX::XMMATRIX world = XMLoadFloat4x4(shape.GetWorldMatrixPtr());
	DirectX::XMMATRIX worldViewProj = world*XMLoadFloat4x4(&shape.GetViewProjMatrix());
	DirectX::XMFLOAT4X4 tempMVP;
	DirectX::XMFLOAT4X4 tempWorld;
	DirectX::XMStoreFloat4x4(&tempMVP, worldViewProj);
	DirectX::XMStoreFloat4x4(&tempWorld, world);
	Renderer::SetPerObjectData(tempMVP, tempWorld);

	Renderer::devicecontext->DrawIndexed(shape.getNumPrimitives() * 3,
		shape.startIndex, shape.startVertex);
}
