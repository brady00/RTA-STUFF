#include "skybox.h"
#include "RenderMaterial.h"
#include "RenderShape.h"
#include "FBXLoader.h"

skybox::~skybox()
{
	delete renderSet;
}

bool skybox::Create()
{
	renderSet = new RenderSet;
	RenderContext* renderContext = new RenderContext;
	std::vector<PosUv> verticies;
	verticies = CreateCube(renderContext);
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	renderContext->setVertexShader("Skybox_VS.cso");
	renderContext->setPixelShader("Skybox_PS.cso");
	renderContext->setInputLayout(layout, 2);
	renderContext->RenderFunc = renderContext->RenderFunction;
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.FrontCounterClockwise = false;
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_FRONT;
	desc.DepthClipEnable = false;
	renderContext->setRasterizerState(desc);
	renderContext->CreateRenderMaterials();
	RenderMaterial* renderMaterial = new RenderMaterial;
	renderMaterial->CreateTexture(L"SkyBox.dds");
	renderMaterial->RenderFunc = renderMaterial->RenderFunction;
	renderMaterial->CreateRenderShapes();
	renderContext->AddRenderMaterials((RenderNode*)renderMaterial);
	RenderShape* renderShape = new RenderShape;
	renderShape->SetWorldMatrix(DirectX::XMFLOAT4X4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));
	renderShape->RenderFunc = renderShape->RenderFunction;
	renderShape->setNumPrimitives(verticies.size());
	renderShape->setPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	renderShape->setstartIndex(0);
	renderShape->setStartVertex(0);
	DirectX::XMFLOAT4X4 matrix;
	DirectX::XMStoreFloat4x4(&matrix, DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(300), 1, 0.1f, 1000.0f));
	renderShape->SetViewProjMatrix(matrix);
	renderMaterial->AddRenderShapes((RenderNode*)renderShape);

	renderSet->AddRenderNode((RenderNode*)renderContext);
	return true;
}

bool skybox::Render()
{
	Renderer::Render(renderSet);
	Renderer::devicecontext->ClearDepthStencilView(Renderer::DepthStencilView, D3D11_CLEAR_DEPTH, 1, 0);
	return true;
}

std::vector<PosUv> skybox::CreateCube(RenderContext* renderContext)
{
	FileInfo::ExporterHeader file;
	//std::vector<MyVertex> verts;
	std::vector<PosUv> verticies;
	/*if (file.FBXLoad("skybox.fbx", &verts, "skybox.bin"))
		file.FBXSave("skybox.bin", verts);
	file.FBXRead("skybox.bin", verts);
	for (unsigned int i = 0; i < verts.size(); i++)
	{
		PosUv temp;
		temp.pos[0] = verts[i].pos[0];
		temp.pos[1] = verts[i].pos[1];
		temp.pos[2] = verts[i].pos[2];
		temp.uv[0] = verts[i].uv[0];
		temp.uv[1] = verts[i].uv[1];
		verticies.push_back(temp);
	}*/
	PosUv verts[36];
	verts[0].pos[0] = -1; verts[1].pos[0] = -1; verts[2].pos[0] = 1; verts[3].pos[0] = 1; verts[4].pos[0] = -1; verts[5].pos[0] = 1;
	verts[0].pos[1] = 1; verts[1].pos[1] = -1; verts[2].pos[1] = 1; verts[3].pos[1] = 1; verts[4].pos[1] = -1; verts[5].pos[1] = -1;
	verts[0].pos[2] = 1; verts[1].pos[2] = 1; verts[2].pos[2] = 1; verts[3].pos[2] = 1; verts[4].pos[2] = 1; verts[5].pos[2] = 1;
	verts[0].uv[0]  = 1; verts[1].uv[0]  = 1; verts[2].uv[0]  = 0; verts[3].uv[0]  = 0; verts[4].uv[0]  = 1; verts[5].uv[0]  = 0;
	verts[0].uv[1]  = 0; verts[1].uv[1]  = 1; verts[2].uv[1]  = 0; verts[3].uv[1]  = 0; verts[4].uv[1]  = 1; verts[5].uv[1]  = 1;
	
	verts[6].pos[0] = 1; verts[7].pos[0] = 1; verts[8].pos[0] = -1; verts[9].pos[0] = -1; verts[10].pos[0] = 1; verts[11].pos[0] = -1;
	verts[6].pos[1] = 1; verts[7].pos[1] = -1; verts[8].pos[1] = 1; verts[9].pos[1] = 1; verts[10].pos[1] = -1; verts[11].pos[1] = -1;
	verts[6].pos[2] = -1; verts[7].pos[2] = -1; verts[8].pos[2] = -1; verts[9].pos[2] = -1; verts[10].pos[2] = -1; verts[11].pos[2] = -1;
	verts[6].uv[0]  = 1; verts[7].uv[0]  = 1; verts[8].uv[0]  = 0; verts[9].uv[0]  = 0; verts[10].uv[0]  = 1; verts[11].uv[0]  = 0;
	verts[6].uv[1]  = 0; verts[7].uv[1]  = 1; verts[8].uv[1]  = 0; verts[9].uv[1]  = 0; verts[10].uv[1]  = 1; verts[11].uv[1]  = 1;

	verts[12].pos[0] = 1; verts[13].pos[0] = 1; verts[14].pos[0] = 1; verts[15].pos[0] = 1; verts[16].pos[0] = 1; verts[17].pos[0] = 1;
	verts[12].pos[1] = 1; verts[13].pos[1] = -1; verts[14].pos[1] = 1; verts[15].pos[1] = 1; verts[16].pos[1] = -1; verts[17].pos[1] = -1;
	verts[12].pos[2] = 1; verts[13].pos[2] = 1; verts[14].pos[2] = -1; verts[15].pos[2] = -1; verts[16].pos[2] = 1; verts[17].pos[2] = -1;
	verts[12].uv[0]  = 1; verts[13].uv[0]  = 1; verts[14].uv[0]  = 0; verts[15].uv[0]  = 0; verts[16].uv[0]  = 1; verts[17].uv[0]  = 0;
	verts[12].uv[1]  = 0; verts[13].uv[1]  = 1; verts[14].uv[1]  = 0; verts[15].uv[1]  = 0; verts[16].uv[1]  = 1; verts[17].uv[1]  = 1;

	verts[18].pos[0] = -1; verts[19].pos[0] = -1; verts[20].pos[0] = -1; verts[21].pos[0] = -1; verts[22].pos[0] = -1; verts[23].pos[0] = -1;
	verts[18].pos[1] = 1; verts[19].pos[1] = -1; verts[20].pos[1] = 1; verts[21].pos[1] = 1; verts[22].pos[1] = -1; verts[23].pos[1] = -1;
	verts[18].pos[2] = -1; verts[19].pos[2] = -1; verts[20].pos[2] = 1; verts[21].pos[2] = 1; verts[22].pos[2] = -1; verts[23].pos[2] = 1;
	verts[18].uv[0] = 1; verts[19].uv[0] = 1; verts[20].uv[0] = 0; verts[21].uv[0] = 0; verts[22].uv[0] = 1; verts[23].uv[0] = 0;
	verts[18].uv[1] = 0; verts[19].uv[1] = 1; verts[20].uv[1] = 0; verts[21].uv[1] = 0; verts[22].uv[1] = 1; verts[23].uv[1] = 1;

	verts[24].pos[0] = 1; verts[25].pos[0]= 1;verts[26].pos[0] = -1;verts[27].pos[0] = -1;verts[28].pos[0] = 1; verts[29].pos[0] = -1;
	verts[24].pos[1] = 1; verts[25].pos[1]= 1; verts[26].pos[1] = 1; verts[27].pos[1] = 1; verts[28].pos[1] = 1; verts[29].pos[1] = 1;
	verts[24].pos[2] = 1; verts[25].pos[2]= -1;verts[26].pos[2] = 1; verts[27].pos[2] = 1; verts[28].pos[2] = -1; verts[29].pos[2] = -1;
	verts[24].uv[0] = 1; verts[25].uv[0] = 1; verts[26].uv[0] = 0; verts[27].uv[0] = 0; verts[28].uv[0] = 1; verts[29].uv[0] = 0;
	verts[24].uv[1] = 0; verts[25].uv[1] = 1; verts[26].uv[1] = 0; verts[27].uv[1] = 0; verts[28].uv[1] = 1; verts[29].uv[1] = 1;

	verts[30].pos[0] = 1; verts[31].pos[0]= 1;verts[32].pos[0] = -1;verts[33].pos[0] = -1;verts[34].pos[0] = 1; verts[35].pos[0] = -1;
	verts[30].pos[1] = -1; verts[31].pos[1]= -1; verts[32].pos[1] = -1; verts[33].pos[1] = -1; verts[34].pos[1] = -1; verts[35].pos[1] = -1;
	verts[30].pos[2] = -1; verts[31].pos[2]= 1;verts[32].pos[2] = -1; verts[33].pos[2] = -1; verts[34].pos[2] = 1; verts[35].pos[2] = -1;
	verts[30].uv[0] = 1; verts[31].uv[0] = 1; verts[32].uv[0] = 0; verts[33].uv[0] = 0; verts[34].uv[0] = 1; verts[35].uv[0] = 0;
	verts[30].uv[1] = 0; verts[31].uv[1] = 1; verts[32].uv[1] = 0; verts[33].uv[1] = 0; verts[34].uv[1] = 1; verts[35].uv[1] = 1;
	for (unsigned int i = 0; i < 36; i++)
		verticies.push_back(verts[i]);
	
	
	D3D11_BUFFER_DESC VertDesc;
	ZeroMemory(&VertDesc, sizeof(VertDesc));
	VertDesc.Usage = D3D11_USAGE_IMMUTABLE;
	VertDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	VertDesc.ByteWidth = sizeof(PosUv) * verticies.size();
	VertDesc.StructureByteStride = sizeof(PosUv);
	D3D11_SUBRESOURCE_DATA VertData;
	ZeroMemory(&VertData, sizeof(VertData));
	VertData.pSysMem = &verticies[0];
	renderContext->setVertexBuffer(VertDesc, VertData, sizeof(PosUv), 0);
	return verticies;
}