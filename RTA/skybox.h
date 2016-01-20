#pragma once
#include "RenderContext.h"
#include "VeretxStructures.h"
#include <vector>
class skybox
{
private:
	RenderSet* renderSet;
	std::vector<PosUv> CreateCube(RenderContext* renderContext);
public:
	bool Create();
	bool Render();
	skybox(){}
	~skybox();
};

