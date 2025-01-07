cbuffer WorldBuffer : register(b0) {
	float4x4 WorldViewProject; 
	float4x4 World; 
	float4x4 ViewProject; 
};

cbuffer StreamDeclBuffer : register(b3) {
	float4 NormalScaleOffset;
	float4 TexcoordScales;
};

cbuffer InstanceBuffer : register(b5) {
	float4 InstanceParams[8];
};

struct VS_INPUT {
	float4 col : COLOR;
	float4 uv  : TEXCOORD;
	float3 pos : POSITION;
};

struct PS_INPUT {
	float4 pos : SV_POSITION;
	float4 col : COLOR;
	float2 uv  : TEXCOORD;
};

PS_INPUT main(VS_INPUT input) {
	PS_INPUT output;
	output.pos = mul(WorldViewProject, float4(input.pos, 1.f));
	output.col =
		input.col.zyxw * InstanceParams[0].x +
		input.col.xyzw * InstanceParams[0].y;
	output.col = clamp(output.col, 0.f, 1.f);
	output.uv  = input.uv.xy * TexcoordScales.x;
	return output;
}
