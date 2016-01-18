#include "FBXLoader.h"
#include <fstream>



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
			FbxGeometryElementNormal* lNormalElement = pMesh->GetElementNormal();

			const bool lUseIndex = lUVElement->GetReferenceMode() != FbxGeometryElement::eDirect;

			FbxVector4* pVertices = pMesh->GetControlPoints();

			int lPolyIndexCounter = 0;
			for (int j = 0; j < pMesh->GetPolygonCount(); j++)
			{
				int iNumVertices = pMesh->GetPolygonSize(j);

				for (int k = 0; k < iNumVertices; k++)
				{
					int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

					MyVertex vertex;
					vertex.pos[0] = (float)pVertices[iControlPointIndex].mData[0];
					vertex.pos[0] = -vertex.pos[0];
					vertex.pos[1] = (float)pVertices[iControlPointIndex].mData[1];
					vertex.pos[1] = -vertex.pos[1];
					vertex.pos[2] = (float)pVertices[iControlPointIndex].mData[2];

					int lPolyVertIndex = pMesh->GetPolygonVertex(j, k);
					
					int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyVertIndex) : lPolyVertIndex;
					const int lIndexCount = (lUseIndex) ? lUVElement->GetIndexArray().GetCount() : 0;
					FbxVector2 lUVValue;
					if (lPolyIndexCounter < lIndexCount)
					{
						int lUVIndex = lUseIndex ? lUVElement->GetIndexArray().GetAt(lPolyIndexCounter) : lPolyIndexCounter;
						lUVValue = lUVElement->GetDirectArray().GetAt(lUVIndex);
						lPolyIndexCounter++;
					}
					

					//int lNormalIndex = 0;
					//reference mode is direct, the normal index is same as vertex index.
					//get normals by the index of control vertex
					//if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
					//lNormalIndex = k;
					int lNormalIndex = 0;
					//reference mode is direct, the normal index is same as lIndexByPolygonVertex.
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eDirect)
						lNormalIndex = j;
					//reference mode is index-to-direct, get normals by the index-to-direct
					if (lNormalElement->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
						lNormalIndex = lNormalElement->GetIndexArray().GetAt(j);
					//Got normals of each polygon-vertex.
					FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(lNormalIndex);

					//FbxVector4 lNormal = lNormalElement->GetDirectArray().GetAt(k);
					vertex.uv[0] = lUVValue.mData[0];
					vertex.uv[1] = lUVValue.mData[1];
					vertex.uv[1] = 1 - vertex.uv[1];
					//vertex.uv[0] = 1 - vertex.uv[0];
					vertex.normals[0] = lNormal.mData[0];
					vertex.normals[1] = lNormal.mData[1];
					vertex.normals[2] = lNormal.mData[2];
				
					pOutVertexVector->push_back(vertex);
				}
			}

		}

	}
	return true;
}

bool FileInfo::ExporterHeader::FBXSave(char * fileName, std::vector<MyVertex>& pinVertexVector)
{
	std::ofstream save;
	save.open(fileName, std::ios_base::binary);
	if (save.is_open())
	{
		unsigned int size = pinVertexVector.size();
		save.write((char*)&size, sizeof(unsigned int));
		for (unsigned int i = 0; i < size; i++)
		{
			save.write((char*)&(pinVertexVector)[i].pos[0], sizeof((pinVertexVector)[i].pos[0]));
			save.write((char*)&(pinVertexVector)[i].pos[1], sizeof((pinVertexVector)[i].pos[1]));
			save.write((char*)&(pinVertexVector)[i].pos[2], sizeof((pinVertexVector)[i].pos[2]));
		}
		for (unsigned int i = 0; i < size; i++)
		{
			save.write((char*)&(pinVertexVector)[i].uv[0], sizeof((pinVertexVector)[i].uv[0]));
			save.write((char*)&(pinVertexVector)[i].uv[1], sizeof((pinVertexVector)[i].uv[1]));
		}
		for (unsigned int i = 0; i < size; i++)
		{
			save.write((char*)&(pinVertexVector)[i].normals[0], sizeof((pinVertexVector)[i].normals[0]));
			save.write((char*)&(pinVertexVector)[i].normals[1], sizeof((pinVertexVector)[i].normals[1]));
			save.write((char*)&(pinVertexVector)[i].normals[2], sizeof((pinVertexVector)[i].normals[2]));
		}
	}
	else
		return false;
	return true;
}