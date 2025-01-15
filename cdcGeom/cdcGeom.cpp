#include <cstdint>
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

Geom *NEW_Geom(HGeomCommand *cmd) {
	cmd->floatList = cmd->floatData;
	Vector3 pos {
		cmd->floatData[0],
		cmd->floatData[1],
		cmd->floatData[2]};

	switch (cmd->type) {
	case kGeomSphere:
		return new GeomSphere(pos, /*radius=*/ cmd->floatData[3]);

	case kGeomBox:
		return new GeomBox(); // TODO

	case kGeomRect:
		return new GeomRect(); // TODO

	case kGeomDisc:
		return new GeomDisc(); // TODO


	case kGeomPoint:
		return new GeomPoint(pos);

	case kGeomSweep:
		return new GeomSweep(); // TODO

	case kGeomWrap:
		return new GeomWrap(); // TODO

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
