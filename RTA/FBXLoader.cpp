#include "FBXLoader.h"



FbxManager* g_pFbxSdkManager = nullptr;

bool FileInfo::ExporterHeader::FBXLoad(char * fileName, std::vector<MyVertex>* pOutVertexVector)
{
	if (g_pFbxSdkManager == nullptr)
	{
		g_pFbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, fileName);
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, fileName);
	FbxStringList UVsetNames;

	bool bSuccess = pImporter->Initialize(fileName, -1, g_pFbxSdkManager->GetIOSettings());
	if (!bSuccess) return false;

	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess) return false;

	pImporter->Destroy();

	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();

	if (pFbxRootNode)
	{
		for (int i = 0; i < pFbxRootNode->GetChildCount(); i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
			pMesh->GetUVSetNames(UVsetNames);
			const char * UVList = UVsetNames.GetStringAt(i);
			const FbxGeometryElementUV * lUVElement = pMesh->GetElementUV(UVList);

			const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;

			FbxVector4* pVertices = pMesh->GetControlPoints();

			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);


				for (int k = 0; k < iNumVertices; k++)
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					MyVertex vertex;
					vertex.pos[0] = (float)pVertices[iControlPointIndex].mData[0];
					vertex.pos[1] = (float)pVertices[iControlPointIndex].mData[1];
					vertex.pos[2] = (float)pVertices[iControlPointIndex].mData[2];

					FbxVector2 lUVValue;
					int lPolyVertIndex = pMesh->GetPolygonVertex(j, k);
					
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;
					lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);


					pOutVertexVector->push_back(vertex);

				}
			}

		}

	}
	return true;
}


