#pragma once
#include "Headers.h"

class RenderNode;
typedef void(*EDRenderFunc)(RenderNode &node);

class RenderNode
{
protected:
	RenderNode *nextPtr;
public:
	RenderNode() { nextPtr = 0; }
	virtual ~RenderNode(){};
	EDRenderFunc RenderFunc;
	void RenderProcess(){RenderFunc(*this);}
	inline RenderNode *GetNext(void) { return nextPtr; }
	inline void SetNext(RenderNode *nodePtr) { nextPtr = nodePtr; }
};

