
// Vertex Shader
struct VS_INPUT
{
	float3 vPosition : POSITION;
	float3 vColor: COLOR0;
};

struct PS_INPUT
{
	float4 vPosition : SV_POSITION;
	float4 vColor : TEXCOORD0;
};

cbuffer SceneConstantBuffer : register(b0)
{
	float4x4 g_MVPMatrix;
};

PS_INPUT VSMain( VS_INPUT i )
{
	PS_INPUT o;
	o.vPosition = mul( g_MVPMatrix, float4( i.vPosition, 1.0 ) );
#ifdef VULKAN
	o.vPosition.y = -o.vPosition.y;
#endif
	o.vColor = float4( i.vColor.rgb, 1.0 );
	return o;
}

float4 PSMain( PS_INPUT i ) : SV_TARGET
{
	return i.vColor;
}
