#pragma once

namespace cdc {
	class IMaterial;
	class IRenderTerrain;
	class RenderMesh;
	class ScriptType;
	struct MaterialBlobSub;
	struct ModelBatch;
	struct VertexDecl;
}

namespace dtp {
	struct Intro;
	struct IMFRef;
}

class Instance;

struct UIActions {
	virtual void select(cdc::IRenderTerrain *renderTerrain) = 0;
	virtual void select(cdc::RenderMesh*) = 0;
	virtual void select(cdc::ModelBatch*) = 0;
	virtual void select(cdc::VertexDecl*) = 0;
	virtual void select(cdc::IMaterial*) = 0;
	virtual void select(cdc::MaterialBlobSub*) = 0;
	virtual void select(cdc::ScriptType*) = 0;
	virtual void select(dtp::Intro *intro) = 0;
	virtual void select(dtp::IMFRef *imfRef) = 0;
	virtual void select(Instance *instance) = 0;
};
