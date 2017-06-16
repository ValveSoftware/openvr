
// Vertex Shader
struct VS_INPUT
{
	float2 vPosition : POSITION;
	float2 vUVCoords: TEXCOORD0;
};

struct PS_INPUT
{
	float4 vPosition : SV_POSITION;
	float2 vUVCoords : TEXCOORD0;
};

Texture2DMS<float4> g_Texture : register(t0);


PS_INPUT VSMain( VS_INPUT i )
{
	PS_INPUT o;
	o.vPosition = float4( i.vPosition, 0.0, 1.0 );
#ifdef VULKAN
	o.vPosition.y = -o.vPosition.y;
#endif
	o.vUVCoords = i.vUVCoords;
	return o;
}

float4 PSMain( PS_INPUT i ) : SV_TARGET
{
	// Determine texture dimensions
	float nTextureWidth;
	float nTextureHeight;
	float nNumSamples;
	g_Texture.GetDimensions( nTextureWidth, nTextureHeight, nNumSamples );

	// Fetch sample 0
	float4 vColor = g_Texture.Load( i.vUVCoords * float2( nTextureWidth, nTextureHeight ), 0 );
	return vColor;
}
