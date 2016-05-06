Shader "Custom/SteamVR_Fade" {
SubShader { Pass {
	Blend SrcAlpha OneMinusSrcAlpha
	ZTest Always Cull Off ZWrite Off Fog { Mode Off }
	BindChannels { Bind "vertex", vertex Bind "color", color }
} } }