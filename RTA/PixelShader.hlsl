#include "ConstantBuffers.h"

struct VertexOut
{
	float4 position : SV_POSITION;
	float2 UV : UV0;
	float3 Normal : NORMAL0;
};

texture2D baseTexture : register(t0);

SamplerState filters[2] : register(s0);

float4 main(VertexOut input) : SV_TARGET
{
	return baseTexture.Sample(filters[0], input.UV);
}