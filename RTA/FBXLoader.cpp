#include "FBXLoader.h"
#include "Header.h"
#include <fstream>


FbxManager* g_pFbxSdkManager = nullptr;

bool FileInfo::ExporterHeader::FBXLoad(char * fileName, std::vector<MyVertex>* pOutVertexVector, char* binaryFile)
{
	std::ifstream binary;
	binary.open(binaryFile, std::ios_base::binary);
	if (binary.is_open())
	{
		binary.read((char*)&version, sizeof(version));
		binary.read((char*)&updateTime, sizeof(updateTime));
	}
	if (g_pFbxSdkManager == nullptr)
	{
		g_pFbxSdkManager = FbxManager::Create();

		FbxIOSettings* pIOsettings = FbxIOSettings::Create(g_pFbxSdkManager, IOSROOT);
		g_pFbxSdkManager->SetIOSettings(pIOsettings);
	}

	FbxImporter* pImporter = FbxImporter::Create(g_pFbxSdkManager, fileName);
	FbxScene* pFbxScene = FbxScene::Create(g_pFbxSdkManager, fileName);
	FbxIOFileHeaderInfo* header = pImporter->GetFileHeaderInfo();

	if (header->mFileVersion != version)
	{
		if (header->mCreationTimeStampPresent)
		{
			if (header->mCreationTimeStamp.mSecond != updateTime)
				return true;
		}
	}
	version = header->mFileVersion;
	updateTime = header->mCreationTimeStamp.mSecond;
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
					pMesh->get

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
			//skeleton stufff
			LoadMesh_Skeleton(pMesh);

			DisplayAnimation(pMesh->GetScene());
		}

	}
	return true;
}

void FileInfo::ExporterHeader::LoadMesh_Skeleton(FbxMesh *fbxMesh)
{
	int numDeformers = fbxMesh->GetDeformerCount();
	FbxSkin* skin = (FbxSkin*)fbxMesh->GetDeformer(0, FbxDeformer::eSkin);
	if (skin != 0)
	{
		int boneCount = skin->GetClusterCount();
		for (int boneIndex = 0; boneIndex < boneCount; boneIndex++)
		{
			FbxCluster* cluster = skin->GetCluster(boneIndex);
			FbxNode* bone = cluster->GetLink(); // Get a reference to the bone's node

			// Get the bind pose
			FbxAMatrix bindPoseMatrix;
			cluster->GetTransformLinkMatrix(bindPoseMatrix);

			int *boneVertexIndices = cluster->GetControlPointIndices();
			double *boneVertexWeights = cluster->GetControlPointWeights();

			// Iterate through all the vertices, which are affected by the bone
			int numBoneVertexIndices = cluster->GetControlPointIndicesCount();
			for (int boneVertexIndex = 0; boneVertexIndex < numBoneVertexIndices; boneVertexIndex++)
			{
				boneVertexIndex = boneVertexIndices[boneVertexIndex];
				float boneWeight = (float)boneVertexWeights[boneVertexIndex];
			}
		}
	}
}

void FileInfo::ExporterHeader::LoadNodeKeyframeAnimation(FbxMesh* pMesh)
{
	bool isAnimated = false;

	// Iterate all animations (for example, walking, running, falling and etc.)
	FbxScene* fbxScene = pMesh->GetScene();
	FbxAnimStack *animStack = (FbxAnimStack*)fbxScene->GetSrcObject();
	FbxAnimEvaluator *animEvaluator = fbxScene->GetAnimationEvaluator();
	animStack->GetName(); // Get the name of the animation if needed

	// Iterate all the transformation layers of the animation. You can have several layers, for example one for translation, one for rotation, one for scaling and each can have keys at different frame numbers.
	int numLayers = animStack->GetMemberCount();
	for (int layerIndex = 0; layerIndex < numLayers; layerIndex++)
	{
		FbxAnimLayer *animLayer = (FbxAnimLayer*)animStack->GetMember(layerIndex);
		animLayer->GetName(); // Get the layer's name if needed

		FbxAnimCurve *translationCurve = pMesh->GetNode()->LclTranslation.GetCurve(animLayer);
		FbxAnimCurve *rotationCurve = pMesh->GetNode()->LclRotation.GetCurve(animLayer);
		FbxAnimCurve *scalingCurve = pMesh->GetNode()->LclScaling.GetCurve(animLayer);

		if (scalingCurve != 0)
		{
			int numKeys = scalingCurve->KeyGetCount();
			for (int keyIndex = 0; keyIndex < numKeys; keyIndex++)
			{
				FbxTime frameTime = scalingCurve->KeyGetTime(keyIndex);
				FbxDouble3 scalingVector = pMesh->GetNode()->EvaluateLocalScaling(frameTime);
				float x = (float)scalingVector[0];
				float y = (float)scalingVector[1];
				float z = (float)scalingVector[2];

				float frameSeconds = (float)frameTime.GetSecondDouble(); // If needed, get the time of the scaling keyframe, in seconds
			}
		}
		else
		{
			// If this animation layer has no scaling curve, then use the default one, if needed
			FbxDouble3 scalingVector = pMesh->GetNode()->LclScaling.Get();
			float x = (float)scalingVector[0];
			float y = (float)scalingVector[1];
			float z = (float)scalingVector[2];
		}

		// Analogically, process rotationa and translation 
	}

}

bool FileInfo::ExporterHeader::FBXSave(char * fileName, std::vector<MyVertex>& pinVertexVector)
{
	std::ofstream save;
	save.open(fileName, std::ios_base::binary);
	if (save.is_open())
	{
		save.write((char*)&version, sizeof(version));
		save.write((char*)&updateTime, sizeof(version));
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

bool FileInfo::ExporterHeader::FBXRead(char * fileName, std::vector<MyVertex>& pinVertexVector)
{
	std::ifstream save;
	save.open(fileName, std::ios_base::binary);
	if (save.is_open())
	{
		save.read((char*)&version, sizeof(version));
		save.read((char*)&updateTime, sizeof(version));
		unsigned int size = pinVertexVector.size();
		save.read((char*)&size, sizeof(unsigned int));
		for (unsigned int i = 0; i < size; i++)
		{
			save.read((char*)&(pinVertexVector)[i].pos[0], sizeof((pinVertexVector)[i].pos[0]));
			save.read((char*)&(pinVertexVector)[i].pos[1], sizeof((pinVertexVector)[i].pos[1]));
			save.read((char*)&(pinVertexVector)[i].pos[2], sizeof((pinVertexVector)[i].pos[2]));
		}
		for (unsigned int i = 0; i < size; i++)
		{
			save.read((char*)&(pinVertexVector)[i].uv[0], sizeof((pinVertexVector)[i].uv[0]));
			save.read((char*)&(pinVertexVector)[i].uv[1], sizeof((pinVertexVector)[i].uv[1]));
		}
		for (unsigned int i = 0; i < size; i++)
		{
			save.read((char*)&(pinVertexVector)[i].normals[0], sizeof((pinVertexVector)[i].normals[0]));
			save.read((char*)&(pinVertexVector)[i].normals[1], sizeof((pinVertexVector)[i].normals[1]));
			save.read((char*)&(pinVertexVector)[i].normals[2], sizeof((pinVertexVector)[i].normals[2]));
		}
	}
	else
		return false;
	return true;
}
