struct VSInput
{
	float4 Position : POSITION;
	float2 UV : TEXCOORD;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float2 UV : TEXCOORD;
};

Texture2D g_texture : register(t0);
SamplerState g_sampler : register(s0);

PSInput VSMain(VSInput input)
{
	PSInput output;
	
	output.Position = input.Position;
	output.UV = input.UV;

	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	//return input.Color;
	return g_texture.Sample(g_sampler, input.UV);
}