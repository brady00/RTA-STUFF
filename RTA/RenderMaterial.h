#pragma once
#include "RenderSet.h"

class RenderMaterial
{
protected:
	RenderSet* renderShapes;
	//textures
	ID3D11Texture2D* texture;
private:

public:
	RenderMaterial();
	~RenderMaterial();
	inline RenderSet* getRenderShapes(void){ return renderShapes; }
	inline void CreateRenderShapes(void){ renderShapes = new RenderSet; }
	inline void AddRenderShapes(RenderNode* node){ renderShapes->AddRenderNode(node); }
	inline void ClearRenderShapes(void){ renderShapes->ClearRenderSet(); }
	void RenderFunc(RenderNode& node);
	void CreateTexture(const WCHAR *buffer);

};

