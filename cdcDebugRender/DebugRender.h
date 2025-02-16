#pragma once
#include "cdcMath/Math.h"

namespace cdc {

class DebugRender {
public:
	static DebugRender *s_instance;

	void DrawArc(
		Vector3 *centerPos, float radius, float startAngle, float stopAngle,
		Color *color, Matrix *mat, const bool noFudge, int numSubdivisions);

	virtual void DrawLine(Vector3Arg v0, Vector3Arg v1, Vector4Arg color) = 0;
	virtual void Flush() = 0;

  /*
  float (__thiscall *GetVisibleZNear)(cdc::DebugRender *this);
  _BYTE gap4[16];
  void (*RenderString)(cdc::DebugRender *this, float, float, const char *, ...);
  void (*RenderString)(cdc::DebugRender *this, int, int, const char *, ...);
  void (*RenderString)(cdc::DebugRender *this, cdc::Color *, const char *, ...);
  void (*RenderString)(cdc::DebugRender *this, const char *, ...);
  void (*RenderStringI)(cdc::DebugRender *this, int, int, float, float, float, const char *, ...);
  void (*RenderStringI)(cdc::DebugRender *this, int, int, const char *, ...);
  void (__thiscall *SetLocalToWorld)(cdc::DebugRender *this, cdc::Matrix *);
  void (__thiscall *DrawLine)(cdc::DebugRender *this, cdc::Vector3 *, cdc::Vector3 *, cdc::Color *);
  void (__thiscall *DrawLineLocal)(cdc::DebugRender *this, cdc::Vector3 *, cdc::Vector3 *, cdc::Color *);
  void (__thiscall *DrawScreenLine)(cdc::DebugRender *this, cdc::Vector3 *, cdc::Vector3 *, cdc::Color *);
  void (__thiscall *DrawTriangle)(cdc::DebugRender *this, cdc::Vector3 *, cdc::Vector3 *, cdc::Vector3 *, cdc::Color *);
  void (__thiscall *DrawTriangleLocal)(cdc::DebugRender *this, cdc::Vector3 *, cdc::Vector3 *, cdc::Vector3 *, cdc::Color *);
  void (__thiscall *DrawScreenTriangle)(cdc::DebugRender *this, cdc::Vector3 *, cdc::Vector3 *, cdc::Vector3 *, cdc::Color *);
  void (__thiscall *DrawQuadPatch)(cdc::DebugRender *this, cdc::Array<cdc::Array<cdc::Vector3> > *, cdc::Color *, bool);
  void (__thiscall *DrawQuadPatchLocal)(cdc::DebugRender *this, cdc::Array<cdc::Array<cdc::Vector3> > *, cdc::Color *, bool);
  void (__thiscall *DrawSphere)(cdc::DebugRender *this, cdc::Vector3 *, float, cdc::Color *);
  void (__thiscall *DrawCylinder)(cdc::DebugRender *this, cdc::Vector3 *, cdc::Vector3 *, float, cdc::Color *);
  void (__thiscall *DrawTube)(cdc::DebugRender *this, cdc::Array<cdc::Vector3> *, float, cdc::Color *);
  void (__thiscall *DrawLineSphere)(cdc::DebugRender *this, cdc::Vector3 *, float, cdc::Color *);
  void (__thiscall *DrawSphereLocal)(cdc::DebugRender *this, cdc::Vector3 *, float, cdc::Color *);
  void (__thiscall *DrawCone)(cdc::DebugRender *this, cdc::Matrix *, float, float, cdc::Color *);
  void (__thiscall *DrawSpotLightVolume)(cdc::DebugRender *this, cdc::Vector3 *, cdc::Vector3 *, float, float, cdc::Color *);
  void (__thiscall *DrawDirectionalLightVolume)(cdc::DebugRender *this, cdc::Vector3 *, cdc::Vector3 *, float, cdc::Color *);
  void (__thiscall *AddFlushCallback)(cdc::DebugRender *this, void (__cdecl *)(void *), void *);
  void (__thiscall *Release)(cdc::DebugRender *this);
  unsigned int (__thiscall *Flush)(cdc::DebugRender *this);
  void (__thiscall *PrintRenderStats)(cdc::DebugRender *this);
  */
};

}
