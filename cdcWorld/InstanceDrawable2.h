class MeshComponent : public InstanceComponent {
	Instance *m_instance;
	int8_t m_currentRenderModel;
	int8_t m_currentBaseModel;
	float m_drawgroupFadeValue;
	uint64_t m_noDrawGroups;
	uint32_t m_fxNoDrawGroups;
	ModelChangedCallback m_modelChangedCallback;
	void *m_callbackData;

public:
	MeshComponent(Instance *instance) : m_instance(instance) {}
};
