#include "ConstantBuffers.h"

struct VertexIn
{
	float3 position : POSITION0;
};

struct VertexOut
{
	float4 position : POSITION0;
};

VertexOut main( VertexIn input) : SV_POSITION
{
	VertexOut output;

	output.position = mul(float4(input.position, 1.0), gMVP);

	return output;
}