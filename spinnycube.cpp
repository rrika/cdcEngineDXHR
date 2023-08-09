#include <cmath>
#include <cstdio>
#include <functional>
#include <iterator>
#include <memory>
#include "config.h" // for ENABLE_IMGUI and ENABLE_D3DCOMPILER

#include <windows.h>
#include <d3d11_1.h>
#if ENABLE_D3DCOMPILER
#include <d3dcompiler.h>
#endif

#include "spinnycube.h"
#include "camera/CameraManager.h"
#include "camera/GenericCamera.h"
#include "cdc/dtp/objectproperties/imfref.h"
#include "cdc/dtp/objectproperties/intermediatemesh.h"
#include "cdc/dtp/soundplex.h"
#include "cdcFile/ArchiveFileSystem.h"
#include "cdcFile/FileHelpers.h" // for archiveFileSystem_default
#include "cdcFile/FileSystem.h" // for enum cdc::FileRequest::Priority
#include "cdcFile/FileUserBufferReceiver.h"
#include "game/dtp/objecttypes/globaldatabase.h"
#include "game/Gameloop.h"
#include "game/script/game/NsMainMenuMovieController.h"
#include "game/ui/FakeScaleform/fakescaleform.h"
#include "game/ui/Scaleform/ScaleformMovieInstance.h"
#include "cdcGameShell/cdcGameShell.h" // for LOAD_IMFFileName
#ifdef _WIN32
#include "cdcGameShell/win32/MainVM.h" // for yellowCursor
#endif
#include "input/PCMouseKeyboard.h"
#include "cdcLocale/localstr.h"
#include "cdcMath/Math.h" // for cdc::Matrix
#include "cdcObjects/Object.h"
#include "cdcObjects/ObjectManager.h" // for buildObjectsUI
#include "rendering/buffers/PCDX11ConstantBufferPool.h"
#include "rendering/buffers/PCDX11IndexBuffer.h"
#include "rendering/buffers/PCDX11UberConstantBuffer.h"
#include "rendering/drawables/PCDX11FXAADrawable.h"
#include "rendering/CommonRenderTerrainInstance.h"
#include "rendering/IPCDeviceManager.h"
#include "rendering/IRenderPassCallback.h"
#include "rendering/pc/buffers/PCStaticIndexBuffer.h"
#include "rendering/pc/buffers/PCVertexBuffer.h"
#include "rendering/pc/PCDeviceManager.h"
#include "rendering/pc/PCMaterial.h"
#include "rendering/pc/PCModelDrawable.h"
#include "rendering/pc/PCRenderContext.h"
#include "rendering/pc/PCRenderDevice.h"
#include "rendering/pc/PCRenderModel.h"
#include "rendering/pc/PCStateManager.h"
#include "rendering/pc/PCStreamDecl.h"
#include "rendering/pc/PCStreamDeclManager.h"
#include "rendering/pc/shaders/PCPixelShader.h"
#include "rendering/pc/shaders/PCVertexShader.h"
#include "rendering/pc/surfaces/PCDeviceTexture.h"
#include "rendering/pc/surfaces/PCTexture.h"
#include "rendering/PCDX11DeviceManager.h"
#include "rendering/PCDX11Material.h"
#include "rendering/PCDX11MatrixState.h"
#include "rendering/PCDX11RenderContext.h"
#include "rendering/PCDX11RenderDevice.h"
#include "rendering/PCDX11RenderModel.h"
#include "rendering/PCDX11RenderModelInstance.h"
#include "rendering/PCDX11RenderTerrain.h"
#include "rendering/PCDX11Scene.h"
#include "rendering/PCDX11StateManager.h"
#include "rendering/PCDX11StreamDecl.h"
#include "rendering/Projection.h"
#include "rendering/renderdevice.h"
#include "rendering/RenderModelInstance.h"
#include "rendering/RenderPasses.h"
#include "rendering/shaders/PCDX11PixelShader.h"
#include "rendering/shaders/PCDX11VertexShader.h"
#include "rendering/surfaces/PCDX11DefaultRenderTarget.h"
#include "rendering/surfaces/PCDX11DepthBuffer.h"
#include "rendering/surfaces/PCDX11Texture.h"
#include "rendering/VertexDeclaration.h"
#include "cdcResource/DRMIndex.h"
#include "cdcResource/ResolveObject.h"
#include "cdcResource/ResolveReceiver.h"
#include "cdcResource/ResolveSection.h"
#include "cdcResource/WaveSection.h"
#include "cdcScript/ScriptType.h"
#include "scene/IMFTypes.h"
#include "scene/IScene.h"
#include "scene/SceneCellGroup.h" // for SceneCellGroup to ISceneCellGroup cast
#include "scene/SceneEntity.h"
#include "cdcSound/SoundPlex.h"
#include "cdcWorld/RMIDrawableBase.h"
#include "cdcWorld/stream.h" // for buildUnitsUI
#include "cdcWorld/StreamUnit.h"
#include "cdcWorld/SceneLayer.h" // for g_scene

#if ENABLE_IMGUI
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_dx9.h"
#include "imgui/backends/imgui_impl_dx11.h"
#ifdef _WIN32
#include "imgui/backends/imgui_impl_win32.h"
#else
#include "imgui/backends/imgui_impl_sdl.h"
#endif
#include "rendering/Inspector.h"
#endif

#ifdef __linux__
#include <SDL2/SDL.h>
#endif

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

const char shaders9 [] = (
	"float4x4 WorldViewProject : register(c0);\n"
	"float4x4 World : register(c4);\n"
	"struct vs_in { float4 position : POSITION; float3 normal : NORMAL; float2 texcoord : TEXCOORD1; float3 color : COLOR; };\n"
	"struct vs_out { float4 position : SV_POSITION; float4 color : COLOR; float2 texcoord : TEXCOORD; };\n"
	"sampler2D mytexture;\n"
	"static const float3 lightvector = { 1.0f, -1.0f, 1.0f };\n"
	"vs_out vs_main(vs_in input) {\n"
	"    float light = clamp(dot(normalize(mul(World, float4(input.normal, 0.0f)).xyz), normalize(-lightvector)), 0.0f, 1.0f) * 0.8f + 0.2f;\n"
	"    vs_out output;\n"
	"    output.position = mul(WorldViewProject, input.position);\n"
	"    output.texcoord = input.texcoord;\n"
	"    output.color = float4(input.color * light, 1.0f);\n"
	"    return output;\n"
	"}\n"
	"float4 ps_main(vs_out input) : SV_TARGET { return tex2D(mytexture, input.texcoord) * input.color; }\n"
);

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

const uint8_t psblob[] = {
	0x00, 0x02, 0xff, 0xff, 0xfe, 0xff, 0x20, 0x00, 0x43, 0x54, 0x41, 0x42, 0x1c, 0x00, 0x00, 0x00,
	0x53, 0x00, 0x00, 0x00, 0x00, 0x02, 0xff, 0xff, 0x01, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x6d, 0x79, 0x74, 0x65,
	0x78, 0x74, 0x75, 0x72, 0x65, 0x00, 0xab, 0xab, 0x04, 0x00, 0x0c, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0x73, 0x5f, 0x32, 0x5f, 0x30, 0x00, 0x4d,
	0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66, 0x74, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4c, 0x53,
	0x4c, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6f, 0x6d, 0x70, 0x69, 0x6c, 0x65,
	0x72, 0x20, 0x31, 0x30, 0x2e, 0x31, 0x00, 0xab, 0x1f, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x80,
	0x00, 0x00, 0x0f, 0x90, 0x1f, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x03, 0xb0,
	0x1f, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x90, 0x00, 0x08, 0x0f, 0xa0, 0x42, 0x00, 0x00, 0x03,
	0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0xe4, 0xb0, 0x00, 0x08, 0xe4, 0xa0, 0x05, 0x00, 0x00, 0x03,
	0x00, 0x00, 0x0f, 0x80, 0x00, 0x00, 0xe4, 0x80, 0x00, 0x00, 0xe4, 0x90, 0x01, 0x00, 0x00, 0x02,
	0x00, 0x08, 0x0f, 0x80, 0x00, 0x00, 0xe4, 0x80, 0xff, 0xff, 0x00, 0x00 
};

const uint8_t vsblob[] = {
	0x00, 0x02, 0xfe, 0xff, 0xfe, 0xff, 0x28, 0x00, 0x43, 0x54, 0x41, 0x42, 0x1c, 0x00, 0x00, 0x00,
	0x74, 0x00, 0x00, 0x00, 0x00, 0x02, 0xfe, 0xff, 0x02, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00,
	0x00, 0x01, 0x00, 0x00, 0x6d, 0x00, 0x00, 0x00, 0x44, 0x00, 0x00, 0x00, 0x02, 0x00, 0x04, 0x00,
	0x03, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x00, 0x00, 0x00,
	0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x57, 0x6f, 0x72, 0x6c, 0x64, 0x00, 0xab, 0xab, 0x03, 0x00, 0x03, 0x00, 0x04, 0x00, 0x04, 0x00,
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x56, 0x69, 0x65,
	0x77, 0x50, 0x72, 0x6f, 0x6a, 0x65, 0x63, 0x74, 0x00, 0x76, 0x73, 0x5f, 0x32, 0x5f, 0x30, 0x00,
	0x4d, 0x69, 0x63, 0x72, 0x6f, 0x73, 0x6f, 0x66, 0x74, 0x20, 0x28, 0x52, 0x29, 0x20, 0x48, 0x4c,
	0x53, 0x4c, 0x20, 0x53, 0x68, 0x61, 0x64, 0x65, 0x72, 0x20, 0x43, 0x6f, 0x6d, 0x70, 0x69, 0x6c,
	0x65, 0x72, 0x20, 0x31, 0x30, 0x2e, 0x31, 0x00, 0x51, 0x00, 0x00, 0x05, 0x07, 0x00, 0x0f, 0xa0,
	0x3a, 0xcd, 0x13, 0xbf, 0x3a, 0xcd, 0x13, 0x3f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3f,
	0x51, 0x00, 0x00, 0x05, 0x08, 0x00, 0x0f, 0xa0, 0xcd, 0xcc, 0x4c, 0x3f, 0xcd, 0xcc, 0x4c, 0x3e,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x80,
	0x00, 0x00, 0x0f, 0x90, 0x1f, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x80, 0x01, 0x00, 0x0f, 0x90,
	0x1f, 0x00, 0x00, 0x02, 0x05, 0x00, 0x01, 0x80, 0x02, 0x00, 0x0f, 0x90, 0x1f, 0x00, 0x00, 0x02,
	0x0a, 0x00, 0x00, 0x80, 0x03, 0x00, 0x0f, 0x90, 0x05, 0x00, 0x00, 0x03, 0x00, 0x00, 0x0f, 0x80,
	0x00, 0x00, 0x55, 0x90, 0x01, 0x00, 0xe4, 0xa0, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x0f, 0x80,
	0x00, 0x00, 0xe4, 0xa0, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0xe4, 0x80, 0x04, 0x00, 0x00, 0x04,
	0x00, 0x00, 0x0f, 0x80, 0x02, 0x00, 0xe4, 0xa0, 0x00, 0x00, 0xaa, 0x90, 0x00, 0x00, 0xe4, 0x80,
	0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x0f, 0xc0, 0x03, 0x00, 0xe4, 0xa0, 0x00, 0x00, 0xff, 0x90,
	0x00, 0x00, 0xe4, 0x80, 0x05, 0x00, 0x00, 0x03, 0x00, 0x00, 0x07, 0x80, 0x01, 0x00, 0x55, 0x90,
	0x05, 0x00, 0xe4, 0xa0, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x07, 0x80, 0x04, 0x00, 0xe4, 0xa0,
	0x01, 0x00, 0x00, 0x90, 0x00, 0x00, 0xe4, 0x80, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x07, 0x80,
	0x06, 0x00, 0xe4, 0xa0, 0x01, 0x00, 0xaa, 0x90, 0x00, 0x00, 0xe4, 0x80, 0x24, 0x00, 0x00, 0x02,
	0x01, 0x00, 0x07, 0x80, 0x00, 0x00, 0xe4, 0x80, 0x08, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01, 0x80,
	0x01, 0x00, 0xd8, 0x80, 0x07, 0x00, 0xd0, 0xa0, 0x0b, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01, 0x80,
	0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0xaa, 0xa0, 0x0a, 0x00, 0x00, 0x03, 0x00, 0x00, 0x01, 0x80,
	0x00, 0x00, 0x00, 0x80, 0x07, 0x00, 0xff, 0xa0, 0x04, 0x00, 0x00, 0x04, 0x00, 0x00, 0x01, 0x80,
	0x00, 0x00, 0x00, 0x80, 0x08, 0x00, 0x00, 0xa0, 0x08, 0x00, 0x55, 0xa0, 0x05, 0x00, 0x00, 0x03,
	0x00, 0x00, 0x07, 0xd0, 0x00, 0x00, 0x00, 0x80, 0x03, 0x00, 0xe4, 0x90, 0x01, 0x00, 0x00, 0x02,
	0x00, 0x00, 0x08, 0xd0, 0x07, 0x00, 0xff, 0xa0, 0x01, 0x00, 0x00, 0x02, 0x00, 0x00, 0x03, 0xe0,
	0x02, 0x00, 0xe4, 0x90, 0xff, 0xff, 0x00, 0x00
};

class ImGuiDrawable : public cdc::IRenderDrawable {
public:
	std::function<void()> lastMinuteAdditions;

	void draw(uint32_t funcSetIndex, IRenderDrawable *other) override;
	uint32_t compare(uint32_t funcSetIndex, IRenderDrawable *other) override { /*TODO*/ return 0; };
};

class SpinnyCubePass : public cdc::IRenderPassCallback {
public:
	D3D11_VIEWPORT *viewport;
	ID3D11BlendState1 *keepAlphaBlend;

	bool pre(
		cdc::CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		cdc::CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

DRMIndex drmIndex;

#if ENABLE_IMGUI
struct DRMExplorer {

	void draw(bool *showWindow) {
		ImGui::Begin("DRM Explorer", showWindow);

		ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
		if (ImGui::BeginTabBar("MyTabBar", tab_bar_flags)) {

			if (ImGui::BeginTabItem("Loaded DRMs")) {
				for (auto& entry : drmIndex.sectionHeaders) {
					if (ImGui::TreeNode(entry.first.c_str())) {
						uint32_t i=0;
						for (auto& section : entry.second) {
							const char *names[] = {
								"Generic",
								"Empty",
								"Animation",
								"",
								"",
								"RenderResource",
								"FMODSoundBank",
								"DTPData",
								"Script",
								"ShaderLib",
								"Material",
								"Object",
								"RenderMesh",
								"CollisionMesh",
								"StreamGroupList",
								"AnyType",
							};
							ImGui::Text("%3d: %04x %s allocFlags:%d unk6:%x (%d bytes)",
								i++, section.id, names[section.type], section.allocFlags, section.unknown06, section.payloadSize);
							if (section.type == 5) { // RenderResource
								ImGui::Text("    ");
								ImGui::SameLine();
								auto *resource = (cdc::RenderResource*)cdc::g_resolveSections[5]->getWrapped(section.id);
								if (auto tex = dynamic_cast<cdc::PCDX11Texture*>(resource)) {
									ImGui::Image(
										tex->createShaderResourceView(), ImVec2(256, 256));
								}
								if (auto tex = dynamic_cast<cdc::PCTexture*>(resource)) {
									if (auto *deviceTexture = tex->GetDeviceBaseTexture())
										ImGui::Image(deviceTexture->GetD3DTexture(), ImVec2(256, 256));
								}
							}
							if (section.type == 6) { // FMOD
								ImGui::PushID(section.id);
								ImGui::SameLine();
								if (ImGui::SmallButton("Play sound")) {
									((cdc::WaveSection*)cdc::g_resolveSections[6])->playSound(section.id);
								}
								ImGui::PopID();
							}
							if (section.type == 7 && section.allocFlags == 0xD) { // DTP (SoundPlex)
								ImGui::PushID(section.id);
								ImGui::SameLine();
								auto *plex = (dtp::SoundPlex*)cdc::g_resolveSections[7]->getWrapped(section.id);
								if (plex) {
									if (ImGui::SmallButton("Play soundplex")) {
										cdc::SOUND_StartPaused(plex, /*delay=*/ 0.0f);
									}
									buildUI(plex, /*indent=*/ "    ");
								}
								ImGui::PopID();

							}
							if (section.type == 8) { // Script
								if (auto *ty = (cdc::ScriptType*)cdc::g_resolveSections[8]->getWrapped(section.id)) {
									ImGui::SameLine();
									ImGui::Text(" %s", ty->blob->name);
								}
							}
						}
						ImGui::TreePop();
					}
				}
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}

		ImGui::End();
	}
} drmexplorer;
#endif

int spinnyCube(HWND window) {

	bool useDX11 = cdc::g_CurrentRenderer == cdc::RENDERER_DX11;

	if (!useDX11) {
		auto renderDevice9 = static_cast<cdc::PCRenderDevice*>(cdc::g_renderDevice);
		cdc::PCRenderContext *renderContext = renderDevice9->getRenderContext();
		auto d3dDevice9 = cdc::deviceManager9->getD3DDevice();
		renderContext->internalCreate();

#if ENABLE_IMGUI
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsDark();
	#ifdef _WIN32
		ImGui_ImplWin32_Init(window);
	#else
		ImGui_ImplSDL2_InitForSDLRenderer((SDL_Window*)window, nullptr);
	#endif
		ImGui_ImplDX9_Init(d3dDevice9);

		bool showDRMWindow = true;
#endif

		IDirect3DVertexBuffer9 *v_buffer = nullptr;
		d3dDevice9->CreateVertexBuffer(sizeof(VertexData),
			0,
			0,
			D3DPOOL_MANAGED,
			&v_buffer,
			nullptr);

		void *pVoid;
		v_buffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, VertexData, sizeof(VertexData));
		v_buffer->Unlock();

		cdc::HackVertexBuffer9 cdcVertexBuffer(v_buffer, 12*sizeof(float));

		IDirect3DIndexBuffer9 *i_buffer = nullptr;
		d3dDevice9->CreateIndexBuffer(sizeof(IndexData),
			0,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			&i_buffer,
			nullptr);
		i_buffer->Lock(0, 0, (void**)&pVoid, 0);
		memcpy(pVoid, IndexData, sizeof(IndexData));
		i_buffer->Unlock();

		cdc::PCStateManager stateManager9(d3dDevice9);
		cdc::deviceManager9->stateManager = &stateManager9;
		cdc::HackIndexBuffer9 cdcIndexBuffer9(i_buffer);

#if ENABLE_D3DCOMPILER
		ID3DBlob *vsBlob, *psBlob;

		D3DCompile(shaders9, sizeof(shaders9), "shaders.hlsl", nullptr, nullptr, "vs_main", "vs_2_0", 0, 0, &vsBlob, nullptr);
		D3DCompile(shaders9, sizeof(shaders9), "shaders.hlsl", nullptr, nullptr, "ps_main", "ps_2_0", 0, 0, &psBlob, nullptr);

		cdc::PCVertexShader cdcVertex9((char*)vsBlob->GetBufferPointer(), false, false);
		cdc::PCPixelShader cdcPixel9((char*)psBlob->GetBufferPointer(), false, false);
#else
		cdc::PCVertexShader cdcVertex9((char*)vsblob, false, false);
		cdc::PCPixelShader cdcPixel9((char*)psblob, false, false);
#endif // ENABLE_D3DCOMPILER

		/*
		D3DVERTEXELEMENT9 elements[] = {
			{0,  0, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION,  0},
			{0, 16, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,    0},
			{0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  1},
			{0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR,     0},
			D3DDECL_END()
		};
		cdc::PCStreamDecl cdcStreamDecl(elements);
		*/

		cdc::PCStreamDeclManager streamDeclManager(renderDevice9);
		cdc::PCStreamDecl *pCdcStreamDecl = nullptr;
		cdc::VertexAttributeA cdcElements[4] = {
			{ cdc::VertexAttributeA::kPosition,   0, 3, 0}, // D3DDECLTYPE_FLOAT4
			{ cdc::VertexAttributeA::kNormal,    16, 2, 0}, // D3DDECLTYPE_FLOAT3
			{ cdc::VertexAttributeA::kTexcoord2, 28, 1, 0}, // D3DDECLTYPE_FLOAT2
			{ cdc::VertexAttributeA::kColor1,    36, 2, 0}  // D3DDECLTYPE_FLOAT3
		};
		auto *vertexDecl = cdc::VertexDecl::Create(cdcElements, 4, cdcVertexBuffer.GetStride());
		auto *inputSpec = (cdc::ShaderInputSpec *)new char[
			sizeof(cdc::ShaderInputSpec) +
			sizeof(cdc::VertexAttributeB) * 5];
		inputSpec->hash0 = 0;
		inputSpec->hash4 = 0;
		inputSpec->numAttribs = 4;
		inputSpec->dwordC = 0;
		inputSpec->attr[0] = { cdc::VertexAttributeA::kPosition,  ~0u,  1 };
		inputSpec->attr[1] = { cdc::VertexAttributeA::kNormal,    ~0u,  4 };
		inputSpec->attr[2] = { cdc::VertexAttributeA::kTexcoord2, ~0u, 11 };
		inputSpec->attr[3] = { cdc::VertexAttributeA::kColor1,      4,  8 };
		inputSpec->attr[4] = { cdc::VertexAttributeA::kNormal,    ~0u,  8 };
		if (true) {
			pCdcStreamDecl = streamDeclManager.FindOrCreate(vertexDecl, inputSpec, true);
		} else {
			pCdcStreamDecl = streamDeclManager.FindOrCreate(vertexDecl);
		}
		cdc::PCStreamDecl& cdcStreamDecl = *pCdcStreamDecl;

		cdcStreamDecl.internalCreate();

		cdc::Matrix World_Bottle = {
			0.03, 0, 0, 0,
			0, 0, 0.03, 0,
			0, 0.03, 0, 0,
			0, -1, 3, 1 };

		cdc::Matrix World_Cube = {
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 3, 1 };

		cdc::Matrix Project = cdc::BuildPerspectiveLH(
			0.925f,
			4.0f/3.0f,
			0.01f,
			100.0f);

		cdc::Matrix WorldViewProject_Bottle = Project * World_Bottle;
		cdc::Matrix WorldViewProject_Cube = Project * World_Cube;

		auto bottleIndex = cdc::objectIdByName("alc_beer_bottle_a");
		cdc::requestObjectNormal(bottleIndex);
		cdc::archiveFileSystem_default->processAll();
		auto bottleTexture = (cdc::PCTexture*)cdc::g_resolveSections[5]->getWrapped(0x0396);

		cdc::ResolveSection *objectSection = cdc::g_resolveSections[11];
		cdc::Object *bottleObject = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(0x04a8));
		auto bottleRenderModel = (cdc::PCRenderModel*)bottleObject->models[0]->renderMesh;

		cdc::ModelBatch *bottleBatch0 = &bottleRenderModel->modelBatches[0];
		cdc::PrimGroup *bottleGroup0 = &bottleRenderModel->primGroups[0];
		cdc::VertexDecl *bottleVertexDecl = (cdc::VertexDecl*) bottleBatch0->format;
		cdc::PCMaterial *bottleMaterial = static_cast<cdc::PCMaterial*>(bottleGroup0->material);
		cdc::PCStreamDecl *bottleStreamDecl = streamDeclManager.FindOrCreate(bottleVertexDecl, inputSpec, true);;
		bottleStreamDecl->internalCreate();

		cdc::PersistentPGData bottlePPG {
			.material = bottleMaterial
		};
		cdc::PCModelDrawable bottleRenderDrawable {
			bottleRenderModel,
			/*ext=*/ nullptr,
			bottleBatch0,
			bottleGroup0,
			&bottlePPG,
			/*poseData=*/ nullptr,
			1,
			1.0f,
			/*flags=*/ 0
		};

		cdc::MaterialInstanceData bottleMaterialInstance;

		for (uint32_t i=0; i<bottleVertexDecl->numAttr; i++) {
			cdc::VertexAttributeA& a = bottleVertexDecl->attrib[i];
			printf("%08x %d %d %d\n",
				a.attribKind,
				a.offset,
				a.format,
				a.bufferIndex);
		}

		cdc::RenderViewport renderViewport;
		renderViewport.nearz = 1.0f;
		renderViewport.farz = 10000.0f;
		renderViewport.fov = 0.925f;
		renderViewport.aspect = 4.f/3;
		renderViewport.mask = 0x1000; // normals pass

		while (true)
		{
		#ifdef _WIN32
			MSG msg;

			while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				if (msg.message == WM_DESTROY) {
					PostQuitMessage(0);
					goto end2;
				}
				if (msg.message == WM_QUIT) {
					goto end2;
				}

				//if (msg.message == WM_KEYDOWN) return 0;
				DispatchMessageA(&msg);
			}
		#else
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
			#if ENABLE_IMGUI
				ImGui_ImplSDL2_ProcessEvent(&event);
			#endif
				switch (event.type) {
					case SDL_WINDOWEVENT:
						// if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
						// 	renderDevice11->handleResize(event.window.data1, event.window.data2);
						break;
					case SDL_QUIT:
						goto end2;
					default:
						break;
				}
			}
		#endif

	#if ENABLE_IMGUI
			ImGui_ImplDX9_NewFrame();
		#ifdef _WIN32
			ImGui_ImplWin32_NewFrame(); // this will reset our pretty cursor
		#else
			ImGui_ImplSDL2_NewFrame();
		#endif
			ImGui::NewFrame();

			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("Test")) {
					if (ImGui::MenuItem("Show DRMs")) { showDRMWindow = true; }
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}

			if (showDRMWindow)
				drmexplorer.draw(&showDRMWindow);
	#endif

			d3dDevice9->SetRenderState(D3DRS_ZENABLE, TRUE);
			d3dDevice9->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
			d3dDevice9->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
			d3dDevice9->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
			d3dDevice9->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
			D3DCOLOR clear_col_dx = D3DCOLOR_RGBA(0x80, 0x80, 0x80, 0xff);
			d3dDevice9->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);

			if (d3dDevice9->BeginScene() >= 0) {

				stateManager9.setDeviceTexture(0, bottleTexture->GetDeviceBaseTexture(), cdc::kTextureFilterTrilinear, 0.0f);

				// draw bottle
				if (true) {
					stateManager9.SetWorldMatrix(&World_Bottle);
					stateManager9.SetViewMatrix(&identity4x4);
					stateManager9.SetProjectionMatrix(&Project);
					stateManager9.UpdateStateMatrices();
				} else {
					d3dDevice9->SetVertexShaderConstantF(0, (float*)WorldViewProject_Bottle.m, 4);
					d3dDevice9->SetVertexShaderConstantF(4, (float*)World_Bottle.m, 4);
				}
				struct { float values[4]; } row;
				row = {2.0f, -1.0f, 0.0f, 0.0f}; d3dDevice9->SetVertexShaderConstantF(21, (float*)WorldViewProject_Bottle.m, 1); // NormalScaleOffset
				row = {1.0f,  0.0f, 0.0f, 0.0f}; d3dDevice9->SetVertexShaderConstantF(254, (float*)World_Bottle.m, 1); // TexcoordScales
				row = {1.0f, 0.0f, 0.0f, 0.0f}; d3dDevice9->SetPixelShaderConstantF(  1, row.values, 1); // opacity
				row = {1.0f, 0.0f, 0.0f, 0.0f}; d3dDevice9->SetPixelShaderConstantF( 39, row.values, 1); // some matrix
				row = {0.0f, 1.0f, 0.0f, 0.0f}; d3dDevice9->SetPixelShaderConstantF( 40, row.values, 1); // some matrix
				row = {0.0f, 0.0f, 1.0f, 0.0f}; d3dDevice9->SetPixelShaderConstantF( 41, row.values, 1); // some matrix
				row = {1.0f, 1.0f, 1.0f, 1.0f}; d3dDevice9->SetPixelShaderConstantF(100, row.values, 1); // some scaling factors

				if (true) {
					if (true) {
						renderDevice9->resetRenderLists();
						renderDevice9->beginRenderList(nullptr);
						renderDevice9->createSubScene(&renderViewport, nullptr, nullptr);
						renderDevice9->recordDrawable(&bottleRenderDrawable, 0x1000, false);
						renderDevice9->finishScene();
						renderDevice9->endRenderList();
						renderDevice9->drawRenderLists();

					} else if (true) {
						auto *linear = renderDevice9->getLinear();
						auto *list = new (linear, 0, true) cdc::DrawableList { linear, 0, 0, 0 };
						list->add(&bottleRenderDrawable);
						renderDevice9->renderPasses.draw(list, cdc::kPassIndexNormal);

					} else {
						cdc::PCModelDrawable::drawNormal(0, &bottleRenderDrawable, nullptr);
					}

				} else {
					cdc::PCStreamDecl *bottleStreamDecl2 = bottleMaterial->SetupNormalMapPass(
						bottleMaterialInstance,
						nullptr,
						0,
						bottleVertexDecl,
						0,
						1.0f);

					if (true) {
						bottleRenderDrawable.DrawPrimitive(renderDevice9, &stateManager9, bottleStreamDecl, false);

					} else {
						stateManager9.setVertexBuffer(static_cast<cdc::PCVertexBuffer*>(bottleBatch0->staticVertexBuffer));
						stateManager9.setIndexBuffer(bottleRenderModel->indexBuffer);
						//stateManager9.setStreamDecl(bottleStreamDecl); // more saturated colors (=normals)
						stateManager9.setStreamDecl(bottleStreamDecl2); // darker colors (=normals)
						d3dDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, bottleBatch0->numVertices, 0, bottleBatch0->numTrianglesProbably);
					}
				}

				// draw cube
				stateManager9.setVertexShader(&cdcVertex9);
				stateManager9.setPixelShader(&cdcPixel9);
				stateManager9.setVertexBuffer(&cdcVertexBuffer);
				stateManager9.setIndexBuffer(&cdcIndexBuffer9);
				stateManager9.setStreamDecl(&cdcStreamDecl);
				d3dDevice9->SetVertexShaderConstantF(0, (float*)WorldViewProject_Cube.m, 4);
				d3dDevice9->SetVertexShaderConstantF(4, (float*)World_Cube.m, 4);
				d3dDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 168, 0, 288 / 3);

			#if ENABLE_IMGUI
				ImGui::Render();
				ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			#endif
				d3dDevice9->EndScene();
			}

			// HRESULT result = d3dDevice9->Present(NULL, NULL, NULL, NULL);
			renderContext->present(nullptr, nullptr, window);
		}

	end2:
#if ENABLE_IMGUI
		ImGui_ImplDX9_Shutdown();
	#ifdef _WIN32
		ImGui_ImplWin32_Shutdown();
	#else
		ImGui_ImplSDL2_Shutdown();
	#endif
		ImGui::DestroyContext();
#endif
		return 0;
	}

	ID3D11Device *baseDevice = useDX11 ? cdc::deviceManager->getD3DDevice() : nullptr;
	ID3D11DeviceContext *baseDeviceContext = useDX11 ? cdc::deviceManager->getD3DDeviceContext() : nullptr;

	std::unique_ptr<cdc::PCMouseKeyboard> mouseKeyboard(cdc::PCMouseKeyboard::create(window));
	auto renderDevice11 = static_cast<cdc::PCDX11RenderDevice*>(cdc::g_renderDevice);
	auto renderDevice = cdc::g_renderDevice;

	///////////////////////////////////////////////////////////////////////////////////////////////

	ID3D11Device1* device;

	baseDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&device));

	ID3D11DeviceContext1* deviceContext;

	baseDeviceContext->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&deviceContext));

	///////////////////////////////////////////////////////////////////////////////////////////////

	cdc::PCDX11RenderContext *renderContext = renderDevice11->getRenderContextAny();
	renderContext->internalCreate();

	///////////////////////////////////////////////////////////////////////////////////////////////

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	renderContext->frameBuffer->GetDesc(&depthBufferDesc); // base on framebuffer properties

	{
		cdc::PCDX11RenderTarget& cdcRenderTarget = *renderContext->renderTarget2C;
		cdc::PCDX11DepthBuffer& cdcDepthBuffer = *renderContext->depthBuffer;
		cdcDepthBuffer.renderTexture.sampleCount = depthBufferDesc.SampleDesc.Count;
		cdcDepthBuffer.renderTexture.sampleQuality = depthBufferDesc.SampleDesc.Quality;

		cdcRenderTarget.getRenderTexture11()->createRenderTargetView();
		cdcDepthBuffer.renderTexture.createDepthStencilView();

		// do not keep the cdcRenderTarget and cdcDepthBuffer references around
		// these objects get destroyed on window resize
	}

	///////////////////////////////////////////////////////////////////////////////////////////////

#if ENABLE_IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
#ifdef _WIN32
	ImGui_ImplWin32_Init(window);
#else
	ImGui_ImplSDL2_InitForSDLRenderer((SDL_Window*)window, nullptr);
#endif
	ImGui_ImplDX11_Init(baseDevice, baseDeviceContext);
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////

	ID3D11BlendState1* keepAlphaBlend = NULL;

	D3D11_BLEND_DESC1 BlendState;
	memset(&BlendState, 0, sizeof(D3D11_BLEND_DESC1));
	BlendState.AlphaToCoverageEnable = false;
	BlendState.IndependentBlendEnable = false;
	BlendState.RenderTarget[0].BlendEnable = true;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; // ignore shader
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE; // keep RT value
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState1(&BlendState, &keepAlphaBlend);

	///////////////////////////////////////////////////////////////////////////////////////////////

	cdc::PCDX11StateManager stateManager(deviceContext, device);
	stateManager.internalCreate();
	cdc::deviceManager->stateManager = &stateManager; // hack

	auto bottleIndex = cdc::objectIdByName("alc_beer_bottle_a");
	cdc::requestObjectNormal(bottleIndex);

	auto lightIndex = cdc::objectIdByName("deferred_fast_omni_diffuse");
	cdc::requestObjectNormal(lightIndex);

	// see below for why
	auto obj3 = cdc::ResolveObject::create(
		"pc-w\\shaderlibs\\deferred_fast_omni_diffusemt_79464a5e59514c7f_dx11.drm",
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		0,
		cdc::FileRequest::NORMAL
	);
	auto obj4 = cdc::ResolveObject::create(
		"pc-w\\s_scn_det_sarifhq_rail_tutorial_barrettintro_det_sarifhq_rail_tutorial.drm",
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		0,
		cdc::FileRequest::NORMAL
	);

	auto obj5 = cdc::ResolveObject::create(
		"pc-w\\globaloutershell.drm",
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		0,
		cdc::FileRequest::NORMAL
	);

	cdc::archiveFileSystem_default->processAll();

	cdc::ResolveSection *objectSection = cdc::g_resolveSections[11];
	cdc::Object *bottleObject = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(0x04a8));
	printf("have bottle object: %p\n", bottleObject);

	// unrelated: get the name of the first map in the game
	uint32_t globalDatabaseId = cdc::objectIdByName("GlobalDatabase");
	cdc::Object *globalDatabaseObject = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(globalDatabaseId));
	auto *globalDatabase = (GlobalDatabase*)globalDatabaseObject->dword58;

	printf("first map is: %s\n", globalDatabase->newGameMap);

	auto bottleTexture = (cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x0396);
	printf("have bottle cdc texture: %p\n", bottleTexture);
	bottleTexture->asyncCreate();
	renderDevice11->missingTexture = bottleTexture;
	printf("have bottle d3d texture: %p\n", bottleTexture->d3dTexture128);

	// create the other four textures
	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x0395))->asyncCreate();
	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x005b))->asyncCreate();
	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x0061))->asyncCreate();

	auto bottleRenderModel_direct = (cdc::PCDX11RenderModel*)cdc::g_resolveSections[12]->getWrapped(0xA301);
	auto bottleRenderModel = (cdc::PCDX11RenderModel*)bottleObject->models[0]->renderMesh;


	printf("have bottle cdc render model: %p (directly)\n", bottleRenderModel_direct);
	printf("have bottle cdc render model: %p (via object)\n", bottleRenderModel);
	printf("have bottle cdc mesh blob: %p\n", bottleRenderModel->getMesh());

	for (uint32_t i = 0; i < bottleRenderModel->numPrimGroups; i++)
		printf("  bottle->tab0Ext128Byte[i].material = %p\n", bottleRenderModel->tab0Ext128Byte[i].material);

	RMIDrawableBase rmiDrawable(bottleRenderModel);

	auto sceneCube = g_scene->CreateEntity();
	sceneCube->setDrawable(&rmiDrawable);
	sceneCube->setCellGroup(g_scene->GetCellGroup(0));

	// cdc::RenderModelInstance *bottleRenderModelInstance =
	// 	renderDevice->createRenderModelInstance(bottleRenderModel);

	///////////////////////////////////////////////////////////////////////////////////////////////

	// for some reason deferred_fast_omni_diffuse.drm depends on shaderlibs/decal_853189b19db6f909_dx11.drm
	// when it should be using a shader from shaderlibs/decal_853189b19db6f909_dx11.drm
	// namely section 5: ShaderLib 0 cd2 unk6:2ce3 DX11 (4f38 bytes)
	//
	// one of the few materials referencing this shaderlib is
	// s_scn_det_sarifhq_rail_tutorial_barrettintro_det_sarifhq_rail_tutorial.drm/7: Material 12 a4 unk6:7a84 DX11 (7e0 bytes)


	cdc::Object *lightObject = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(lightIndex));
	printf("have light object: %p\n", lightObject);

	auto lightRenderModel = (cdc::PCDX11RenderModel*)lightObject->models[0]->renderMesh;
	printf("have light cdc render model: %p (via object)\n", lightRenderModel);

	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x0061))->asyncCreate();
	((cdc::PCDX11Texture*)cdc::g_resolveSections[5]->getWrapped(0x014c))->asyncCreate();

	auto lightMaterial = (cdc::PCDX11Material*)cdc::g_resolveSections[10]->getWrapped(0x00a4);
	printf("have light material (from scenario drm): %p\n", lightMaterial);

	// patch light material
	for (uint32_t i = 0; i < lightRenderModel->numPrimGroups; i++)
		lightRenderModel->tab0Ext128Byte[i].material = lightMaterial;

	cdc::RenderModelInstance *lightRenderModelInstance =
		renderDevice->createRenderModelInstance(lightRenderModel);

	cdc::Vector4 *lightInstanceParams = static_cast<cdc::CommonRenderModelInstance*>(lightRenderModelInstance)->ext->instanceParams;
	// taken from a random light draw in renderdoc
	float lightRadius = 2.0f;
	float invLightRadius = 1.0f / lightRadius;
	lightInstanceParams[0] = cdc::Vector4{1.0, 1.0, 2.0, 1.0}; // .xyz = viewspace light position perhaps
	lightInstanceParams[1] = cdc::Vector4{0.88235, 0.7098, 0.3451, 0.00}; // .xyz = light color
	lightInstanceParams[2] = cdc::Vector4{invLightRadius, invLightRadius, invLightRadius, 1.00}; // .x = 1/radius
	lightInstanceParams[3] = cdc::Vector4{0.80, 0.00, 0.00, 0.00}; // .x= light scale

	///////////////////////////////////////////////////////////////////////////////////////////////

	D3D11_VIEWPORT viewport = { 0.0f, 0.0f, static_cast<float>(depthBufferDesc.Width), static_cast<float>(depthBufferDesc.Height), 0.0f, 1.0f };

	///////////////////////////////////////////////////////////////////////////////////////////////

	float w = viewport.Width / viewport.Height; // width (aspect ratio)
	float h = 1.0f;                             // height
	float n = 1.0f;                             // near
	float f = 10000.0f;                         // far

	float scale = 1.f;
	cdc::Vector modelRotation    = { 0.0f, 0.0f, 0.0f };
	cdc::Vector modelScale       = { scale, scale, scale };
	cdc::Vector modelTranslation = { 50.0f, 0.0f, 0.0f };

	bool mouseLook = false;
	cdc::Vector cameraPos{0, 0, 0};

	///////////////////////////////////////////////////////////////////////////////////////////////

	GenericCamera camera;
	CameraManager cameraManager;

	cameraManager.activeCamera = &camera;

	///////////////////////////////////////////////////////////////////////////////////////////////

	cdc::RenderViewport renderViewport;
	renderViewport.nearz = n;
	renderViewport.farz = f;
	renderViewport.fov = 0.925f;
	renderViewport.aspect = w;
	renderViewport.mask = 0x310B; // pass 0, 12. 13, 1, and 8
	// pass 12 normals (function set 10, draw bottle normals)
	// pass 13 deferred shading (just contains a cleardrawable)
	// pass 1 composite (draw bottle textures)
	// pass 3 translucent
	// pass 8 runs last and is where I put imgui since it messes with the render state

	SpinnyCubePass cubePass;
	cubePass.viewport = &viewport;
	cubePass.keepAlphaBlend = keepAlphaBlend;
	renderDevice->setPassCallback(0, &cubePass);

	ImGuiDrawable imGuiDrawable;

	cdc::PCDX11FXAADrawable fxaaDrawable(
		renderDevice11,
		/*quality*/ 2,
		/*texture*/ nullptr, // HACK
		/*renderTarget*/ nullptr, // HACK
		/*flags*/ 0,
		/*sortZ*/ 0.0f);

	///////////////////////////////////////////////////////////////////////////////////////////////

	int introShowRange[2] = {0, 99999};
	int imfShowRange[2] = {0, 0};
#if ENABLE_IMGUI
	bool loadedSarifHQ = false;
	bool showDrawablesWindow = false;
	bool showFilesystemWindow = false;
	bool showObjectsWindow = false;
	bool showDRMWindow = false;
	bool showUnitsWindow = false;
	bool showLoadedUnitsWindow = false;
	bool showStringsWindow = false;
	std::vector<std::pair<void*, cdc::CommonScene*>> captures { { nullptr, nullptr } };
	uint32_t selectedCapture = 0;

	cdc::ScriptType *mainMenuScriptType =
		(cdc::ScriptType*)cdc::g_resolveSections[8]->getWrapped(0x154a7); // pc-w/globaloutershell.drm section 0xb7
	ScaleformMovieInstance mainMenuInstance(&mainMenuMovie);
	NsMainMenuMovieController mainMenuMovieController(mainMenuScriptType);

	// unsure how this link is established in the game
	mainMenuInstance.m_controllerArray.push_back(&mainMenuMovieController);
	mainMenuMovieController.movieInstance = &mainMenuInstance;

	mainMenuInstance.init();
#else
	mouseLook = true;
	mouseKeyboard->setCursorGrab(true);
#endif

	std::unordered_map<cdc::IRenderTerrain*, cdc::CommonRenderTerrainInstance *> renderTerrainInstances;

	cdc::GameShell::UpdateLoop(0.5);

	while (true)
	{
#ifdef _WIN32
		MSG msg;

		while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			if (msg.message == WM_DESTROY) {
				PostQuitMessage(0);
				goto end;
			}
			if (msg.message == WM_QUIT) {
				goto end;
			}

			mouseKeyboard->processWndProc(msg.message, msg.wParam, msg.lParam);

			//if (msg.message == WM_KEYDOWN) return 0;
			DispatchMessageA(&msg);
		}
#else
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
#if ENABLE_IMGUI
			ImGui_ImplSDL2_ProcessEvent(&event);
#endif
			mouseKeyboard->processSDLEvent(&event);

			switch (event.type) {
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
						renderDevice11->handleResize(event.window.data1, event.window.data2);
					break;
				case SDL_QUIT:
					goto end;
				default:
					break;
			}
		}
#endif

		mouseKeyboard->update();

		float forward = 0;
		float sideways = 0;
		float speedModifier = 100.0f;

#if ENABLE_IMGUI
		ImGui_ImplDX11_NewFrame();
#ifdef _WIN32
		ImGui_ImplWin32_NewFrame(); // this will reset our pretty cursor
#else
		ImGui_ImplSDL2_NewFrame();
#endif
		ImGui::NewFrame();

		if (ImGui::IsKeyPressed(ImGuiKey_Escape)) {
			mouseLook = false;
			mouseKeyboard->setCursorGrab(false);
		}
		else if (ImGui::IsKeyPressed(ImGuiKey_Tab)) {
			mouseLook = !mouseLook;
			mouseKeyboard->setCursorGrab(mouseLook);
		}
		if (ImGui::IsKeyDown(ImGuiKey_W))
			forward += 1.0f;
		if (ImGui::IsKeyDown(ImGuiKey_S))
			forward -= 1.0f;
		if (ImGui::IsKeyDown(ImGuiKey_A))
			sideways -= 1.0f;
		if (ImGui::IsKeyDown(ImGuiKey_D))
			sideways += 1.0f;
		if (ImGui::IsKeyDown(ImGuiKey_ModShift))
			speedModifier *= 4.0f;

#ifdef _WIN32
		if (mouseLook) {
			// ImGui::SetMouseCursor(ImGuiMouseCursor_None);
			SetCursor((HCURSOR)0);
			io.ConfigFlags |= ImGuiConfigFlags_NoMouse;
		} else {
			// ImGui::SetMouseCursor(ImGuiMouseCursor_Arrow);
			SetCursor((HCURSOR)yellowCursor); // ahh, much better
			io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
		}
#endif
#endif

		// cubePass.viewport ->
		viewport = {
			0.0f, 0.0f,
			static_cast<float>(renderContext->width),
			static_cast<float>(renderContext->height),
			0.0f, 1.0f };
		float w = viewport.Width / viewport.Height; // width (aspect ratio)
		renderViewport.aspect = w;

		///////////////////////////////////////////////////////////////////////////////////////////

		cdc::Matrix rotateX   = { 1, 0, 0, 0, 0, static_cast<float>(cos(modelRotation.x)), -static_cast<float>(sin(modelRotation.x)), 0, 0, static_cast<float>(sin(modelRotation.x)), static_cast<float>(cos(modelRotation.x)), 0, 0, 0, 0, 1 };
		cdc::Matrix rotateY   = { static_cast<float>(cos(modelRotation.y)), 0, static_cast<float>(sin(modelRotation.y)), 0, 0, 1, 0, 0, -static_cast<float>(sin(modelRotation.y)), 0, static_cast<float>(cos(modelRotation.y)), 0, 0, 0, 0, 1 };
		cdc::Matrix rotateZ   = { static_cast<float>(cos(modelRotation.z)), -static_cast<float>(sin(modelRotation.z)), 0, 0, static_cast<float>(sin(modelRotation.z)), static_cast<float>(cos(modelRotation.z)), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
		cdc::Matrix bottleScale = { modelScale.x, 0, 0, 0, 0, modelScale.y, 0, 0, 0, 0, modelScale.z, 0, 0, 0, 0, 1 };
		cdc::Matrix bottleTranslate = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, modelTranslation.x, modelTranslation.y, modelTranslation.z, 1 };
		cdc::Matrix cameraTranslate = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -cameraPos.x, -cameraPos.y, -cameraPos.z, 1 };
		cdc::Matrix lightScaleTranslate = { lightRadius * 2.0f, 0, 0, 0, 0, lightRadius * 2.0f, 0, 0, 0, 0, lightRadius * 2.0f, 0, 1, 1, 1, 1 };
		cdc::Matrix zUpWorld = {0, 0, 1, 0, -1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1};

		if (mouseLook) {
			modelRotation.x += mouseKeyboard->state.deltaY;
			modelRotation.y += mouseKeyboard->state.deltaX;
		}

		cdc::Matrix cameraRotate = rotateX * rotateY * zUpWorld;

		cameraPos += cdc::Vector{
			cameraRotate.m[0][2],
			cameraRotate.m[1][2],
			cameraRotate.m[2][2]} * (speedModifier * forward);
		cameraPos += cdc::Vector{
			cameraRotate.m[0][0],
			cameraRotate.m[1][0],
			cameraRotate.m[2][0]} * (speedModifier * sideways);

		///////////////////////////////////////////////////////////////////////////////////////////

		if (cdc::Level *level = STREAM_GetStreamUnitWithID(0)->level) {
			cdc::CellGroupData *cellGroupData = level->sub50;
			uint32_t numCells = cellGroupData->header->numTotalCells;
			for (uint32_t i=0; i < numCells; i++) {
				auto *cell = cellGroupData->cells[i];
				if (cell->sub4) {
					cdc::IRenderTerrain *rt = cell->sub4->pTerrain;
					if (rt) {
						if (renderTerrainInstances.find(rt) != renderTerrainInstances.end())
							continue;
						auto *rti = (cdc::CommonRenderTerrainInstance*)renderDevice->createRenderTerrainInstance(rt);
						renderTerrainInstances[rt] = rti;
						printf("created RenderTerrainInstance %p for RenderTerrain %p\n", rti, rt);
					}
				}
			}
		}

		///////////////////////////////////////////////////////////////////////////////////////////

		cdc::Matrix viewMatrix = cameraRotate * cameraTranslate;
		camera.matrix = viewMatrix;
		cameraManager.update();
		viewMatrix = *cameraManager.getMatrix(); // wow, it's nothing

		renderDevice->resetRenderLists();
		renderDevice->beginRenderList(nullptr);
		auto *scene = renderDevice->createSubScene(
			&renderViewport,
			renderContext->renderTarget2C,
			renderContext->depthBuffer);
		scene->viewMatrix = viewMatrix;

		cdc::Matrix bottleWorldMatrix = bottleTranslate * bottleScale;

		cdc::PCDX11MatrixState lightMatrixState(renderDevice11);
		lightMatrixState.resize(1);
		auto *lightWorldMatrix = reinterpret_cast<cdc::Matrix*>(lightMatrixState.poseData->getMatrix(0));
		*lightWorldMatrix = lightScaleTranslate;

		// add drawables to the scene
		float backgroundColor[4] = {0.025f, 0.025f, 0.025f, 1.0f};
		// float lightAccumulation[4] = {0.9f, 0.9f, 0.9f, 1.0f};
		float lightAccumulation[4] = {0.0f, 0.0f, 0.0f, 1.0f};

		StreamUnit *unit = STREAM_GetStreamUnitWithID(0);
		cdc::Level *level = unit->level;
		uint32_t numIntros = level ? level->admdData->numObjects : 0;
		dtp::Intro *intros = level ? level->admdData->objects : nullptr;
		uint32_t numIMFRefs = level ? level->admdData->numIMFRefs : 0;
		dtp::IMFRef *imfrefs = level ? level->admdData->imfrefs : nullptr;

		renderDevice->clearRenderTarget(10, /*mask=*/ 1, 0.0f, backgroundColor, 1.0f, 0);
		renderDevice->clearRenderTarget(2, /*mask=*/ 0x2000, 0.0f, lightAccumulation, 1.0f, 0); // deferred shading buffer
		static_cast<cdc::PCDX11RenderModelInstance*>(lightRenderModelInstance)->baseMask = 0x2000; // deferred lighting
		lightRenderModelInstance->recordDrawables(&lightMatrixState);
		// static_cast<cdc::PCDX11RenderModelInstance*>(bottleRenderModelInstance)->baseMask = 0x1002; // normals & composite
		// bottleRenderModelInstance->recordDrawables(&bottleMatrixState);

		static_cast<cdc::PCDX11RenderModelInstance*>(rmiDrawable.rmi)->baseMask = 0x1002; // normals & composite

		std::vector<std::unique_ptr<RMIDrawableBase>> recycleRMI;

		auto putObject = [&](cdc::PCDX11RenderModel *renderModel, cdc::Matrix& instanceMatrix) {
			if (renderModel) {
				// printf("%p %s\n", renderModel, typeid(*(cdc::RenderMesh*)renderModel).name());
				// printf("%p\n", renderModel->getMesh());

				// printf("%f %f %f\n", instanceMatrix.m[3][0], instanceMatrix.m[3][1], instanceMatrix.m[3][2]);

				auto *instanceRMIDrawable = new RMIDrawableBase(renderModel);
				recycleRMI.emplace_back(instanceRMIDrawable);
				instanceRMIDrawable->draw(&instanceMatrix, 0.0f);
			} else {
				rmiDrawable.draw(&instanceMatrix, 0.0f);
			}
		};

		// all the other objects
		for (uint32_t i=introShowRange[0]; i<numIntros && i<introShowRange[1]; i++) {
			auto &intro = intros[i];
			float s = 1.f;
			cdc::Matrix instanceMatrix = {
				s*intro.scale[0], 0, 0, 0,
				0, s*intro.scale[1], 0, 0,
				0, 0, s*intro.scale[2], 0,
				intro.position[0], intro.position[1], intro.position[2], 1
			};
			if (intro.objectListIndex == 0)
				continue;
			cdc::Object *object = (cdc::Object*)objectSection->getWrapped(objectSection->getDomainId(intro.objectListIndex));
			if (!object)
				continue;
			if (object->numModels == 0)
				continue;
			auto *renderModel = (cdc::PCDX11RenderModel*)object->models[0]->renderMesh;
			putObject(renderModel, instanceMatrix);
		}

		for (uint32_t i=imfShowRange[0]; i<numIMFRefs && i<imfShowRange[1]; i++) {
			dtp::IMFRef &ref = imfrefs[i];
			if (!ref.m_imfDRMName)
				continue;
			if (ref.m_pResolveObject == nullptr) {
				char path[256];
				cdc::GameShell::LOAD_IMFFileName(path, ref.m_imfDRMName);
				ref.m_pResolveObject = cdc::ResolveObject::create(
					path,
					nullptr, nullptr, nullptr,
					nullptr,
					nullptr,
					nullptr,
					0,
					cdc::FileRequest::NORMAL);
				cdc::archiveFileSystem_default->processAll();
			}
			if (isLoaded(ref.m_pResolveObject)) {
				//printf("%d %04x %s ", i, ref.dtpID, ref.m_imfDRMName);
				dtp::IntermediateMesh *im = cdc::GetIMFPointerFromId(ref.dtpID);
				//printf("%p ", im);
				cdc::RenderMesh *model = im->pRenderModel;
				//printf("%p\n", model);
				putObject(static_cast<cdc::PCDX11RenderModel*>(model), ref.m_transform);
			}
		}

		// single bottle at origin
		// rmiDrawable.draw(&bottleWorldMatrix, 0.0f);
		sceneCube->setMatrix(bottleWorldMatrix);
		g_scene->Draw();

		// draw cells
		if (cdc::Level *level = STREAM_GetStreamUnitWithID(0)->level) {
			cdc::CellGroupData *cellGroupData = level->sub50;
			uint32_t numCells = cellGroupData->header->numTotalCells;
			for (uint32_t i=0; i < numCells; i++) {
				auto *cell = cellGroupData->cells[i];

				// draw renderterrain
				if (cell->sub4) {
					cdc::IRenderTerrain *rt = cell->sub4->pTerrain;
					if (auto rtiIt = renderTerrainInstances.find(rt); rtiIt != renderTerrainInstances.end()) {
						static_cast<cdc::PCDX11RenderTerrain*>(rtiIt->first)->hackDraw(rtiIt->second, &bottleWorldMatrix);
					}
				}

				// draw rendermesh (these are for occlusion culling only, presumably)
				if (cell->renderMesh && false) {
					auto *cellRMIDrawable = new RMIDrawableBase(cell->renderMesh);
					recycleRMI.emplace_back(cellRMIDrawable);
					static_cast<cdc::PCDX11RenderModelInstance*>(cellRMIDrawable->rmi)->baseMask = 0x0002; // normals & composite
					cellRMIDrawable->draw(&bottleWorldMatrix, 0.0f);
				}
			}
		}

		// uncomment to apply FXAA to the normal map
		// can't apply to the proper color buffer because it'd be read/written at the same time
		// renderDevice->recordDrawable(&fxaaDrawable, /*mask=*/ 0x100, 0);

		renderDevice11->recordDrawable(&imGuiDrawable, /*mask=*/ 0x100, /*addToParent=*/ 0);
		renderDevice->finishScene();
		renderDevice->endRenderList();

#if ENABLE_IMGUI
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Windows")) {
				if (!loadedSarifHQ && ImGui::MenuItem("Load det_sarifhq_rail_tutorial")) {
					loadedSarifHQ = true;
					Gameloop::InitiateLevelLoad("det_sarifhq_rail_tutorial", nullptr);
					cdc::getDefaultFileSystem()->processAll();
				}
				if (ImGui::MenuItem("Capture frame")) {

					selectedCapture = captures.size();
					captures.push_back({renderDevice->captureRenderLists(), scene});
				}
				if (ImGui::MenuItem("Show drawables")) { showDrawablesWindow = true; }
				// if (ImGui::MenuItem("Show filesystem")) { showFilesystemWindow = true; }
				if (ImGui::MenuItem("Show objects")) { showObjectsWindow = true; }
				if (ImGui::MenuItem("Show DRMs")) { showDRMWindow = true; }
				if (ImGui::MenuItem("Show units")) { showUnitsWindow = true; }
				if (ImGui::MenuItem("Show loaded units")) { showLoadedUnitsWindow = true; }
				if (ImGui::MenuItem("Show strings")) { showStringsWindow = true; }
				ImGui::EndMenu();
			}
			if (mouseLook)
				ImGui::Text("Press TAB or ESC to release cursor");
			else
				ImGui::Text("Press TAB to grab cursor");
			ImGui::EndMainMenuBar();
		}

		imGuiDrawable.lastMinuteAdditions = [&]() {
			if (showDrawablesWindow) {
				cdc::CommonScene *xscene = captures[selectedCapture].second;
				if (!xscene)
					xscene = scene;
				ImGui::Begin("Scene drawables", &showDrawablesWindow);
				ImGui::BeginChild("capture list", ImVec2(0, 150), true);
				for (uint32_t i = 0; i < captures.size(); i++) {
					ImGui::PushID(i);
					const char *name = i ? "capture" : "live";
					if (ImGui::Selectable(name, i == selectedCapture)) {
						selectedCapture = i;
						renderDevice->revisitRenderLists(captures[i].first);
					}
					ImGui::PopID();
				}
				ImGui::EndChild();

				ImGui::Text("Note: 'Capture frame' is in main menu for now");
				// doesn't work within lastMinuteAdditions
				// if (ImGui::Button("Capture frame")) {
				// 	selectedCapture = captures.size();
				// 	captures.push_back({renderDevice->captureRenderLists(), scene});
				// }

				// buildUI(xscene->drawableListsAndMasks);
				buildUI(&renderDevice->renderPasses, xscene->drawableListsAndMasks);
				buildUI(xscene);
				ImGui::End();
			}
		};
		if (showFilesystemWindow) {
			// TODO
		}
		if (showObjectsWindow) {
			ImGui::Begin("Objects", &showObjectsWindow);
			cdc::buildObjectsUI();
			ImGui::End();
		}
		if (showDRMWindow) {
			drmexplorer.draw(&showDRMWindow);
		}
		if (showUnitsWindow) {
			ImGui::Begin("Units", &showUnitsWindow);
			buildUnitsUI();
			ImGui::End();
		}
		if (showLoadedUnitsWindow) {
			ImGui::Begin("Loaded units", &showLoadedUnitsWindow);
			StreamUnit *unit = STREAM_GetStreamUnitWithID(0);
			cdc::Level *level = unit->level;
			ImGui::DragInt2("visible intros", introShowRange);
			ImGui::DragInt2("visible IMFs", imfShowRange);
			if (!level) {
				ImGui::Text("not loaded");
			} else {
				ImGui::Text("level %p", level);

				cdc::CellGroupData *cellGroupData = level->sub50;
				uint32_t numCells = cellGroupData->header->numTotalCells;
				for (uint32_t i=0; i < numCells; i++) {
					cdc::CellData *cell = cellGroupData->cells[i];
					ImGui::Text("cell %i: %s", i, cell->sub0->name);
					// RenderTerrain at cell->sub4->pTerrain
					// RenderMesh at cell->renderMesh (for visibility)
					ImGui::SameLine();
  					ImGui::PushID(i);
					if (ImGui::SmallButton("Teleport to")) {
						float *mins = cell->sub0->vec10;
						float *maxs = cell->sub0->vec20;
						cameraPos.x = (mins[0] + maxs[0]) / 2.0f;
						cameraPos.y = (mins[1] + maxs[1]) / 2.0f;
						cameraPos.z = (mins[2] + maxs[2]) / 2.0f;
					}
					ImGui::PopID();
				}

				auto *admd = level->admdData;
				for (uint32_t i=0; i < admd->numObjects; i++) {
					auto &intro = admd->objects[i];
					auto oid = intro.objectListIndex;
					auto name = oid >= cdc::g_objectManager->objectList->count
						? "???": cdc::g_objectManager->objectList->entries[oid].name;
					ImGui::Text("  [%3d] intro %s (%d) %f",
						i, name, oid, intro.scale[0]);
					ImGui::SameLine();
  					ImGui::PushID(i);
					if (ImGui::SmallButton("Teleport to")) {
						cameraPos.x = intro.position[0];
						cameraPos.y = intro.position[1];
						cameraPos.z = intro.position[2];
					}
					ImGui::PopID();
				}
			}
			ImGui::End();
		}
		if (showStringsWindow) {
			if (ImGui::Begin("Strings", &showStringsWindow)) {
				for (uint32_t i = 0; i < 1000; i++)
					ImGui::Text("%5d %s", i, localstr_get(i));
			}
			ImGui::End();
		}

		// mainMenuInstance.buildUI();
#endif

		renderDevice->drawRenderLists();

		///////////////////////////////////////////////////////////////////////////////////////////

		renderContext->present();

		cdc::GameShell::UpdateLoop(1.0f /*TODO*/);
	}
end:
#if ENABLE_IMGUI 
	ImGui_ImplDX11_Shutdown();
#ifdef _WIN32
	ImGui_ImplWin32_Shutdown();
#else
	ImGui_ImplSDL2_Shutdown();
#endif
	ImGui::DestroyContext();
#endif
	return 0;
}

bool SpinnyCubePass::pre(
	cdc::CommonRenderDevice *renderDevice,
	uint32_t passId,
	uint32_t drawableCount,
	uint32_t priorPassesBitfield)
{
	auto *deviceContext = static_cast<cdc::PCDX11RenderDevice*>(cdc::g_renderDevice)->getD3DDeviceContext();

	deviceContext->RSSetViewports(1, viewport);

	// hack hack, one of the lights gets too close to the camera
	cdc::deviceManager->getStateManager()->m_rasterizerDesc.DepthClipEnable = false;

	cdc::deviceManager->getStateManager()->setDepthState(D3D11_COMPARISON_LESS, true);
	deviceContext->OMSetBlendState(keepAlphaBlend, nullptr, 0xffffffff);

	return true;
}

void SpinnyCubePass::post(
	cdc::CommonRenderDevice *renderDevice,
	uint32_t passId)
{
	// empty
}

void ImGuiDrawable::draw(uint32_t funcSetIndex, IRenderDrawable *other) {
#if ENABLE_IMGUI
	if (lastMinuteAdditions)
		lastMinuteAdditions();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
#endif
}
