Texture2D shaderTexture;
SamplerState SampleType;

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
	float2 texCoord : TEXCOORD0;
};

float random( float2 p )
{
  // We need irrationals for pseudo randomness.
  // Most (all?) known transcendental numbers will (generally) work.
  const float2 r = float2(
    23.1406926327792690,  // e^pi (Gelfond's constant)
     2.6651441426902251); // 2^sqrt(2) (Gelfond–Schneider constant)
  return frac( cos( fmod( 123456789., 1e-7 + 256. * dot(p,r) ) ) );  
}


float4 noiser(float2 pos : POSITION) : COLOR
{
	float n =  random( pos*100 );
	n = clamp( n, 0.5f, 1.0f );
    return float4( n, n, n, 1.0f );
}

float4 main(PixelShaderInput input) : SV_TARGET
{
//	return shaderTexture.Sample(SampleType, input.texCoord);
	return float4(input.color,1.0f)*shaderTexture.Sample(SampleType, input.texCoord);
//	return float4(input.color,1.0f)*noiser( input.texCoord );
}
