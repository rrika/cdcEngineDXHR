#include <cstdint>
#include "cdcMath/VectorInlines.h"
#include "cdcGeom.h"
#include "GeomBox.h"
#include "GeomDisc.h"
#include "GeomPoint.h"
#include "GeomRect.h"
#include "GeomSegment.h"
#include "GeomSphere.h"
#include "GeomSweep.h"
#include "GeomTriangle.h"
#include "GeomWrap.h"

enum GeomType : uint16_t {
	kGeomSphere = 0x0,
	kGeomBox = 0x1,
	kGeomRect = 0x2,
	kGeomDisc = 0x3,
	kGeomPoint = 0x4,
	kGeomSweep = 0x5,
	kGeomWrap = 0x6,
	kGeomSegment = 0x7,
	kGeomTriangle = 0x8,
};

struct HGeomCommand {
	HGeomCommand *gc1;  //  0
	HGeomCommand *gc2;  //  4
	GeomType type;      //  8
	float *floatList;   //  C
	float floatData[0]; // 10
};

namespace cdc {

bool Intersects(Geom const& g1, Geom const& g2, float boundaryTolerance, int maxIterations) { // line 164
	// Gilbert-Johnson-Keerthi algorithm for overlap detection

	Vector3 c1 = g1.GetGlobalCenter();
	Vector3 c2 = g2.GetGlobalCenter();
	Vector3 cd = c2-c1;

	Vector3 dir = -cd;
	if (dir.IsZero3())
		return true;

	// A
	Vector3 a = g2.GetGlobalSupportPoint(dir) - g1.GetGlobalSupportPoint(-dir);
	if (Dot3(a, dir) <= 0.f)
		return false;

	dir = {Cross3(a, dir)};
	if (dir.IsZero3())
		return true;

	// C
	Vector3 c = g2.GetGlobalSupportPoint(dir) - g1.GetGlobalSupportPoint(-dir);
	if (Dot3(c, dir) <= 0.f)
		return false;

	dir = {Cross3(a-cd, c-cd)};

	// flip sign
	if (Dot3(dir, cd) > 0.f) {
		dir = -dir;
		std::swap(a, c);
	}

	while (maxIterations-- > 0) {
		// B
		Vector3 b = g2.GetGlobalSupportPoint(dir) - g1.GetGlobalSupportPoint(-dir);
		if (Dot3(b, dir) <= 0.f)
			return false;

		if (Vector3 n = {Cross3(a, b)}; Dot3(n, cd) < 0.f) {
			c = b; // discard C
			dir = {Cross3(a-cd, b-cd)};
			continue;

		} else if (Vector3 n = {Cross3(a, c)}; Dot3(n, cd) < 0.f) {
			a = b; // discard A
			dir = {Cross3(b-cd, c-cd)};
			continue;
		}

		while (maxIterations-- > 0) {
			dir = {Cross3(b-a, c-a)};
			if (Dot3(dir, a) >= 0.f)
				return true;

			Vector3 d = g2.GetGlobalSupportPoint(dir) - g1.GetGlobalSupportPoint(-dir);
			dir.Normalize3();
			if (Dot3(d, dir) >= 0.f)
				return false;

			if (boundaryTolerance >= Dot3(dir, d-b))
				return false;

			Vector3 indicator = {Cross3(cd, d)};

			if (Dot3(indicator, a) <= 0.f) {
				if (Dot3(indicator, b) <= 0.f)
					a = d;
				else
					c = d;

			} else {
				if (Dot3(indicator, c) <= 0.f)
					b = d;
				else
					a = d;
			}
		}
	}
	return false;
}

Geom *NEW_Geom(HGeomCommand *cmd) { // line 572
	cmd->floatList = cmd->floatData;
	Vector3 pos {
		cmd->floatData[0],
		cmd->floatData[1],
		cmd->floatData[2]};

	switch (cmd->type) {
	case kGeomSphere:
		return new GeomSphere(pos, /*radius=*/ cmd->floatData[3]);

	case kGeomBox: {
		Euler euler{
			cmd->floatData[3],
			cmd->floatData[4],
			cmd->floatData[5]};
		Vector3 extents {
			cmd->floatData[6],
			cmd->floatData[7],
			cmd->floatData[8]};
		Quat quat(euler);
		auto *box = new GeomBox(extents);
		box->m_position = pos;
		box->m_rotation = quat;
		return box;
	}

	case kGeomRect: {
		Euler euler{
			cmd->floatData[3],
			cmd->floatData[4],
			cmd->floatData[5]};
		Vector2 extents {
			cmd->floatData[6],
			cmd->floatData[7]};
		Quat quat(euler);
		auto *rect = new GeomRect(extents);
		rect->m_position = pos;
		rect->m_rotation = quat;
		return rect;
	}

	case kGeomDisc: {
		Euler euler{
			cmd->floatData[3],
			cmd->floatData[4],
			cmd->floatData[5]};
		Quat quat(euler);
		float radius = cmd->floatData[6];
		auto *disc = new GeomDisc(pos, quat, radius);
		return disc;
	}

	case kGeomPoint:
		return new GeomPoint(pos);

	case kGeomSweep: {
		Euler euler{
			cmd->floatData[3],
			cmd->floatData[4],
			cmd->floatData[5]};
		Quat quat(euler);
		auto *g1 = NEW_Geom(cmd->gc1);
		auto *g2 = NEW_Geom(cmd->gc2);
		auto *sweep = new GeomSweep(g1, g2);
		sweep->m_position += pos;
		sweep->m_rotation = quat;
		return sweep;
	}

	case kGeomWrap: {
		Euler euler{
			cmd->floatData[3],
			cmd->floatData[4],
			cmd->floatData[5]};
		Quat quat(euler);
		auto *g1 = NEW_Geom(cmd->gc1);
		auto *g2 = NEW_Geom(cmd->gc2);
		auto *wrap = new GeomWrap(g1, g2);
		wrap->m_position += pos;
		wrap->m_rotation = quat;
		return wrap;
	}

	case kGeomSegment:
		return new GeomSegment(
			Vector3{
				cmd->floatData[3],
				cmd->floatData[4],
				cmd->floatData[5]},
			Vector3{
				cmd->floatData[6],
				cmd->floatData[7],
				cmd->floatData[8]});

	case kGeomTriangle:
		return new GeomTriangle(); // TODO
	}
}

Geom *BuildCDCGeom(HGeomCommand *cmd) {
	return NEW_Geom(cmd);
}

}
