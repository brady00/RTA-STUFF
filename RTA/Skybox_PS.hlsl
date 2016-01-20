struct VertexOut
{
	float4 position : SV_POSITION;
	float3 UV : UV0;
};

TextureCube baseTexture : register(t0);

SamplerState filter : register(s0);

float4 main(VertexOut input) : SV_TARGET
{
	return baseTexture.Sample(filter, input.UV);
}