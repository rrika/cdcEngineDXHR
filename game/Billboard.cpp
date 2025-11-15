#include <cstdio>
#include <cstring>
#include "Billboard.h"
#include "cdcMath/Math.h"
#include "cdcMath/MatrixInlines.h"
#include "cdcMath/VectorInlines.h"
#include "cdcRender/CommonRenderDevice.h"

using namespace cdc;

void DrawBillboards(
	Matrix& matrix,
	uint32_t numBillboards,
	Billboard *billboards,
	Vector tint,
	Vector& param0,
	float zoffset)
{
	MaterialInstanceParams mip;

	Matrix& viewMatrix = g_renderDevice->getScene()->getViewMatrix(); // HACK
	Matrix& projectMatrix = g_renderDevice->getScene()->getProjectMatrix();
	Matrix viewProjectMatrix = projectMatrix * viewMatrix;

	// the shader will attenuate the lens flare by
	// sampling the depth buffer at   param[0].xy
	// and comparing to               param[0].z
	param0 = Vector{matrix.m[3][0], matrix.m[3][1], matrix.m[3][2], matrix.m[3][3]};
	// project
	param0 = viewProjectMatrix * Vector4{param0};
	param0.x = param0.x / param0.w;
	param0.y = param0.y / param0.w;
	param0.z = param0.z / param0.w;
	// move to texture space
	param0.x =  0.5f * param0.x + 0.5f;
	param0.y = -0.5f * param0.y + 0.5f;
	// apply fudge factor against z-fighting
	param0.z =         param0.z + zoffset;
	mip.m_shaderConstants[0] = {param0};

	for (uint32_t i=0; i<numBillboards; i++) {
		Billboard& b = billboards[i];
		if (b.material == nullptr)
			continue;

		uint32_t flags = POLYFLAG_QUADLIST | POLYFLAG_DOUBLESIDED; // 0x10040

		Vector v0, v1, v2, v3;
		uint32_t diffuse;
		Vector normal {0, 1, 0}; // TODO

		{
			uint8_t color[4];
			memcpy((void*)color, (void*)&b.color, 4);

			color[0] = (255.f * color[0] * tint.x) / 255.f;
			color[1] = (255.f * color[1] * tint.y) / 255.f;
			color[2] = (255.f * color[2] * tint.z) / 255.f;
			color[3] = (255.f * color[3] * tint.w) / 255.f;

			std::swap(color[0], color[2]);
			memcpy((void*)&diffuse, (void*)color, 4);
		}

		switch (b.mode) {
		case 0: {
			flags |= POLYFLAG_2D; // 0x10010040
			Vector center = viewProjectMatrix *
				Vector4{matrix.m[3][0], matrix.m[3][1], matrix.m[3][2], matrix.m[3][3]};
			center.x = center.x / center.w;
			center.y = center.y / center.w;
			center.x = 0.5f * center.x + 0.5f;
			center.y = 0.5f * center.y + 0.5f;
			center.z = 85.f;
			center.w = 0.f;

			if (b.mode0.oppositeness != 0.f) {
				Vector alt {1-center.x, 1-center.y, center.z};
				center += (alt+-center) * b.mode0.oppositeness;
			}

			Vector x = {b.mode0.sizeX / 1280.f, 0.f, 0.f, 0.f};
			Vector y = {0.f, b.mode0.sizeY / 720.f, 0.f, 0.f};
			Vector v03 = center +- x;
			Vector v12 = center + x;
			v0 = v03 +- y;
			v1 = v12 +- y;
			v2 = v12 + y;
			v3 = v03 + y;
			break;
		}
		case 1:
		case 2:
			// TODO
			continue;
		default:
			break;
		}


		RenderVertexShadedDiffuse v[4];

		v[0].vX = v0.x;
		v[0].vY = v0.y;
		v[0].vZ = v0.z;
		v[0].diffuse = diffuse;
		v[0].vU = b.u0;
		v[0].vV = b.v0;
		v[0].nNX = normal.x;
		v[0].nNY = normal.y;
		v[0].nNZ = normal.z;

		v[1].vX = v1.x;
		v[1].vY = v1.y;
		v[1].vZ = v1.z;
		v[1].diffuse = diffuse;
		v[1].vU = b.u1;
		v[1].vV = b.v0;
		v[1].nNX = normal.x;
		v[1].nNY = normal.y;
		v[1].nNZ = normal.z;


		v[2].vX = v2.x;
		v[2].vY = v2.y;
		v[2].vZ = v2.z;
		v[2].diffuse = diffuse;
		v[2].vU = b.u1;
		v[2].vV = b.v1;
		v[2].nNX = normal.x;
		v[2].nNY = normal.y;
		v[2].nNZ = normal.z;


		v[3].vX = v3.x;
		v[3].vY = v3.y;
		v[3].vZ = v3.z;
		v[3].diffuse = diffuse;
		v[3].vU = b.u0;
		v[3].vV = b.v1;
		v[3].nNX = normal.x;
		v[3].nNY = normal.y;
		v[3].nNZ = normal.z;

		g_renderDevice->DrawPrimitiveShadedDiffuse(
			&identity4x4,
			b.material,
			v,
			1,
			flags | (b.depthFuncLE ? 0 : POLYFLAG_FADING),
			0.0,
			&mip,
			0x52CB);
	}
}
