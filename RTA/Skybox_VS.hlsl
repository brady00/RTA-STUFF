#include "ConstantBuffers.h"

struct VertexIn
{
	float3 position : POSITION0;
	float2 UV : TEXCOORD0;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float3 UV : UV0;
};

VertexOut main(VertexIn input)
{
	VertexOut output;

	//float4x4 view = mul(gView, float4x4(100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100));
	output.position = mul(float4(input.position, 1.0), gWorld);
	output.position = mul(output.position, gView);
	output.position = mul(output.position, gProj);
	output.UV = input.position * 2;

	return output;
}