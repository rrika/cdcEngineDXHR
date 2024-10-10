#pragma once
#include <cstdint>
#include <vector>

namespace cdc { class CdcPlayer; };
namespace dtp { struct Interaction; };

class NsInstance;
class ConversationBase;

class ConversationManager {
	std::vector<ConversationBase*> m_conversations;
	uint32_t dwordC;
	uint32_t dword10;
	uint32_t dword14;
	uint8_t byte18;
	uint32_t dword1C;
	uint32_t dword20;
	uint32_t dword24;
public:
	void *create(cdc::CdcPlayer *player, NsInstance *instance, dtp::Interaction *interaction);
	// void serialize(BinaryWriter&);
	// void deserialize(BinaryReader&);
};

