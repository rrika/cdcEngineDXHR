#pragma once
#include <cstdint>
#include <vector>
#include "cdcAnim/IAnimGraphMessageListener.h"
#include "game/dtp/pickup.h"

namespace cdc {

class BinaryReader;
class BinaryWriter;

}

struct InventorySlotSub {
	// uint16_t word0 = 0;
	// uint8_t byte2 = 0;
	// uint8_t byte3 = 0;
	// uint8_t byte4 = 0;
	// uint8_t byte5 = 0;
	// uint8_t byte6 = 0;
	// uint8_t byte7 = 0;
	// uint8_t byte8 = 0;
	// uint32_t dwordC = ~0u;
	// uint8_t byte10 = 0;
};

struct InventorySlot {
	uint32_t dtpId = ~0u; // 0
	// uint8_t byte4 = 1;
	uint8_t transpose = 0; // 4
	uint16_t count = 0; // 6
	uint32_t x = 0; // 8
	uint32_t y = 0; // C
	uint8_t slotIndex = 0; // 10
	InventorySlotSub sub; // 14
};

class IInventoryContainer {
public:
	virtual ~IInventoryContainer() = default;
	virtual int32_t add(uint32_t, int32_t amount, InventorySlotSub*, std::vector<InventorySlot*>*) = 0; // 4
	virtual uint32_t removeByType(uint32_t, uint32_t amount) = 0; // 8
	virtual uint32_t removeFromSlot(uint32_t, uint32_t amount) = 0; // C
	virtual uint32_t removeWholeStack(uint32_t) = 0; // 10
	virtual void clear() = 0; // 14
	virtual uint32_t totalOfType(uint32_t) = 0; // 18
	virtual std::vector<InventorySlot*> collectAllSlots() = 0; // 1C
	virtual std::vector<InventorySlot*> collectSlotsOfCategory(dtp::Pickup::Category, bool noDuplicates) = 0; // 20
	virtual std::vector<InventorySlot*> collectSlotsOfType(uint32_t) = 0; // 24
	virtual uint32_t countSlotsInCategory(dtp::Pickup::Category cat, bool noDuplicates) = 0; // 28
	virtual InventorySlot *getSlot(uint32_t) = 0; // 2C
	virtual InventorySlot *firstSlotOfType(uint32_t) = 0; // 30
	virtual bool checkFit(std::vector<uint32_t>&, std::vector<uint32_t>&) = 0; // 34
	virtual bool serialize(cdc::BinaryWriter&) = 0; // 38
	virtual bool deserialize(cdc::BinaryReader&) = 0; // 3C
};

class InventorySystem :
	// public BaseSystem,
	public cdc::IAnimGraphMessageListener
{
	// inheriting: NsActor *systemOf; // 4
	// inheriting: Instance *instance; // 8
	IInventoryContainer *ic; // 10
	// PrimaryWeapon *primaryWeapon; // 14
	// uint32_t dword18;
	// uint32_t dword1C;
	// uint32_t dword20;
	// uint8_t byte24;
	// uint8_t byte25;
	// uint8_t byte26;
	// uint8_t byte27;
	// uint8_t byte28;
	// uint8_t byte29;
	// uint32_t dword2C;
	// uint8_t byte30;
	// uint8_t byte31;
	// uint8_t byte32;
	// uint8_t byte33;
	// uint64_t oword40;
	// uint32_t dword50;
	// uint32_t dword54;
	// uint32_t dword58;

public:
	virtual ~InventorySystem() = default;
	virtual void method4();
	virtual void method8();
	virtual void methodC();
	virtual void method10();
	virtual void method14();
	virtual void method18();
	virtual void method1C() = 0;
	virtual void method20();
	virtual void method24();
	virtual void method28();
	virtual void method2C();
	virtual void serialize(cdc::BinaryWriter&); // 30
	virtual void deserialize(cdc::BinaryReader&); // 34
	virtual bool haveItem(uint16_t);
	virtual void method3C();
	virtual void method40();
	virtual void method44();
};
