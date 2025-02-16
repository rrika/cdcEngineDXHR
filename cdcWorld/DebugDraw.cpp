#include <cstdint>
#include "DebugDraw.h"
#include "GameTracker.h"
#include "cdcCollide/AABBCollision.h"
#include "cdcCollide/collideMB.h"
#include "cdcKit/Physics/MultibodyClient.h"
#include "cdcPhysics/MultibodyAABBCollision.h"
#include "cdcPhysics/PhysicsGeometry.h"
#include "cdcSys/Color.h"

using namespace cdc;

uint32_t g_DrawGeomsAABBTree = 0;
bool g_DrawGeomsAABBPairs = false;
bool g_DrawGeomsAABB = false;
bool g_DrawGeoms = false;
bool g_DrawGeomsInfo = false;
float g_DrawGeomsDist = 2048.f;

void DEBUG_DrawGeometry(PhysicsGeometryImpl *geom, int color2, bool nowireframe, bool ignoreSkipStat);
void DEBUG_DrawPrimitive(PhysicsGeometryImpl *geom, int color1, int color2, bool nowireframe, float AddSizeCm);

void DEBUG_DrawGeometries(Instance *pCameraOwnerInstance, GameTracker *gameTracker) { // line 3729
	MultibodyAABBCollision *aabbCollision = gMultibodySystem->m_aabbCollision;
	if (g_DrawGeomsAABBTree > 0) {
		uint32_t ns = 0;
		struct {
			AABBCollisionTreeNode *node;
			uint32_t depth;
		} stack[256];
		stack[ns++] = {aabbCollision->primTreeRootNode, 0};

		while (ns) {
			auto [node, depth] = stack[--ns];
			if (depth == g_DrawGeomsAABBTree) {
				BBox box {
					{sortableIntToFloat(node->m_min[0]),
					 sortableIntToFloat(node->m_min[1]),
					 sortableIntToFloat(node->m_min[2])},
					{sortableIntToFloat(node->m_max[0]),
					 sortableIntToFloat(node->m_max[1]),
					 sortableIntToFloat(node->m_max[2])}
				};
				// DebugRender::s_instance->DrawWireframeBox(box, node->IsLeaf() ? ColorYELLOW : ColorCYAN);
			}
			if (!node->IsLeaf()) {
				stack[ns++] = {node->GetLeftChild(), depth+1};
				stack[ns++] = {node->GetRightChild(), depth+1};
			}
		}
	}
	if (g_DrawGeomsAABBPairs) {

	}
	if (g_DrawGeomsAABB || g_DrawGeoms) {
		float limitSq = g_DrawGeomsDist * g_DrawGeomsDist * 3.f;
		for (PhysicsGeometryImpl *geom = gMultibodySystem->firstGeometry; geom; geom = geom->next) {
			// if (g_DrawGeomsInfo)
			// 	DEBUG_DrawGeometriesInfo(pCameraOwnerInstance, gameTracker, TODO, geom->clientData, geom)
			float distSq = 0.f; // TODO
			if (distSq <= limitSq)
				DEBUG_DrawGeometry(geom, /*color=*/0, /*nowireframe=*/false, /*ignoreSkipStat=*/false);
		}
	}
}

void DEBUG_DrawGeometry(PhysicsGeometryImpl *geom, int color2, bool nowireframe, bool ignoreSkipStat) { // line 3827
	int color1 = color2;
	if (!ignoreSkipStat) {
		// TODO
	}
	if (color1 == 0) {
		color1 = 0x80FF0000;
		// TODO
	}
	DEBUG_DrawPrimitive(geom, color1, color2, nowireframe, /*AddSizeCm=*/0.f);
}

void DEBUG_DrawPrimitive(PhysicsGeometryImpl *geom, int color1, int color2, bool nowireframe, float AddSizeCm) { // line 4002
	switch (geom->type) {
	case PhysicsGeometryImpl::Box: break;
	case PhysicsGeometryImpl::Capsule: break;
	case PhysicsGeometryImpl::Plane: break;
	case PhysicsGeometryImpl::Sphere: break;
	case PhysicsGeometryImpl::CDCGeom: break;
	case PhysicsGeometryImpl::Unknown: break;
	}
}
