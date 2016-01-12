#pragma once
#include "RenderSet.h"

class RenderMaterial : public RenderNode
{
protected:
	RenderSet* renderShapes;
	//textures
	ID3D11Texture2D* texture;
	ID3D11ShaderResourceView* ShaderResourceView;
private:

public:
	RenderMaterial();
	~RenderMaterial();
	inline RenderSet* getRenderShapes(void){ return renderShapes; }
	inline void CreateRenderShapes(void){ renderShapes = new RenderSet; }
	inline void AddRenderShapes(RenderNode* node){ renderShapes->AddRenderNode(node); }
	inline void ClearRenderShapes(void){ renderShapes->ClearRenderSet(); }
	static void RenderFunction(RenderNode& node);
	void CreateTexture(const WCHAR *buffer);

};

