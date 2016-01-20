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

	output.position = mul(float4(input.position, 1.0), gMVP);
	output.UV = input.position * 2;

	return output;
}