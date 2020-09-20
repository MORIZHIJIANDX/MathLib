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

cbuffer FrameBuffer : register(b0)
{
	float TotalTime;
	float2 Speed;
};


PSInput VSMain(VSInput input)
{
	PSInput output;
	
	output.Position = input.Position;

	output.Position.xy += sin(TotalTime) * Speed;

	output.UV = input.UV;

	return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	//return input.Color;
	return g_texture.Sample(g_sampler, input.UV);

	//float x = TotalTime;

	//return float4(x, x, x, 1.0f);
}