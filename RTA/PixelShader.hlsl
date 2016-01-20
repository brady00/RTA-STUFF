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
	float4 color = baseTexture.Sample(filters[0], input.UV);
	float4 NewColor = color;
	float3 lightdir = -normalize(float3(-1, -1, 0));
	float3 normal = normalize(input.Normal);
	float4 directionlight = clamp((dot(lightdir, normal) * NewColor) + (NewColor + color), 0, 1);
	return directionlight;
}