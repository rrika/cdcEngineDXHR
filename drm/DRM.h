#pragma once
#include <stdint.h>

namespace cdc {

enum ContentType /* class ContentType : uint8_t*/ {
	Generic = 0,
	Empty = 1,
	Animation = 2,
	RenderResource = 5,
	FMODSoundBank = 6,
	DTPData = 7,
	Script = 8,
	ShaderLib = 9,
	Material = 10,
	Object = 11,
	RenderMesh = 12,
	CollisionMesh = 13,
	StreamGroupList = 14,
	AnyType = 15
};

struct DRMSectionHeader {
	uint32_t payloadSize;
	ContentType type;
	uint8_t  unknown05;
	uint16_t unknown06;
	struct {
		bool     singleFlag : 1;
		uint8_t  allocFlags : 7;
		uint32_t relocSize : 24;
	};
	uint32_t id;
	uint32_t languageBits;
};

struct DRMHeader {
	uint32_t version;
	uint32_t dependencyDrmListSize;
	uint32_t dependencyObjListSize;
	uint32_t unknown0C;
	uint32_t unknown10;
	uint32_t flags;
	uint32_t sectionCount;
	uint32_t rootSection;
};

}
