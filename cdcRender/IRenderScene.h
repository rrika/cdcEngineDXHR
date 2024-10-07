#pragma once

namespace cdc {

class RenderViewport;
class IRenderTarget;
class IDepthBuffer;
struct Matrix;

class IRenderScene {
public:
	virtual Matrix& getViewMatrix() = 0;
	virtual void scene4() = 0;
	virtual Matrix& getProjectMatrix() = 0;
	virtual void sceneC() = 0;
	virtual void getCameraPosition(float *pos) = 0;
	virtual void getCameraDirection(float *dir) = 0;
	virtual RenderViewport& getViewport() = 0;
	virtual IRenderTarget *getRenderTarget() = 0;
	virtual IDepthBuffer *getDepthBuffer() = 0;
	virtual void scene24() = 0;
	virtual void scene28() = 0;
};

}
