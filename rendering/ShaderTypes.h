using mat4 = float[16];
using mat4x3 = float[12];
using mat4x2 = float[8];
using vec2 = float[2];
using vec3 = float[3];
using vec4 = float[2];

struct DrawableBufferPS {
	vec4 FogColor;
	vec4 DebugColor;
	float MaterialOpacity;
	float AlphaThreshold;
};

struct ShadowLights {
	// ?
};

struct NonShadowLights {
	// ?
};

struct SunLight {
	vec4 position;							// 144 36
	vec3 direction;							// 148 37
	vec4 attParams;							// 152 38
	vec4 spotParams;						// 156 39
	vec4 diffuseColor;						// 160 40
	vec4 shadowFadeParams;					// 164 41
};

struct LightBufferPS {			// my own offset, metadata reference, /4, /16
	ShadowLights ShadowLights;				// 0    0     0    0
	NonShadowLights NonShadowLights;		// 24   288   72   18
	SunLight SunLight;						// 48   576   144  36
	mat4 WorldToShadowMap[3];				// 72   672   168  42
	mat4x3 WorldToModulationMapSpot[2];		// 120  864   216  54
	mat4x2 WorldToModulationMapSun;			// 152  960   240  60
	int ShadowLightEnabled[3];				// 168  992   248  62
	int NonShadowLightEnabled[3];			// 180  1040  260  65
	int SunLightEnabled;					// 192 or 189  1088  272  68
	// 1104  276  69
};

struct MaterialBufferPS {
	vec4 MaterialParams[32];
};

struct SceneBufferVS {
	mat4 View;								// 0
	mat4 ScreenMatrix;						// 4
	vec2 DepthExportScale;					// 8
	vec2 FogScaleOffset;					// 9
	vec3 CameraPosition;					// 10
	vec3 CameraDirection;					// 11
	vec3 DepthFactors;						// 12
	vec2 ShadowDepthBias;					// 13
	vec4 SubframeViewport;					// 14
	mat4x3 DepthToWorld;					// 15
	vec4 DepthToView;						// 18
	vec4 OneOverDepthToView;				// 19
	vec4 DepthToW;							// 20
	vec4 ClipPlane;							// 21
	vec2 ViewportDepthScaleOffset;			// 22
	vec2 ColorDOFDepthScaleOffset;			// 23
	vec2 TimeVector;						// 24
	vec3 HeightFogParams;					// 25
	vec3 GlobalAmbient;						// 26
	vec4 GlobalParams[16];					// 27
	float DX3_SSAOScale;					// 43
	vec4 ScreenExtents;						// 44
	vec2 ScreenResolution;					// 45
	vec4 PSSMToMap1Lin;						// 46
	vec4 PSSMToMap1Const;					// 47
	vec4 PSSMToMap2Lin;						// 48
	vec4 PSSMToMap2Const;					// 49
	vec4 PSSMToMap3Lin;						// 50
	vec4 PSSMToMap3Const;					// 51
	vec4 PSSMDistances;						// 52
	mat4 WorldToPSSM0;						// 53
};

struct SceneBufferPS : SceneBufferVs {
	float StereoOffset;
};

struct SkinningBufferVS {
	mat4 SkinMatrices[42];
};

struct StreamDeclBufferVS {
	vec4 NormalScaleOffset;
	vec4 TexcoordScales;
};

struct WorldBufferVS {
	mat4 WorldViewProject;
	mat4 World;
	mat4 ViewProject;
};
