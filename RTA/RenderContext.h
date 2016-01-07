#pragma once
#include "RenderNode.h"
#include "RenderSet.h"

class RenderContext : RenderNode
{
protected:
	RenderSet* renderMaterials;
private:

public:
	RenderContext();
	~RenderContext();
	inline RenderSet* getRenderMaterials(void){ return renderMaterials; }
	inline void CreateRenderMaterials(void){ renderMaterials = new RenderSet; }
	inline void AddRenderMaterials(RenderNode* node){ renderMaterials->AddRenderNode(node); }
	inline void ClearRenderMaterials(void){ renderMaterials->ClearRenderSet(); }
};

