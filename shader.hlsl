struct VSInput
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct PSInput
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

PSInput VS(VSInput input)
{
	PSInput output;
	
	output.Position = input.Position;
	output.Color = input.Color;

	return output;
}

float4 PS(PSInput input) : SV_TARGET
{
	return input.Color;
}