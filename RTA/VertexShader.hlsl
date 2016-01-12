#include "ConstantBuffers.h"

struct VertexIn
{
	float3 position : POSITION0;
	float2 UV : TEXCOORD0;
	float3 Normal : NORMAL0;
};

struct VertexOut
{
	float4 position : SV_POSITION;
	float2 UV : UV0;
	float3 Normal : NORMAL0;
};

VertexOut main( VertexIn input)
{
	VertexOut output;

	output.position = mul(float4(input.position, 1.0), gMVP);
	output.UV = input.UV;
	output.Normal = input.Normal;

	return output;
}