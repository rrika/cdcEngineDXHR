#include <cmath>
#include <cstdio>
#include <memory>

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>

#include "spinnycube.h"
#include "types.h"
#include "matrix.h"
#include "input/PCMouseKeyboard.h"
#include "rendering/IPCDeviceManager.h"
#include "rendering/IRenderPassCallback.h"
#include "rendering/PCDX11ConstantBufferPool.h"
#include "rendering/PCDX11DepthBuffer.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/PCDX11IndexBuffer.h"
#include "rendering/PCDX11PixelShader.h"
#include "rendering/PCDX11RenderDevice.h"
#include "rendering/PCDX11RenderModel.h"
#include "rendering/PCDX11RenderTarget.h"
#include "rendering/PCDX11Scene.h"
#include "rendering/PCDX11SimpleStaticIndexBuffer.h"
#include "rendering/PCDX11SimpleStaticVertexBuffer.h"
#include "rendering/PCDX11StateManager.h"
#include "rendering/PCDX11StreamDecl.h"
#include "rendering/PCDX11Texture.h"
#include "rendering/PCDX11UberConstantBuffer.h"
#include "rendering/PCDX11VertexShader.h"
#include "rendering/RenderPasses.h"
#include "rendering/RenderModelInstance.h"
#include "rendering/VertexAttribute.h"
#include "drm/ResolveReceiver.h"
#include "drm/sections/RenderResourceSection.h"
#include "drm/sections/ShaderLibSection.h"

float VertexData[] = // float4 position, float3 normal, float2 texcoord, float3 color
{
    -1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  2.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  8.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 10.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  0.6f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  8.0f,  2.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  2.0f,  8.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  8.0f,  8.0f,  0.973f,  0.480f,  0.002f,
    -1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 10.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  2.0f, 10.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f,  8.0f, 10.0f,  0.973f,  0.480f,  0.002f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, -1.0f, 10.0f, 10.0f,  0.973f,  0.480f,  0.002f,
     1.0f,  1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  1.0f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  2.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  1.0f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  8.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 10.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  2.0f,  2.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  8.0f,  2.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  2.0f,  8.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  8.0f,  8.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 10.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -1.0f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  2.0f, 10.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -1.0f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  8.0f, 10.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 10.0f, 10.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 10.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,  2.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f,  2.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,  8.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f,  8.0f,  0.612f,  0.000f,  0.069f,
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 10.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f, 10.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f, 10.0f,  0.612f,  0.000f,  0.069f,
    -1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 10.0f, 10.0f,  0.612f,  0.000f,  0.069f,
    -1.0f,  1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  1.0f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  2.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  1.0f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  8.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 10.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  2.0f,  2.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  8.0f,  2.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  2.0f,  8.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  8.0f,  8.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 10.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -1.0f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  2.0f, 10.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -1.0f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  8.0f, 10.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 10.0f, 10.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  2.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 10.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f,  0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  2.0f,  2.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f,  0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f,  2.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  2.0f,  8.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f,  8.0f,  0.000f,  0.254f,  0.637f,
    -1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 10.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  2.0f, 10.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f, 10.0f,  0.000f,  0.254f,  0.637f,
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 10.0f, 10.0f,  0.000f,  0.254f,  0.637f,
    -1.0f, -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  2.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  8.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 10.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f, -0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  2.0f,  2.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f, -0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  8.0f,  2.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  2.0f,  8.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  8.0f,  8.0f,  0.001f,  0.447f,  0.067f,
    -1.0f, -1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 10.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  2.0f, 10.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  8.0f, 10.0f,  0.001f,  0.447f,  0.067f,
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 10.0f, 10.0f,  0.001f,  0.447f,  0.067f,
    -0.6f,  0.6f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  0.6f, -0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  0.6f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     1.0f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f, -0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f, -0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f,  0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  0.6f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  0.6f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -1.0f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f,  0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f, -0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  0.6f, -0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  1.0f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  0.6f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  0.6f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f, -0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -0.6f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -0.6f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f,  0.6f,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
};

unsigned short IndexData[] =
{
      0,   1,   9,   9,   8,   0,   1,   2,   5,   5,   4,   1,   6,   7,  10,  10,   9,   6,   2,   3,  11,  11,  10,   2,
     12,  13,  21,  21,  20,  12,  13,  14,  17,  17,  16,  13,  18,  19,  22,  22,  21,  18,  14,  15,  23,  23,  22,  14,
     24,  25,  33,  33,  32,  24,  25,  26,  29,  29,  28,  25,  30,  31,  34,  34,  33,  30,  26,  27,  35,  35,  34,  26,
     36,  37,  45,  45,  44,  36,  37,  38,  41,  41,  40,  37,  42,  43,  46,  46,  45,  42,  38,  39,  47,  47,  46,  38,
     48,  49,  57,  57,  56,  48,  49,  50,  53,  53,  52,  49,  54,  55,  58,  58,  57,  54,  50,  51,  59,  59,  58,  50,
     60,  61,  69,  69,  68,  60,  61,  62,  65,  65,  64,  61,  66,  67,  70,  70,  69,  66,  62,  63,  71,  71,  70,  62,
     72,  73,  74,  74,  75,  72,  76,  77,  78,  78,  79,  76,  80,  81,  82,  82,  83,  80,  84,  85,  86,  86,  87,  84,
     88,  89,  90,  90,  91,  88,  92,  93,  94,  94,  95,  92,  96,  97,  98,  98,  99,  96, 100, 101, 102, 102, 103, 100,
    104, 105, 106, 106, 107, 104, 108, 109, 110, 110, 111, 108, 112, 113, 114, 114, 115, 112, 116, 117, 118, 118, 119, 116,
    120, 121, 122, 122, 123, 120, 124, 125, 126, 126, 127, 124, 128, 129, 130, 130, 131, 128, 132, 133, 134, 134, 135, 132,
    136, 137, 138, 138, 139, 136, 140, 141, 142, 142, 143, 140, 144, 145, 146, 146, 147, 144, 148, 149, 150, 150, 151, 148,
    152, 153, 154, 154, 155, 152, 156, 157, 158, 158, 159, 156, 160, 161, 162, 162, 163, 160, 164, 165, 166, 166, 167, 164,
};

const char shaders [] = (
    "cbuffer WorldBuffer : register(b0) { float4x4 WorldViewProject; float4x4 World; float4x4 ViewProject; }\n"
    "cbuffer SceneBuffer : register(b2) { float4x4 View; }\n"
    "struct vs_in { float4 position : POSITION; float3 normal : NORMAL; float2 texcoord : TEXCOORD1; float3 color : COLOR; };\n"
    "struct vs_out { float4 position : SV_POSITION; float4 color : COLOR; float2 texcoord : TEXCOORD; };\n"
    "Texture2D    mytexture : register(t0);\n"
    "SamplerState mysampler : register(s0);\n"
    "static const float3 lightvector = { 1.0f, -1.0f, 1.0f };\n"
    "vs_out vs_main(vs_in input) {\n"
    "    float light = clamp(dot(normalize(mul(World, float4(input.normal, 0.0f)).xyz), normalize(-lightvector)), 0.0f, 1.0f) * 0.8f + 0.2f;\n"
    "    vs_out output;\n"
    "    output.position = mul(WorldViewProject, input.position);\n"
    "    output.texcoord = input.texcoord;\n"
    "    output.color = float4(input.color * light, 1.0f);\n"
    "    return output;\n"
    "}\n"
    "float4 ps_main(vs_out input) : SV_TARGET { return mytexture.Sample(mysampler, input.texcoord) * input.color; }\n"
);

struct float3 { float x, y, z; };

class SpinnyCubeDrawable : public cdc::IRenderDrawable {
public:
    cdc::PCDX11RenderDevice *renderDevice;
    cdc::PCDX11StateManager *stateManager;
    // cdc::PCDX11ConstantBuffer *cdcConstantBuffer;
    cdc::PCDX11VertexShader *cdcVertexShader;
    cdc::PCDX11PixelShader *cdcPixelShader;
    cdc::PCDX11IndexBuffer *cdcIndexBuffer;
    cdc::PCDX11StreamDecl *streamDecl;
    cdc::PCDX11VertexBuffer *cdcVertexBuffer;
    void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;
    uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other) override { /*TODO*/ return 0; };
};

class SpinnyCubePass : public cdc::IRenderPassCallback {
public:
    D3D11_VIEWPORT *viewport;
    ID3D11RasterizerState1* rasterizerState;
    ID3D11DepthStencilState* depthStencilState;

    bool pre(
        cdc::CommonRenderDevice *renderDevice,
        uint32_t passId,
        uint32_t drawableCount,
        uint32_t priorPassesBitfield) override;
    void post(
        cdc::CommonRenderDevice *renderDevice,
        uint32_t passId) override;
};

int spinnyCube(HWND window,
    ID3D11Device *baseDevice,
    ID3D11DeviceContext *baseDeviceContext) {

    std::unique_ptr<cdc::PCMouseKeyboard> mouseKeyboard(cdc::PCMouseKeyboard::create(window));
    auto renderDevice = static_cast<cdc::PCDX11RenderDevice*>(cdc::gRenderDevice);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3D11Device1* device;

    baseDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&device));

    ID3D11DeviceContext1* deviceContext;

    baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&deviceContext));

    ///////////////////////////////////////////////////////////////////////////////////////////////

    IDXGIDevice1* dxgiDevice;

    device->QueryInterface(__uuidof(IDXGIDevice1), reinterpret_cast<void**>(&dxgiDevice));

    IDXGIAdapter* dxgiAdapter;

    dxgiDevice->GetAdapter(&dxgiAdapter);

    IDXGIFactory2* dxgiFactory;

    dxgiAdapter->GetParent(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory));

    ///////////////////////////////////////////////////////////////////////////////////////////////

    DXGI_SWAP_CHAIN_DESC1 swapChainDesc;
    swapChainDesc.Width              = 0; // use window width
    swapChainDesc.Height             = 0; // use window height
    swapChainDesc.Format             = DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
    swapChainDesc.Stereo             = FALSE;
    swapChainDesc.SampleDesc.Count   = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount        = 1;
    swapChainDesc.Scaling            = DXGI_SCALING_STRETCH;
    swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.AlphaMode          = DXGI_ALPHA_MODE_UNSPECIFIED;
    swapChainDesc.Flags              = 0;

    IDXGISwapChain1* swapChain;

    dxgiFactory->CreateSwapChainForHwnd(device, window, &swapChainDesc, nullptr, nullptr, &swapChain);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3D11Texture2D* frameBuffer;

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&frameBuffer));

    ID3D11RenderTargetView* frameBufferView;

    device->CreateRenderTargetView(frameBuffer, nullptr, &frameBufferView);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_TEXTURE2D_DESC depthBufferDesc;

    frameBuffer->GetDesc(&depthBufferDesc); // base on framebuffer properties

    depthBufferDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    ID3D11Texture2D* depthBuffer;

    device->CreateTexture2D(&depthBufferDesc, nullptr, &depthBuffer);

    ID3D11DepthStencilView* depthBufferView;

    device->CreateDepthStencilView(depthBuffer, nullptr, &depthBufferView);

    cdc::PCDX11RenderTarget cdcRenderTarget;
    cdc::PCDX11DepthBuffer cdcDepthBuffer;

    cdcRenderTarget.renderTexture.view = static_cast<ID3D11View*>(frameBufferView);
    cdcDepthBuffer.renderTexture.view = static_cast<ID3D11View*>(depthBufferView);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3DBlob* vsBlob;

    D3DCompile(shaders, sizeof(shaders), "shaders.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vsBlob, nullptr);

    cdc::PCDX11VertexShader cdcOwnVertexShader(
        (char*)vsBlob->GetBufferPointer(),
        /*takeCopy=*/false,
        /*isWrapped=*/false);
    cdcOwnVertexShader.requestShader();
    cdcOwnVertexShader.awaitResource();

    cdc::PCDX11VertexShader& cdcScavengedVertexShader = *renderDevice->shtab_vs_wvp_1_0.vertexShaders[0];
    cdcScavengedVertexShader.requestShader();
    cdcScavengedVertexShader.awaitResource();

    cdc::PCDX11VertexShader& cdcVertexShader = false
        ? cdcScavengedVertexShader
        : cdcOwnVertexShader;

    uint32_t numAttr = 4;
    auto *layout = (cdc::VertexAttributeLayoutA*)new char[16 + 8 * numAttr];
    layout->numAttr = numAttr;
    layout->attrib[0] = {0xD2F7D823,  0, 3}; // position,  offset  0, DXGI_FORMAT_R32G32B32A32_FLOAT
    layout->attrib[1] = {0x36F5E414, 16, 2}; // normal,    offset 16, DXGI_FORMAT_R32G32B32_FLOAT
    layout->attrib[2] = {0x8317902A, 28, 1}; // texcoord1, offset 28, DXGI_FORMAT_R32G32_FLOAT
    layout->attrib[3] = {0XFFFFFFFF, 36, 2}; // color,     offset 36, DXGI_FORMAT_R32G32B32_FLOAT

    auto *inputElementDesc = new D3D11_INPUT_ELEMENT_DESC[layout->numAttr];
    memset(inputElementDesc, 0, sizeof(D3D11_INPUT_ELEMENT_DESC[layout->numAttr]));
    decodeVertexAttribA(inputElementDesc, layout->attrib, layout->numAttr, cdcVertexShader.wineWorkaround);
    delete[] (char*)layout;

    cdc::PCDX11StreamDecl streamDecl(
        static_cast<cdc::PCDX11RenderDevice*>(cdc::gRenderDevice),
        inputElementDesc, numAttr, &cdcVertexShader.m_sub);
    streamDecl.internalCreate();

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3DBlob* psBlob;

    D3DCompile(shaders, sizeof(shaders), "shaders.hlsl", nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &psBlob, nullptr);

    cdc::PCDX11PixelShader cdcPixelShader(
        (char*)psBlob->GetBufferPointer(),
        /*takeCopy=*/false,
        /*isWrapped=*/false);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_RASTERIZER_DESC1 rasterizerDesc = {};
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_BACK;

    ID3D11RasterizerState1* rasterizerState;

    device->CreateRasterizerState1(&rasterizerDesc, &rasterizerState);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
    depthStencilDesc.DepthEnable    = TRUE;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc      = D3D11_COMPARISON_LESS;

    ID3D11DepthStencilState* depthStencilState;

    device->CreateDepthStencilState(&depthStencilDesc, &depthStencilState);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    // struct Constants
    // {
    //     float4x4 WorldViewProject;
    //     float4x4 World;
    //     float4x4 ViewProject;
    // };

    // cdc::PCDX11ConstantBufferPool cbPool{renderDevice};
    // cdc::PCDX11UberConstantBuffer cdcConstantBuffer((sizeof(Constants) + 15) / 16);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    UINT stride = 12 * 4; // vertex size (11 floats: float4 position, float3 normal, float2 texcoord, float3 color)
    cdc::PCDX11SimpleStaticVertexBuffer cdcVertexBuffer(stride, sizeof(VertexData) / stride, (void*)VertexData);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    cdc::PCDX11StateManager stateManager(deviceContext, device);
    cdc::PCDX11SimpleStaticIndexBuffer cdcIndexBuffer(sizeof(IndexData)/2, IndexData);
    cdc::deviceManager->stateManager = &stateManager; // hack

    cdc::RenderResourceSection renderResourceSection;
    cdc::ShaderLibSection shaderLibSection;

    cdc::ResolveSection *resolveSections[16] = {nullptr};
    resolveSections[5] = &renderResourceSection; // textures
    resolveSections[9] = &shaderLibSection;
    resolveSections[12] = &renderResourceSection; // meshes

    hackResolveReceiver("pickup_dns_156600946691c80e_dx11.drm", resolveSections);
    hackResolveReceiver("alc_beer_bottle_a.drm", resolveSections);

    auto bottleTexture = (cdc::PCDX11Texture*)renderResourceSection.getWrapped(0x0396);
    printf("have bottle cdc texture: %p\n", bottleTexture);
    bottleTexture->asyncCreate();
    printf("have bottle d3d texture: %p\n", bottleTexture->d3dTexture128);

    stateManager.setTextureAndSampler(0, bottleTexture, 0, 0.0f);

    auto bottleRenderModel = (cdc::PCDX11RenderModel*)renderResourceSection.getWrapped(0xA301);
    printf("have bottle cdc render model: %p\n", bottleRenderModel);
    printf("have bottle cdc mesh blob: %p\n", bottleRenderModel->getMesh());

    cdc::RenderModelInstance *bottleRenderModelInstance =
        renderDevice->createRenderModelInstance(bottleRenderModel);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(depthBufferDesc.Width), static_cast<float>(depthBufferDesc.Height), 0.0f, 1.0f };

    ///////////////////////////////////////////////////////////////////////////////////////////////

    float w = viewport.Width / viewport.Height; // width (aspect ratio)
    float h = 1.0f;                             // height
    float n = 1.0f;                             // near
    float f = 9.0f;                             // far

    float scale = 0.1f;
    float3 modelRotation    = { 0.0f, 0.0f, 0.0f };
    float3 modelScale       = { scale, scale, scale };
    float3 modelTranslation = { 0.0f, 0.0f, 4.0f };

    ///////////////////////////////////////////////////////////////////////////////////////////////

    cdc::CommonSceneSub10 commonSceneSub10;
    commonSceneSub10.mask = 1;

    SpinnyCubePass cubePass;
    cubePass.viewport = &viewport;
    cubePass.rasterizerState = rasterizerState;
    cubePass.depthStencilState = depthStencilState;
    renderDevice->renderPasses.addRenderPass(0, 0, 0, 0, 0);
    renderDevice->setPassCallback(0, &cubePass);

    SpinnyCubeDrawable cubeDrawable;
    cubeDrawable.renderDevice = renderDevice;
    cubeDrawable.stateManager = &stateManager;
    // cubeDrawable.cdcConstantBuffer = &cdcConstantBuffer;
    cubeDrawable.cdcVertexShader = &cdcVertexShader;
    cubeDrawable.cdcPixelShader = &cdcPixelShader;
    cubeDrawable.cdcIndexBuffer = &cdcIndexBuffer;
    cubeDrawable.streamDecl = &streamDecl;
    cubeDrawable.cdcVertexBuffer = &cdcVertexBuffer;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    while (true)
    {
        MSG msg;

        while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            if (msg.message == WM_DESTROY) {
                PostQuitMessage(0);
                return 0;
            }
            if (msg.message == WM_QUIT) {
                return 0;
            }

            mouseKeyboard->processWndProc(msg.message, msg.wParam, msg.lParam);

            //if (msg.message == WM_KEYDOWN) return 0;
            DispatchMessageA(&msg);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////

        float4x4 rotateX   = { 1, 0, 0, 0, 0, static_cast<float>(cos(modelRotation.x)), -static_cast<float>(sin(modelRotation.x)), 0, 0, static_cast<float>(sin(modelRotation.x)), static_cast<float>(cos(modelRotation.x)), 0, 0, 0, 0, 1 };
        float4x4 rotateY   = { static_cast<float>(cos(modelRotation.y)), 0, static_cast<float>(sin(modelRotation.y)), 0, 0, 1, 0, 0, -static_cast<float>(sin(modelRotation.y)), 0, static_cast<float>(cos(modelRotation.y)), 0, 0, 0, 0, 1 };
        float4x4 rotateZ   = { static_cast<float>(cos(modelRotation.z)), -static_cast<float>(sin(modelRotation.z)), 0, 0, static_cast<float>(sin(modelRotation.z)), static_cast<float>(cos(modelRotation.z)), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        float4x4 scale     = { modelScale.x, 0, 0, 0, 0, modelScale.y, 0, 0, 0, 0, modelScale.z, 0, 0, 0, 0, 1 };
        float4x4 translate = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, modelTranslation.x, modelTranslation.y, modelTranslation.z, 1 };

        modelRotation.x += 0.005f;
        modelRotation.y += 0.009f;
        modelRotation.z += 0.001f;

        ///////////////////////////////////////////////////////////////////////////////////////////

        float4x4 world = scale * rotateZ * rotateY * rotateX;
        float4x4 project = { 2 * n / w, 0, 0, 0, 0, 2 * n / h, 0, 0, 0, 0, f / (f - n), 1, 0, 0, n * f / (n - f), 0 };

        // Constants constants;
        // constants.WorldViewProject = project * world;
        // constants.World = world;
        // constants.ViewProject = project;

        stateManager.setWorldMatrix(world);

        // memcpy(cdcConstantBuffer.data, &constants, sizeof(Constants));
        // cdcConstantBuffer.syncBuffer(deviceContext);

        renderDevice->resetRenderLists();
        renderDevice->beginRenderList();
        auto *scene = renderDevice->createSubScene(
            &commonSceneSub10,
            &cdcRenderTarget,
            &cdcDepthBuffer);
        scene->viewMatrix = translate;
        scene->projectMatrix = project;

        // add drawables to the scene
        float backgroundColor[4] = {0.025f, 0.025f, 0.025f, 1.0f};
        renderDevice->clearRenderTarget(10, /*mask=*/ 1, 0.0f, backgroundColor, 1.0f, 0);
        renderDevice->recordDrawable(&cubeDrawable, /*mask=*/ 1, /*addToParent=*/ 0);
        bottleRenderModelInstance->recordDrawables();

        renderDevice->finishScene();
        renderDevice->endRenderList();

        renderDevice->drawRenderLists();

        ///////////////////////////////////////////////////////////////////////////////////////////

        swapChain->Present(1, 0);
    }
}

bool SpinnyCubePass::pre(
    cdc::CommonRenderDevice *renderDevice,
    uint32_t passId,
    uint32_t drawableCount,
    uint32_t priorPassesBitfield)
{
    auto *deviceContext = static_cast<cdc::PCDX11RenderDevice*>(cdc::gRenderDevice)->getD3DDeviceContext();

    deviceContext->RSSetViewports(1, viewport);
    deviceContext->RSSetState(rasterizerState);

    deviceContext->OMSetDepthStencilState(depthStencilState, 0);
    deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // use default blend mode (i.e. disable)

    return true;
}

void SpinnyCubePass::post(
    cdc::CommonRenderDevice *renderDevice,
    uint32_t passId)
{
    // empty
}

void SpinnyCubeDrawable::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
    stateManager->updateMatrices();
    stateManager->updateConstantBuffers();

    stateManager->setVertexShader(cdcVertexShader);
    // stateManager->setVsConstantBuffer(0, cdcConstantBuffer);
    stateManager->setCommonConstantBuffers();
    stateManager->setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    stateManager->setStreamDecl(streamDecl);
    stateManager->setVertexBuffer(cdcVertexBuffer);
    stateManager->setIndexBuffer(cdcIndexBuffer);

    if (false) {
        auto errorTable = static_cast<cdc::PCDX11PixelShaderTable*>(renderDevice->shlib_1->table);
        stateManager->setPixelShader(errorTable->pixelShaders[0]);
    } else {
        stateManager->setPixelShader(cdcPixelShader);
    }

    stateManager->updateShaderResources();
    stateManager->updateSamplers();

    renderDevice->getD3DDeviceContext()->DrawIndexed(ARRAYSIZE(IndexData), 0, 0);
}
