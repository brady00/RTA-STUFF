#pragma once
#include "Headers.h"
#include "RenderNode.h"
#include "RenderMaterial.h"

class RenderMaterial;
class RenderShape : public RenderNode
{
protected:
	RenderMaterial* renderMaterial;
	DirectX::XMFLOAT4X4 worldMatrix;
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
	inline RenderMaterial* GetRenderMaterial(void){ return renderMaterial; }
	static void RenderFunc(RenderNode& node);
};

