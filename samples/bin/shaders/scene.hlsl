
// Vertex Shader
struct VS_INPUT
{
	float3 vPosition : POSITION;
	float2 vUVCoords: TEXCOORD0;
};

struct PS_INPUT
{
	float4 vPosition : SV_POSITION;
	float2 vUVCoords : TEXCOORD0;
};

cbuffer SceneConstantBuffer : register(b0)
{
	float4x4 g_MVPMatrix;
};

SamplerState g_SamplerState : register(s0);
Texture2D g_Texture : register(t0);


PS_INPUT VSMain( VS_INPUT i )
{
	PS_INPUT o;
	o.vPosition = mul( g_MVPMatrix, float4( i.vPosition, 1.0 ) );
#ifdef VULKAN
	o.vPosition.y = -o.vPosition.y;
#endif
	o.vUVCoords = i.vUVCoords;
	return o;
}

float4 PSMain( PS_INPUT i ) : SV_TARGET
{
	float4 vColor = g_Texture.Sample( g_SamplerState, i.vUVCoords );
	return vColor;
}
