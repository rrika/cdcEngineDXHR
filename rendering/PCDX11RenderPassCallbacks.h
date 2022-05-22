#include "IRenderPassCallback.h"

namespace cdc {

class PCDX11NormalPassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11DeferredShadingPassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11DepthPassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11CompositePassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11OpaquePassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11TranslucentPassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11AlphaBloomFSXPassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11PredatorPassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11FullScreenFXPassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11PostFSXPassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

class PCDX11DepthDependentPassCallbacks : public IRenderPassCallback {
public:
	bool pre(
		CommonRenderDevice *renderDevice,
		uint32_t passId,
		uint32_t drawableCount,
		uint32_t priorPassesBitfield) override;
	void post(
		CommonRenderDevice *renderDevice,
		uint32_t passId) override;
};

extern PCDX11NormalPassCallbacks normalPassCallbacks;
extern PCDX11DeferredShadingPassCallbacks deferredShadingPassCallbacks;
extern PCDX11DepthPassCallbacks depthPassCallbacks;
extern PCDX11CompositePassCallbacks compositePassCallbacks;
extern PCDX11OpaquePassCallbacks opaquePassCallbacks;
extern PCDX11TranslucentPassCallbacks translucentPassCallbacks;
extern PCDX11AlphaBloomFSXPassCallbacks alphaBloomFSXPassCallbacks;
extern PCDX11PredatorPassCallbacks predatorPassCallbacks;
extern PCDX11FullScreenFXPassCallbacks fullScreenFXPassCallbacks;
extern PCDX11PostFSXPassCallbacks postFSXPassCallbacks;
extern PCDX11DepthDependentPassCallbacks depthDependentPassCallbacks;

}
