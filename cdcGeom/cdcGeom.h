#pragma once

struct HGeomCommand;

namespace cdc {

class Geom;
Geom *NEW_Geom(HGeomCommand *cmd);
Geom *BuildCDCGeom(HGeomCommand *cmd);

}
