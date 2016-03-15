Shader "Custom/SteamVR_ClearAll" {
	Properties { _MainTex ("Base (RGB)", 2D) = "white" {} }

	CGINCLUDE

	#include "UnityCG.cginc"

	sampler2D _MainTex;

	struct v2f {
		float4 pos : SV_POSITION;
		float2 tex : TEXCOORD0;
	};

	v2f vert(appdata_base v) {
		v2f o;
		o.pos = mul(UNITY_MATRIX_MVP, v.vertex);
		o.tex = v.texcoord;
		return o;
	}

	float4 frag(v2f i) : COLOR {
		return float4(0, 0, 0, 0);
	}

	ENDCG

	SubShader {
		Tags{ "Queue" = "Background" }
		Pass {
			ZTest Always Cull Off ZWrite On
			Fog { Mode Off }

			CGPROGRAM
			#pragma vertex vert
			#pragma fragment frag
			ENDCG
		}
	}
}

