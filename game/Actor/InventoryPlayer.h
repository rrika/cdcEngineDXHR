#pragma once
#include <array>
#include <cstdint>
#include "InventorySystem.h"

class InventoryContainer : public IInventoryContainer {
public:
	uint8_t *m_grid = nullptr; // 4
protected:
	std::array<InventorySlot, 256> m_slots; // 8
	uint32_t gridWidth = 0; // 2808
	uint32_t gridHeight = 0; // 280C
	uint8_t nextFreeSlot = 0; // 2810

	// EmailOrIReaderDB *emailDatabase = nullptr; // 2814
	// uint32_t dword2818 = 0;

	InventoryContainer(InventoryContainer const& other);
	InventorySlot *findWithLimit(dtp::Pickup*, uint32_t, uint32_t, uint32_t&);
public:
	uint32_t totalFreeCells();
protected:
	void assignRectangle(
		uint32_t x, uint32_t y, uint32_t w, uint32_t h,
		bool transpose, uint32_t cellByte, uint8_t *grid);
	bool checkRectangle(
		uint32_t x, uint32_t y, uint32_t w, uint32_t h,
		uint8_t *grid, uint32_t refByte);
	bool findFreeSpot(dtp::Pickup*, InventorySlot&);
	void assignSlot(dtp::Pickup*, InventorySlot&);
	void growGrid(uint32_t, uint32_t);
	void removeFromGrid(uint32_t);
	bool reorganizeGridForOneMore(InventorySlot&, uint32_t);

public:
	InventoryContainer(uint32_t width, uint32_t height);
	~InventoryContainer();
	int32_t add(uint32_t, int32_t amount, InventorySlotSub*, std::vector<InventorySlot*>*) override;
	uint32_t removeByType(uint32_t, uint32_t amount) override;
	uint32_t removeFromSlot(uint32_t, uint32_t amount) override;
	uint32_t removeWholeStack(uint32_t) override;
	void clear() override;
	uint32_t totalOfType(uint32_t) override;
	std::vector<InventorySlot*> collectAllSlots() override;
	std::vector<InventorySlot*> collectSlotsOfCategory(dtp::Pickup::Category, bool noDuplicates) override;
	std::vector<InventorySlot*> collectSlotsOfType(uint32_t) override;
	uint32_t countSlotsInCategory(dtp::Pickup::Category cat, bool noDuplicates) override;
	InventorySlot *getSlot(uint32_t) override;
	InventorySlot *firstSlotOfType(uint32_t) override;
	bool checkFit(std::vector<uint32_t>&, std::vector<uint32_t>&) override;
	// bool serialize(BinaryWriter&) override;
	// bool deserialize(BinaryReader&) override;
};

class InventoryPlayer : public InventorySystem {
	uint32_t dword5C;
	uint32_t currentMoney; // 60
	// map map64;
	// uint8_t gap70[4];
	InventoryContainer *otherInventoryContainer; // 74
	// map map78;
	// uint32_t dword84;
	// uint32_t dword88;
	// uint32_t dword8C;
	// uint32_t slot90;
	// uint8_t byte94;
	// uint32_t dword98;
	// uint8_t byte9C;
	uint32_t quickbar[10]; // A0
	// uint32_t dwordC8;
	// uint8_t gapCC[40];
	// uint32_t dwordF4;
	// uint8_t byteF8;

public:
	bool CanCombine(uint32_t, uint32_t);
	bool CanDiscard(uint32_t, uint32_t);
	bool CanMoveAt(uint32_t, uint32_t, uint32_t, bool, uint32_t*);
	bool CanUse(uint32_t);

	void Combine(uint32_t, uint32_t);
	void Discard(uint32_t, uint32_t);
	void MoveAt(uint32_t, uint32_t, uint32_t, bool, uint32_t*);
	void Use(uint32_t);

};
