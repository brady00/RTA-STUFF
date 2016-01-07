#include "FBXLoader.h"



bool FileInfo::ExporterHeader::FBXLoad(FbxDocument * pDocument, char * fileName)
{
	FILE		*	mFilePointer = NULL;
	FbxManager	*	mManager = FbxManager::Create();
	FbxScene	*	lScene = (FbxScene*)(pDocument);
	bool            lIsAScene = (lScene != NULL);
	bool            lResult = false;


	FBXSDK_fopen(mFilePointer, fileName, "r");

	if (lIsAScene)
	{
		FbxNode* lRootNode = lScene->GetRootNode();
		FbxNodeAttribute * lRootNodeAttribute = FbxNull::Create(lScene, "");
		lRootNode->SetNodeAttribute(lRootNodeAttribute);

		int lSize;
		char* lBuffer = NULL;
		if (mFilePointer != NULL)
		{
			//To obtain file size
			fseek(mFilePointer, 0, SEEK_END);
			lSize = ftell(mFilePointer);
			rewind(mFilePointer);

			//Read file content to a string.
			lBuffer = (char*)malloc(sizeof(char)*lSize + 1);
			size_t lRead = fread(lBuffer, 1, lSize, mFilePointer);
			lBuffer[lRead] = '\0';
			FbxString lString(lBuffer);

			//Parse the string to get name and relation of Nodes. 
			FbxString lSubString, lChildName, lParentName;
			FbxNode* lChildNode;
			FbxNode* lParentNode;
			FbxNodeAttribute* lChildAttribute;

			int lEndTokenCount = lString.GetTokenCount("\n");

			for (int i = 0; i < lEndTokenCount; i++)
			{
				lSubString = lString.GetToken(i, "\n");
				FbxString lNodeString;
				lChildName = lSubString.GetToken(0, "\"");
				lParentName = lSubString.GetToken(2, "\"");

				//Build node hierarchy.
				if (lParentName == "RootNode")
				{
					lChildNode = FbxNode::Create(lScene, lChildName.Buffer());
					lChildAttribute = FbxNull::Create(mManager, "");
					lChildNode->SetNodeAttribute(lChildAttribute);

					lRootNode->AddChild(lChildNode);
				}
				else
				{
					lChildNode = FbxNode::Create(lScene, lChildName.Buffer());
					lChildAttribute = FbxNull::Create(lScene, "");
					lChildNode->SetNodeAttribute(lChildAttribute);

					lParentNode = lRootNode->FindChild(lParentName.Buffer());
					lParentNode->AddChild(lChildNode);
				}
			}
			free(lBuffer);
		}
		lResult = true;
	}
	return lResult;
}


