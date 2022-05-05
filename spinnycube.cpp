#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <math.h>

#include <stdio.h>

#include "spinnycube.h"
#include "types.h"
#include "rendering/IPCDeviceManager.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/PCDX11IndexBuffer.h"
#include "rendering/PCDX11PixelShader.h"
#include "rendering/PCDX11RenderDevice.h"
#include "rendering/PCDX11StateManager.h"
#include "rendering/PCDX11StaticVertexBuffer.h"
#include "rendering/PCDX11StreamDecl.h"
#include "rendering/PCDX11Texture.h"
#include "rendering/PCDX11VertexShader.h"
#include "drm/ResolveReceiver.h"
#include "drm/sections/RenderResourceSection.h"

float VertexData[] = // float3 position, float3 normal, float2 texcoord, float3 color
{
    -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  2.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  8.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 10.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  0.6f, -1.0f,  0.0f,  0.0f, -1.0f,  2.0f,  2.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -1.0f,  0.0f,  0.0f, -1.0f,  8.0f,  2.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -1.0f,  0.0f,  0.0f, -1.0f,  2.0f,  8.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -1.0f,  0.0f,  0.0f, -1.0f,  8.0f,  8.0f,  0.973f,  0.480f,  0.002f,
    -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f, 10.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  2.0f, 10.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  8.0f, 10.0f,  0.973f,  0.480f,  0.002f,
     1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 10.0f, 10.0f,  0.973f,  0.480f,  0.002f,
     1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  2.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  1.0f,  0.6f,  1.0f,  0.0f,  0.0f,  8.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 10.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  2.0f,  2.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  8.0f,  2.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  2.0f,  8.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  8.0f,  8.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f, 10.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  2.0f, 10.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -1.0f,  0.6f,  1.0f,  0.0f,  0.0f,  8.0f, 10.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 10.0f, 10.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 10.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,  2.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f,  2.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f,  8.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f,  8.0f,  0.612f,  0.000f,  0.069f,
     1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, 10.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  2.0f, 10.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  8.0f, 10.0f,  0.612f,  0.000f,  0.069f,
    -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 10.0f, 10.0f,  0.612f,  0.000f,  0.069f,
    -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  1.0f,  0.6f, -1.0f,  0.0f,  0.0f,  2.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  1.0f, -0.6f, -1.0f,  0.0f,  0.0f,  8.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 10.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  2.0f,  2.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  8.0f,  2.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  2.0f,  8.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  8.0f,  8.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f, 10.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -1.0f,  0.6f, -1.0f,  0.0f,  0.0f,  2.0f, 10.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -1.0f, -0.6f, -1.0f,  0.0f,  0.0f,  8.0f, 10.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 10.0f, 10.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  2.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  8.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 10.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f,  0.6f,  0.0f,  1.0f,  0.0f,  2.0f,  2.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f,  0.6f,  0.0f,  1.0f,  0.0f,  8.0f,  2.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f, -0.6f,  0.0f,  1.0f,  0.0f,  2.0f,  8.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f, -0.6f,  0.0f,  1.0f,  0.0f,  8.0f,  8.0f,  0.000f,  0.254f,  0.637f,
    -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f, 10.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  2.0f, 10.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  8.0f, 10.0f,  0.000f,  0.254f,  0.637f,
     1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 10.0f, 10.0f,  0.000f,  0.254f,  0.637f,
    -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  2.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f,  8.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 10.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f, -0.6f,  0.0f, -1.0f,  0.0f,  2.0f,  2.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f, -0.6f,  0.0f, -1.0f,  0.0f,  8.0f,  2.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f,  0.6f,  0.0f, -1.0f,  0.0f,  2.0f,  8.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f,  0.6f,  0.0f, -1.0f,  0.0f,  8.0f,  8.0f,  0.001f,  0.447f,  0.067f,
    -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f, 10.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  2.0f, 10.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f,  8.0f, 10.0f,  0.001f,  0.447f,  0.067f,
     1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 10.0f, 10.0f,  0.001f,  0.447f,  0.067f,
    -0.6f,  0.6f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f, -0.6f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
    -0.6f,  0.6f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     0.6f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.973f,  0.480f,  0.002f,
     1.0f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     1.0f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.897f,  0.163f,  0.011f,
     0.6f,  0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  1.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f, -0.6f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
     0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -0.6f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.612f,  0.000f,  0.069f,
    -1.0f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f, -0.6f,  0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f, -0.6f, -0.6f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f,  0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -1.0f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  0.6f, -0.6f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,  0.127f,  0.116f,  0.408f,
    -0.6f,  1.0f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f,  1.0f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  1.0f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
     0.6f,  0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.000f,  0.254f,  0.637f,
    -0.6f, -0.6f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f,  0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -0.6f, -0.6f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -0.6f,  0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -0.6f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f, -0.6f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -0.6f, -0.6f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -1.0f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
    -0.6f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -0.6f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
     0.6f, -1.0f,  0.6f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,  0.001f,  0.447f,  0.067f,
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
	"cbuffer constants : register(b0) { row_major float4x4 transform; row_major float4x4 projection; float3 lightvector; }\n"
	"struct vs_in { float3 position : POS; float3 normal : NOR; float2 texcoord : TEX; float3 color : COL; };\n"
	"struct vs_out { float4 position : SV_POSITION; float2 texcoord : TEX; float4 color : COL; };\n"
	"Texture2D    mytexture : register(t0);\n"
	"SamplerState mysampler : register(s0);\n"
	"vs_out vs_main(vs_in input) {\n"
	"    float light = clamp(dot(normalize(mul(float4(input.normal, 0.0f), transform).xyz), normalize(-lightvector)), 0.0f, 1.0f) * 0.8f + 0.2f;\n"
	"    vs_out output;\n"
	"    output.position = mul(float4(input.position, 1.0f), mul(transform, projection));\n"
	"    output.texcoord = input.texcoord;\n"
	"    output.color = float4(input.color * light, 1.0f);\n"
	"    return output;\n"
	"}\n"
	"float4 ps_main(vs_out input) : SV_TARGET { return mytexture.Sample(mysampler, input.texcoord) * input.color; }\n"
);

struct float3 { float x, y, z; };
struct matrix { float m[4][4]; };

matrix operator*(const matrix& m1, const matrix& m2);

int spinnyCube(HWND window,
    ID3D11Device *baseDevice,
    ID3D11DeviceContext *baseDeviceContext) {

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

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3DBlob* vsBlob;

    D3DCompile(shaders, sizeof(shaders), "shaders.hlsl", nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vsBlob, nullptr);

    ID3D11VertexShader* vertexShader;

    device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &vertexShader);

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = // float3 position, float3 normal, float2 texcoord, float3 color
    {
        { "POS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,                            0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    // device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &inputLayout);

    cdc::ShaderSub vsShaderSub {
        nullptr,
        vsBlob->GetBufferPointer(),
        0,
        (uint16_t)vsBlob->GetBufferSize()
    };

    cdc::PCDX11StreamDecl streamDecl(
        static_cast<cdc::PCDX11RenderDevice*>(cdc::gRenderDevice),
        inputElementDesc, ARRAYSIZE(inputElementDesc), &vsShaderSub);
    streamDecl.internalResource04();

    ///////////////////////////////////////////////////////////////////////////////////////////////

    ID3DBlob* psBlob;

    D3DCompile(shaders, sizeof(shaders), "shaders.hlsl", nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &psBlob, nullptr);

    ID3D11PixelShader* pixelShader;

    device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &pixelShader);

    cdc::PCDX11VertexShader cdcVertexShader(vertexShader);
    cdc::PCDX11PixelShader cdcPixelShader(pixelShader);

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

    struct Constants
    {
        matrix Transform;
        matrix Projection;
        float3 LightVector;
    };

    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.ByteWidth      = sizeof(Constants) + 0xf & 0xfffffff0; // round constant buffer size to 16 byte boundary
    constantBufferDesc.Usage          = D3D11_USAGE_DYNAMIC;
    constantBufferDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    ID3D11Buffer* constantBuffer;

    device->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    UINT stride = 11 * 4; // vertex size (11 floats: float3 position, float3 normal, float2 texcoord, float3 color)
    cdc::PCDX11StaticVertexBuffer cdcVertexBuffer(stride, sizeof(VertexData) / stride, (void*)VertexData);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    D3D11_BUFFER_DESC indexBufferDesc = {};
    indexBufferDesc.ByteWidth = sizeof(IndexData);
    indexBufferDesc.Usage     = D3D11_USAGE_IMMUTABLE;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA indexData = { IndexData };

    ID3D11Buffer* indexBuffer;
    device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);

    cdc::PCDX11StateManager stateManager(deviceContext, device);
    cdc::HackIndexBuffer cdcIndexBuffer(indexBuffer);

    cdc::RenderResourceSection renderResourceSection;
    cdc::ResolveSection *resolveSections[16] = {nullptr};
    resolveSections[5] = &renderResourceSection;
    hackResolveReceiver("alc_beer_bottle_a.drm", resolveSections);
    auto bottleTexture = (cdc::PCDX11Texture*)renderResourceSection.getWrapped(0x0396);
    printf("have bottle cdc texture: %p\n", bottleTexture);
    bottleTexture->asyncCreate();
    printf("have bottle d3d texture: %p\n", bottleTexture->d3dTexture128);

    stateManager.setTextureAndSampler(0, bottleTexture, 0, 0.0f);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    FLOAT backgroundColor[4] = { 0.025f, 0.025f, 0.025f, 1.0f };

    D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(depthBufferDesc.Width), static_cast<float>(depthBufferDesc.Height), 0.0f, 1.0f };
    
    ///////////////////////////////////////////////////////////////////////////////////////////////

    float w = viewport.Width / viewport.Height; // width (aspect ratio)
    float h = 1.0f;                             // height
    float n = 1.0f;                             // near
    float f = 9.0f;                             // far

    float3 modelRotation    = { 0.0f, 0.0f, 0.0f };
    float3 modelScale       = { 1.0f, 1.0f, 1.0f };
    float3 modelTranslation = { 0.0f, 0.0f, 4.0f };

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

            //if (msg.message == WM_KEYDOWN) return 0;
            DispatchMessageA(&msg);
        }

        ///////////////////////////////////////////////////////////////////////////////////////////

        matrix rotateX   = { 1, 0, 0, 0, 0, static_cast<float>(cos(modelRotation.x)), -static_cast<float>(sin(modelRotation.x)), 0, 0, static_cast<float>(sin(modelRotation.x)), static_cast<float>(cos(modelRotation.x)), 0, 0, 0, 0, 1 };
        matrix rotateY   = { static_cast<float>(cos(modelRotation.y)), 0, static_cast<float>(sin(modelRotation.y)), 0, 0, 1, 0, 0, -static_cast<float>(sin(modelRotation.y)), 0, static_cast<float>(cos(modelRotation.y)), 0, 0, 0, 0, 1 };
        matrix rotateZ   = { static_cast<float>(cos(modelRotation.z)), -static_cast<float>(sin(modelRotation.z)), 0, 0, static_cast<float>(sin(modelRotation.z)), static_cast<float>(cos(modelRotation.z)), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
        matrix scale     = { modelScale.x, 0, 0, 0, 0, modelScale.y, 0, 0, 0, 0, modelScale.z, 0, 0, 0, 0, 1 };
        matrix translate = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, modelTranslation.x, modelTranslation.y, modelTranslation.z, 1 };

        modelRotation.x += 0.005f;
        modelRotation.y += 0.009f;
        modelRotation.z += 0.001f;

        ///////////////////////////////////////////////////////////////////////////////////////////

        D3D11_MAPPED_SUBRESOURCE mappedSubresource;

        deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);

        Constants* constants = reinterpret_cast<Constants*>(mappedSubresource.pData);

        constants->Transform   = rotateX * rotateY * rotateZ * scale * translate;
        constants->Projection  = { 2 * n / w, 0, 0, 0, 0, 2 * n / h, 0, 0, 0, 0, f / (f - n), 1, 0, 0, n * f / (n - f), 0 };
        constants->LightVector = { 1.0f, -1.0f, 1.0f };

        deviceContext->Unmap(constantBuffer, 0);

        ///////////////////////////////////////////////////////////////////////////////////////////

        deviceContext->ClearRenderTargetView(frameBufferView, backgroundColor);
        deviceContext->ClearDepthStencilView(depthBufferView, D3D11_CLEAR_DEPTH, 1.0f, 0);

        stateManager.setPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        stateManager.setStreamDecl(&streamDecl);
        stateManager.setVertexBuffer(&cdcVertexBuffer);
        stateManager.setIndexBuffer(&cdcIndexBuffer);

        stateManager.setVertexShader(&cdcVertexShader);
        deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

        deviceContext->RSSetViewports(1, &viewport);
        deviceContext->RSSetState(rasterizerState);

        stateManager.setPixelShader(&cdcPixelShader);
        stateManager.updateShaderResources();
        stateManager.updateSamplers();

        deviceContext->OMSetRenderTargets(1, &frameBufferView, depthBufferView);
        deviceContext->OMSetDepthStencilState(depthStencilState, 0);
        deviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff); // use default blend mode (i.e. disable)

        ///////////////////////////////////////////////////////////////////////////////////////////

        deviceContext->DrawIndexed(ARRAYSIZE(IndexData), 0, 0);

        ///////////////////////////////////////////////////////////////////////////////////////////

        swapChain->Present(1, 0);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

matrix operator*(const matrix& m1, const matrix& m2)
{
    return
    {
        m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0],
        m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1],
        m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2],
        m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3],
        m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0],
        m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1],
        m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2],
        m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3],
        m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0],
        m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1],
        m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2],
        m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3],
        m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0],
        m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1],
        m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2],
        m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3],
    };
}
