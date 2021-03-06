#pragma once
#include "RenderNode.h"

class RenderSet
{
protected:
	RenderNode *headPtr;
	RenderNode *tailPtr;
public:
	RenderSet(void){};
	virtual ~RenderSet(void)
	{
		RenderNode* temp = headPtr;
		while (headPtr != nullptr)
		{
			headPtr = headPtr->GetNext();
			delete temp;
			temp = headPtr;
		}
	}
	virtual void AddRenderNode(RenderNode *nodePtr)
	{
		nodePtr->SetNext(0);
		if (0 == headPtr)
		{
			headPtr = nodePtr;
		}
		else
		{
			tailPtr->SetNext(nodePtr);
		}
		tailPtr = nodePtr;
	}
	void ClearRenderSet(void){headPtr = tailPtr = 0;}
	RenderNode *GetHead() { return headPtr; }
};

