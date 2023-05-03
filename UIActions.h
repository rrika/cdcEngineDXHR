#pragma once

namespace cdc {
	class IMaterial;
	class RenderMesh;
	class ScriptType;
	struct MaterialBlobSub;
	struct ModelBatch;
}

struct UIActions {
	virtual void select(cdc::RenderMesh*) = 0;
	virtual void select(cdc::ModelBatch*) = 0;
	virtual void select(cdc::IMaterial*) = 0;
	virtual void select(cdc::MaterialBlobSub*) = 0;
	virtual void select(cdc::ScriptType*) = 0;
};
