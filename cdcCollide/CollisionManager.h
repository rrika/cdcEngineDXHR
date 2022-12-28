#pragma once

namespace cdc {

class MultibodySystemImpl;

void CollideMesh(MultibodySystemImpl *mb, float timeStep);
void CollideGeometries(MultibodySystemImpl *mb);

}
