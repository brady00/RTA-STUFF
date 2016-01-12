#pragma once
#include "Headers.h"
#include "RenderNode.h"
#include "RenderMaterial.h"

class RenderMaterial;
class RenderShape : public RenderNode
{
protected:
	UINT                        numPrimitives;
	D3D_PRIMITIVE_TOPOLOGY      primitiveType;
	UINT                        startVertex;
	UINT                        startIndex;
	UINT                        numVertices;
	RenderMaterial* renderMaterial;
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewProj;
private:

public:
	RenderShape(){};
	~RenderShape(){};
	inline void Initialize(DirectX::XMFLOAT4X4 *localMatrix) {worldMatrix = *localMatrix;};
	inline void AddToMaterialSet(RenderMaterial *material){ material->getRenderShapes()->AddRenderNode(this); };
	inline void SetWorldMatrix(const DirectX::XMFLOAT4X4 &mat) { worldMatrix = mat; }
	inline void SetWorldMatrix(const DirectX::XMFLOAT4X4 *mat) { worldMatrix = *mat; }
	inline DirectX::XMFLOAT4X4 &GetWorldMatrix(void) { return worldMatrix; }
	inline DirectX::XMFLOAT4X4 *GetWorldMatrixPtr(void) { return &worldMatrix; }
	inline DirectX::XMFLOAT4X4 &GetViewProjMatrix(void) { return viewProj; }
	inline void SetViewProjMatrix(DirectX::XMFLOAT4X4 view) { viewProj = view; }
	inline RenderMaterial* GetRenderMaterial(void){ return renderMaterial; }
	UINT getNumPrimitives(){return numPrimitives;}
	D3D_PRIMITIVE_TOPOLOGY getPrimitiveType() { return primitiveType; }
	UINT getStartVertex(){ return startVertex; }
	UINT getstartIndex(){ return startIndex; }
	UINT getNumVertices(){ return numVertices; }
	void setNumPrimitives(UINT number){ numPrimitives = number; }
	void setPrimitiveType(D3D_PRIMITIVE_TOPOLOGY top) { primitiveType = top; }
	void setStartVertex(UINT start){ startVertex = start; }
	void setstartIndex(UINT start){ startIndex = start; }
	void setNumVertices(UINT num){ numVertices; }
	static void RenderFunction(RenderNode& node);
};

