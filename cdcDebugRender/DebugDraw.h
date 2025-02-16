#include "DebugRender.h"

namespace cdc {

class CommonRenderDevice;

class DebugDraw : public DebugRender {
	CommonRenderDevice *m_pRenderDevice;
	Matrix m_cLocalToWorld;
	bool m_bPerspective;
	float m_curYPos;
	float m_curXPos;
	float m_visibleZNear;
	float m_visibleZFar;
	char *m_TextBuffer;
	int m_TextBufferUsage;
	int m_maxBufferSize;
	bool m_overflowed;
	float *m_sinSphereArc;
	float *m_cosSphereArc;
	float *m_sinSphereSeg;
	float *m_cosSphereSeg;
	float *m_sinConeSeg;
	float *m_cosConeSeg;
	bool (*m_sphereVisCallback)(Vector3*, float);
public:
	void DrawLine(Vector3Arg v0, Vector3Arg v1, Vector4Arg color) override;
	void Flush() override;
};

}
