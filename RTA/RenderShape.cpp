#include "RenderShape.h"
#include "Renderer.h"

void RenderShape::RenderFunction(RenderNode& node)
{
	RenderShape &shape = (RenderShape &)node;
	//RenderContext &context = *shape.GetRenderContext();

	// Set matrices
	Renderer::SetPerObjectData(shape.GetViewMatrix(), shape.GetProjMatrix(), *shape.GetWorldMatrixPtr());
	Renderer::devicecontext->IASetPrimitiveTopology(shape.getPrimitiveType());

	Renderer::devicecontext->Draw(shape.getNumPrimitives(), shape.startVertex);
} 
